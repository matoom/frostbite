# desc: rossman's landing east gate to sky giants
# requirements: -
# run: rossman's landing east gate

move "e"
move "se"
move "e"
move "se"
move "ne"
move "go thicket"
move "e"
move "sw"
move "s"
move "ne"
move "go gap"
move "nw"
move "ne"
move "nw"
move "ne"
move "nw"

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

move "shuffle north"
move "shuffle north"
move "shuffle north"
put "search ravine"

echo "*** Arrived at Giants ravine! ***"