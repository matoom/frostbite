unless Wield::right_noun.empty?
  put "stow right"
  wait
end

unless Wield::left_noun.empty?
  put "stow left"
  wait
end

move "go bridge"
move "n"
move "n"
move "n"
move "e"
move "e"
move "climb rope"
move "climb ladder"

put "climb creeper"
match = { :next => ["up and over"],
          :stand => [/SPLAT!/] }
case match_wait match
  when :stand
    put "stand"
    wait
    move "cli creeper"
end

move "e"
move "down"
move "climb trail"
#move "down"
move "w"
move "w"
move "s"
move "se"
move "se"
move "go gate"
move "sw"
move "s"
move "go gate"
move "sw"
move "s"
move "s"
move "s"
move "s"
move "s"
move "s"
move "w"
move "w"
move "w"
move "sw"
move "sw"
move "sw"
move "s"
move "s"
move "s"
move "se"
move "se"
move "s"
move "s"
move "se"
move "sw"
move "s"
move "s"
move "se"
move "se"
move "s"
move "sw"
move "sw"
move "s"
move "s"
move "s"

echo "*** ARRIVED AT NEG ***"