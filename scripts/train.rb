require "defines"
require "helper"
require "armor"

@start_plan = $args.first
@plan = [:utility, :locksmith, :outdoor]

if @start_plan
  @plan.each do |item|
    break if item.to_s.include?(@start_plan)
    @plan = @plan.rotate
  end
end

echo "Training plan - #{@plan.inspect}"

def reset
  sleep Rt::value
  until Status::standing
    put_wait "stand", /You stand|already standing/
    pause
  end
  if Spell::active.any?
    put_wait "khri stop", /unable to maintain|lose concentration|can no longer|attempt to relax/
    pause
  end
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
    stow_pick
    stow_box
  end

  def self.abort
    SYSTEM::finished = true
  end

  def self.do_run
    $args.clear
    Thread.new {
      load("bli.rb")
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
      load("col.rb")
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
      load("med.rb")
    }
  end
end

def abort_module thread, obj
if obj.respond_to? "abort"
    obj.send "abort"
    thread.join
  else
    thread.terminate
  end
end

def rewrite_finally thread, obj
  define_singleton_method "finally_do" do
    if thread
      abort_module thread, obj
    end
    Client::track_exp_clear
    sleep Rt::value
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
      abort_module thread, obj
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