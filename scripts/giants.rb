# desc: rossman's landing east gate to sky giants
# requirements: -
# run: rossman's landing east gate

def climb
  put "climb rope"
  match = { :wait_climb => [/You'll have to wait/],
            :climb => [/\[On a Rope Bridge, Above a Ravine\]/],
            :stow => [/You can't possibly manage to cross/] }

  case match_wait match
    when :wait_climb
      put "hide"
      wait_for(/finally|on the other side/)
      climb
    when :stow
      put "stow left"
      wait
      put "stow right"
      wait
      climb
  end
end

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

climb

move "shuffle north"
move "shuffle north"
move "shuffle north"
put "search ravine"

echo "*** Arrived at Giants ravine! ***"