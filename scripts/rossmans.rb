# desc: haven east gate to rossman's landing east gate
# requirements: swimming 150+
# run: haven east gate

move "go gate"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "w"
move "go gate"
move "nw"
move "n"
move "n"
move "n"
move "nw"
move "ne"
move "n"
move "ne"
move "nw"
move "n"
move "ne"
move "w"
move "nw"
move "n"
move "ne"
move "n"
move "nw"
move "ne"
put "search out"
move "climb hand"
move "climb wall"
move "n"
put "search ledge"
move "climb shrub"

put "peer path"
match = { :northwest => ["northwest"],
          :northeast => ["northeast"],
          :southwest => ["southwest"],
          :southeast => ["southeast"],
          :north => ["north"],
          :south => ["south"],
          :east => ["east"],
          :west =>["west"] }
result = match_wait match

move "down"

case result
  when :northwest
    move "nw"
  when :northeast
    move "ne"
  when :southwest
    move "sw"
  when :southeast
    move "se"
  when :north
    move "n"
  when :south
    move "s"
  when :east
    move "e"
  when :west
    move "w"
end

put "nw"
wait_for "Birds chitter in the branches"

move "n"
move "nw"
move "n"
move "ne"
move "go bank"
pause 0.5
move "e"
move "ne"
move "ne"
move "e"

move "go ramp"
move "go path"
move "nw"
move "n"
move "nw"
move "w"
move "nw"
move "w"

echo "*** Arrived at Rossman's east gate! ***"