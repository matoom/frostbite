@arrange_count = 5

def check_health
  if Vitals::health < 50
    echo "*** LOW HP! ***"
  else
    pause 5
  end
end

def arrange count
  put "arrange"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|still stunned/],
            :quit => [/You are still stunned/],
            :arrange => [/You begin to arrange|You continue arranging|complete arranging|You make a mistake/],
            :loot => [/arrange what|cannot be skinned|corpse is worthless now/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      arrange count
    when :quit
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
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|still stunned/],
            :quit => [/You are still stunned/],
            :loot => [/Skin what|cannot be skinned|Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      skin
    when :quit
      check_health
      skin
    when :loot
      loot
  end
end

def loot
  put "loot"
  match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|Roundtime|still stunned/],
            :quit => [/You are still stunned/],
            :continue => [/could not find what|You search/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      loot
    when :quit
      check_health
      loot
  end
end

10000.times do
  put "claw"
  match = { :wait => [/\.\.\.wait|entangled in a web/],
            :stunned => [/still stunned/],
            :skin => ["before collapsing", "deflate slightly", "stops all movement", "then grows still",
                      "ceases all movement", "collapses into a massive heap","massive heap before",
                      "sharp halt", "crumbles", "life force fades away"],
            :wait_for => ["trying to attack"],
            :adv => ["You aren't close enough to attack."],
            :continue => ["Roundtime"] }
  result = match_wait match

  case result
    when :wait
      pause 0.4
    when :skin
      arrange 0
    when :wait_for
      echo "*** WAITING ***"
      wait_for(/begins to advance on you|closes to melee range/)
    when :adv
      put "advance"
      pause 2
    when :stunned
      check_health
  end
end