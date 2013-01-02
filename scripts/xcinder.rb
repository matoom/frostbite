# back from cinder beasts

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

move "n"

go "sw"
go "se"
go "s"
go "s"
go "s"
go "s"
go "sw"
move "go tunnel"
move "go crack"
move "nw"
move "up"
move "up"
move "up"

go "s"
go "ne"
go "s"
go "sw"
go "w"
go "ne"
go "nw"
go "n"
go "nw"

move "climb tree"
move "w"

echo "*** ARRIVED ***"
