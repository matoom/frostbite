# desc: trains brawling, appraisal and crossbow
# requirements: arm worn shield, set up stow containers
# run: hunting area
# use: .lx <target>

require "lx_stat"
require "defines"

@ammo = "bolt"
@circle_count = 1

def finally_do
  Stats::report_total_stats
end

def check_health
  if Vitals::health < 50
    echo "*** LOW HP! ***"
  end
  pause 5
end

def start
  put "aim"
  put "appr #{$args.join(" ")} quick"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :health_check => [/You are still stunned/],
            :wait_arrive => [/you trying to attack/],
            :next => [/Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      start
    when :health_check
      check_health
      start
    when :next
      if(@circle_count > 0)
        pause Rt::value
        circle 0
      else
        fire
      end
    when :wait_arrive
      echo "*** WAITING ***"
      wait_for(/advance on you|melee range/)
      Stats::reset_timer
      start
  end
end

def circle count
  put "circle"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :next => [/Roundtime/],
            :health_check => [/You are still stunned/],
            :adv => ["must be closer to use tactical abilities"],
            :stand => [/should stand up/] }
  result = match_wait match

  case result
    when :wait
      pause 0.2
      circle count
    when :stand
      put "stand"
      circle count
    when :health_check
      check_health
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
  Stats::register_shot
  put "fire"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :health_check => [/You are still stunned/],
            :load => [/Roundtime|isn't loaded/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      fire
    when :health_check
      check_health
      fire
    when :load
      load
  end
end

def load
  pause 0.5
  put "get #{@ammo}"
  put "load"
  wait_for_roundtime
  check_status
end

def check_status
  put "look items"
  match = { :wait => [/\.\.\.wait|you may only type ahead|able to move/],
            :health_check => [/You are still stunned/],
            :dead => [/\bdead\b/],
            :continue => [/items in the area/] }
  result = match_wait match

  case result
    when :wait
      check_status
    when :dead
      Stats::register_kill
      arrange 0
    when :health_check
      check_health
      check_status
    when :continue
      start
  end
end

def arrange count
  put "arrange"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :health_check => [/You are still stunned/],
            :arrange => [/You begin to arrange|You continue arranging|complete arranging|You make a mistake/],
            :loot => [/arrange what|cannot be skinned|corpse is worthless now/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      arrange count
    when :health_check
      check_health
      arrange count
    when :arrange
      if count < GD::ARRANGE_COUNT - 1
        arrange count + 1
      else
        skin
      end
    when :loot
      loot
  end
end

def skin
  if Wield::left_noun != ""
    put "stow left"
  end
  put "skin"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|able to move/],
            :health_check => [/You are still stunned/],
            :loot => [/Skin what|cannot be skinned|Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      skin
    when :health_check
      check_health
      skin
    when :loot
      loot
  end
end

def loot
  if Wield::left_noun != ""
    put "stow left"
  end
  put "loot"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|Roundtime|able to move/],
            :health_check => [/You are still stunned/],
            :aim => [/could not find what|You search/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      loot
    when :health_check
      check_health
      loot
    when :aim
      Stats::reset_timer
      pause 0.5
      start
  end
end

# call start method
# to run script
start