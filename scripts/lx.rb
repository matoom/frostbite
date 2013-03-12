# desc: trains brawling, appraisal and crossbow
# requirements: arm worn shield, set up stow containers
# run: hunting area

@arrange_count = 5
@ammo = "quadrello"

def check_health
  if Vitals::health < 50
    put "quit"
  else
    pause 5
  end
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
      pause Rt::value
      circle 0
    when :wait_arrive
      echo "*** WAITING ***"
      wait_for(/advance on you|melee range/)
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
      if count < 1
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
            :health_check => [/You are still stunned/],
            :load => [/Roundtime/] }
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
      if count < @arrange_count - 1
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
      pause 0.5
      start
  end
end

# call start method
# to run script
start