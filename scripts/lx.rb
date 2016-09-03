# desc: trains brawling, appraisal and crossbow
# requirements: arm worn shield, set up stow containers
# run: hunting area
# use: .lx <target>

require "target"
require "hunt"
require "lx_perf"
require "spell"

@ammo = "bolt"
@circle_count = 1

Target::auto "*** attack what? usage: .lx &lt;critter_name&gt; ***"

Thread.new do
  while true
    spell_activate
    sleep 15
  end
end

def shoot
  put "aim"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :pause => [/You are still stunned/],
            :wait_arrive => [/you trying to attack/],
            :next => [/You begin to target/],
            :reload => [/isn't loaded/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      shoot
    when :pause
      pause 3
      shoot
    when :next
      app_locked = Exp::state("appraisal") > 32
      app unless app_locked

      if @circle_count > 0
        pause Rt::value
        if app_locked
          circle -1
        else
          circle 0
        end
      else
        fire
      end
    when :wait_arrive
      echo "*** WAITING ***"
      wait_for(/advance on you|melee range/)
      shoot
    when :reload
      reload
  end
end

def app
  put "appr #{$args.join(" ")} quick"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :pause => [/You are still stunned/],
            :next => [/Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      app
    when :pause
      pause 3
      app
  end
end

def circle count
  put "circle"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :next => [/Roundtime/],
            :pause => [/You are still stunned/],
            :adv => ["must be closer to use tactical abilities", "aren't close enough"],
            :stand => [/should stand up/] }
  result = match_wait match

  case result
    when :wait
      pause 0.2
      circle count
    when :stand
      put "stand"
      circle count
    when :pause
      pause 3
      circle count
    when :next
      if count < @circle_count - 1
        circle count + 1
      else
        fire
      end
    when :adv
      put "advance"
      pause 4
      circle count + 1
  end
end

def fire
  put "fire"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :pause => [/You are still stunned/],
            :reload => [/Roundtime|isn't loaded/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      fire
    when :pause
      pause 3
      fire
    when :reload
      reload
  end
end

def reload
  pause 0.5
  put "get #{@ammo}"
  put "load"
  wait_for_roundtime
  check_status
end

def check_status
  put "look items"
  match = { :wait => [/\.\.\.wait|you may only type ahead|able to move/],
            :pause => [/You are still stunned/],
            :dead => [/\bdead\b/],
            :continue => [/items in the area/] }
  result = match_wait match

  case result
    when :wait
      check_status
    when :dead
      load "skin"
    when :pause
      pause 3
      check_status
  end
end

10000.times do
  hunt
  shoot
end