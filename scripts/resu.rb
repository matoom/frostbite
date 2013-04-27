#Zoluren - From NE Gate to Snaer Hafwa

def check_armor
  put "inv armor"
  match = { :wait => [/\.\.\.wait/],
            :exit => [/INVENTORY HELP/],
            :continue => [/aren't wearing anything like/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      prepare_armor
    when :exit
      echo "*** Wearing armor! ***"
      exit
  end
end

check_armor

move "n"
move "n"
move "n"
move "ne"
move "ne"
move "n"
move "nw"
move "nw"
move "n"
move "n"
move "ne"
move "nw"
move "n"
move "n"
move "nw"
move "nw"
move "n"
move "n"
move "n"
move "ne"
move "ne"
move "ne"
move "e"
move "e"
move "e"
move "n"
move "n"
move "n"
move "n"
move "n"
move "n"
move "ne"
move "go gate"
move "n"
move "ne"
move "go gate"
move "nw"
move "nw"
move "n"
move "e"
move "e"
move "n"
move "n"
move "ne"
move "n"
move "e"
move "e"
move "ne"
move "ne"
move "nw"
move "n"
move "n"
move "n"
move "n"
move "n"
move "n"
move "go brid"
move "n"
move "n"
move "ne"
move "n"
move "nw"
move "nw"
move "n"
move "n"

move "go copse"
move "e"
move "sw"
move "sw"
move "e"
move "e"
move "go rope bridge"
move "n"
move "go stair"
move "go arch"
move "go door"
put "turn sco"
move "go iron door"

move "go hatch"
move "down"
move "down"
put "down"
wait
put "hide"