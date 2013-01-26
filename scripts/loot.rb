# desc: loots all gems, coins and boxes in any area
# requirements: stow container set up
# run: anywhere

100.times do
  put "loot coin"
  match = { :wait => [/\.\.\.wait|you may only type ahead 1 command/],
            :loot_next => ["You pick up"],
            :loot_gems => ["I could not find what"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
    when :loot_gems
      break
  end
end

100.times do
  put "stow gem"
  match = { :wait => [/\.\.\.wait|you may only type ahead 1 command/],
            :loot_next => ["You pick up"],
            :loot_boxes => ["Stow what?"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
    when :loot_boxes
      break
  end
end

100.times do
  put "stow box"
  match = { :wait => [/\.\.\.wait|you may only type ahead 1 command/],
            :loot_next => ["You pick up"],
            :end => ["Stow what?"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
    when :end
      break
  end
end

100.times do
  put "stow bar"
  match = { :wait => [/\.\.\.wait|you may only type ahead 1 command/],
            :loot_next => ["You pick up"],
            :end => ["Stow what?"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
    when :end
      break
  end
end

100.times do
  put "stow nugget"
  match = { :wait => [/\.\.\.wait|you may only type ahead 1 command/],
            :loot_next => ["You pick up"],
            :end => ["Stow what?"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
    when :end
      break
  end
end
