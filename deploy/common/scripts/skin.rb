require "defines"

def arrange count
  put "arrange"
  match = { :wait => [/\.\.\.wait|you may only type ahead/],
            :stun => [/You are still stunned|while entangled in a web/],
            :arrange => [/You begin to arrange|You continue arranging|complete arranging|You make a mistake/],
            :loot => [/arrange what|cannot be skinned|corpse is worthless now/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      arrange count
    when :stun
      pause 5
      arrange count
    when :arrange
      if count > 1
        arrange count - 1
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
  match = { :wait => [/\.\.\.wait|you may only type ahead/],
            :pause => [/You are still stunned|while entangled in a web/],
            :loot => [/Skin what|cannot be skinned|Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      skin
    when :pause
      pause 5
      skin
    when :loot
      loot
  end
end

def loot
  put "loot"
  match = { :wait => [/\.\.\.wait|you may only type ahead|Roundtime/],
            :pause => [/You are still stunned|while entangled in a web/],
            :continue => [/could not find what|You search/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      loot
    when :pause
      pause 5
      loot
  end
end

if GLOBAL::arrange_count > 0
  arrange GLOBAL::arrange_count
elsif $args.first =~ /all/
  arrange 5
elsif GLOBAL::skin
  skin
else
  loot
end