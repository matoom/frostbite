put "climb rope"
match = { :wait_climb => [/You'll have to wait/],
          :climb => [/\[On a Rope Bridge, Above a Ravine\]/],
          :stow => [/You can't possibly manage to cross/] }

result = match_wait match

case result
  when :wait_climb
    put "hide"
    wait_for /finally arriving/
  when :stow
    put "stow left"
    wait
    put "stow right"
    wait
    put "climb"
    wait
end

move "shuffle south"
move "shuffle south"
move "shuffle south"

move "se"
move "sw"
move "se"
move "sw"
move "se"
move "go thicket"
move "s"
move "w"
move "w"
move "n"
move "go forest"
move "sw"
move "nw"
move "w"
move "nw"
move "w"

echo "*** Arrived at Rossman's east gate! ***"