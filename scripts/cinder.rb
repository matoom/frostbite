# desc: from tree in swamp to cinder beasts
# requirements: ?
# run: tree in haven swamp

def go(dir)
  put dir
  match = { :wait => [/\.\.\.wait|you may only type ahead/],
            :retreat => [/You'll have better luck if you first retreat|You are engaged/],
            :continue => [/Roundtime/] }
  res = match_wait match

  case res
    when :wait
      pause 0.5
      go dir
    when :retreat
      put "retreat"
      put "retreat"
      go dir
  end
end

move "e"
move "down"

go "se"
go "s"
go "se"
go "sw"
go "e"
go "ne"
go "n"
go "sw"
go "n"

echo "*** ARRIVED AT ALTAR ***"
put "touch altar"
wait
put "pray"
wait_for(/\[Gargantuan Skeletal Claw\]/)
pause 10
put "stand"
wait_for(/\[Zaulfung, Underwater\]/)

go "se"
move "go crack"
move "up"
go "n"

go "nw"
go "ne"
go "nw"
go "n"
go "nw"
go "n"
go "s"

echo "*** ARRIVED ***"
