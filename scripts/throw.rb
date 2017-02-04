@arrange_count = 5
@weapon = $args.join(" ")

def finally_do
  pause_rt
  put "stow #{@weapon}"
end

def get_weapon
  put "get #{@weapon}"
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
  end
end

def lob target
  put "lob #{target}"
  match = { :wait => [/\.\.\.wait|entangled in a web/],
            :stunned => [/still stunned/],
            :skin => ["before collapsing", "deflate slightly", "stops all movement", "then grows still",
                      "ceases all movement", "collapses into a massive heap","massive heap before",
                      "sharp halt", "crumbles", "life force fades away"],
            :wait_for => ["trying to lob"],
            :continue => ["Roundtime"] }

  case match_wait match
    when :wait_for
      get_weapon
      echo "*** WAITING ***"
      wait_for(/advance on you|melee range/)
    when :skin
      get_weapon
      arrange 0
    when :continue
      get_weapon
  end
end

get_weapon

1000.times do
  lob @target
end
