require "defines"
require "helper"
require "armor"

@start_plan = $args.first
@plan = [:utility, :locksmith, :outdoor]

=begin

[The Black Spire, Stairwell]
The air, thick with smoke from dim oil lamps, is filled with the acrid odor of incense.  Stone steps rise into the gloom and descend to an oak door.  A dark shaft stretches upwards into shadow at the center of the tower.
Obvious exits: west, up.


[The Black Spire, Chamber of Death]
Oddly shaped to fit along the curved tower walls, chairs surround a modest table in the center of the room.  Pitted and marked by the points of countless knives, it is bare except for an elegantly detailed deobar box.  Near the table, an open chest contains short swords, serrated daggers, sharp pointed knives and bucklers of varying styles.  You also see a Dragon Priest assassin, a Dragon Priest assassin, an embroidery needle, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a Dragon Priest assassin, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt, a Dragon Priest assassin, a crossbow bolt, a crossbow bolt, a crossbow bolt, a crossbow bolt and some junk.
Obvious exits: east, southeast.
>

[Wyvern Mountain, Steep Trail] -- final
[Wyvern Mountain, Narrow Plateau] -- trail

stow box

=end

if @start_plan
  @plan.each do |item|
    break if item.to_s.include?(@start_plan)
    @plan = @plan.rotate
  end
end

echo "Train plan - #{@plan.inspect}"

def reset
  sleep Rt::value
  put_wait "stand", /You stand|already standing/ unless Status::standing
  put_wait "khri stop", /unable to maintain|lose concentration|can no longer|attempt to relax/ unless Spell::active.empty?
  pause
end

def stow
  sleep Rt::value
  put_wait "stow right", /Stow what|You put your/ unless Wield::right.empty?
  put_wait "stow left", /Stow what|You put your/ unless Wield::right.empty?
  pause
end

module Locksmith
  def self.before
    reset
  end

  def self.after
    #stow_pick
    #stow_box
  end

  def self.abort
    SYSTEM::finished = true
  end

  def self.do_run
    $args.clear
    Thread.new {
      load("bli")
    }
  end
end

module Outdoor
  def self.before
    reset
  end

  def self.after
    #put_wait "kick pile", /You take|could not find/ if Room::count_objects("pile") > 0
  end

  def self.abort
    SYSTEM::finished = true
  end

  def self.do_run
    $args.clear << "rock"
    Thread.new {
      load("col")
    }
  end
end

module Utility
  def self.before
    put_wait "sit", /sit down|already sitting/ unless Status::sitting
    put_wait "hide", /Roundtime/ unless Status::hidden
    put_wait "khri darken dampen steady shadowstep sagacity", /Roundtime/
  end

  def self.after
    reset
  end

  def self.do_run
    $args.clear
    Thread.new {
      load("med")
    }
  end
end

module Crossbow
  def self.before
    if Room::title.downcase.include?("crossing")
      unless Armor::wearing_armor?
        load("xvarmor")
      end
      load("bank")
      pause
      move "out"
      $args.clear << "shard"
      load("travel")
    end

    if Room::title == /\[Wyvern Trail, Journey's Rest\]/
      load("raven")
      move "se"
    end

    put "remove crossbow"
    put "stance custom"
    pause
  end

  def self.after
    echo ":: after crossbow ::"
    put "wear crossbow"
    pause
  end

  def self.do_run
    $args.clear
    Thread.new {
      load("lx")
    }
  end
end

def rewrite_finally thread, obj
  define_singleton_method "finally_do" do
    thread.terminate if thread
    obj.send("after")
  end
end

@plan.each do |item|
  next if Exp::state(item.to_s) > 30
  echo ":: #{item} ::"

  obj = Object.const_get(item.capitalize)
  obj.send "before" if obj.respond_to? "before"

  SYSTEM::finished = false

  thread = obj.send("do_run")
  rewrite_finally thread, obj
  while true
    exp = Exp::state(item.to_s)
    if exp > 32
      if obj.respond_to? "abort"
        obj.send "abort"
      else
        thread.terminate
      end
      sleep Rt::value
      obj.send "after" if obj.respond_to? "after"
      break
    end
    echo "exp #{item}: #{exp}"
    sleep 5
    unless thread.alive?
      sleep Rt::value
      thread = obj.send("do_run")
    end
  end
end