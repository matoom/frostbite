def steal item
  put "steal #{item}"
  match = { :wait => [/\.\.\.wait|appears different about/],
            :fail => [/Guards!|begins to shout/],
            :leave => [/You haven't picked|You can't steal/],
            :stow => [/You need at least one/],
            :continue => [/Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      steal item
    when :stow
      put "stow left"
      wait
      put "stow right"
      wait
      steal item
    when :fail
      move "out"
      move "e"
      move "se"
      echo "*** Failed to get #{item} ***"
      Kernel::exit
  end

end

move "nw"
move "w"
move "go shop"

steal "chamomile"

move "out"
move "e"
move "se"
move "sw"

move "go gate"
move "n"
put "go stair"
wait_for /^\[.*?\]$/
put "up"
wait_for /^\[.*?\]$/
put "up"
wait_for /^\[.*?\]$/
move "go door"
move "go wid"
move "go wid"
put "go stair"
wait_for /^\[.*?\]$/
move "go clock"
move "go arch"

put "get chamomile"
wait
put "put my chamomile on marble altar"
wait

put "pray"
wait
put "pray"
wait
put "pray"

wait_for "a glowing Phelim orb rests in its place upon the altar."

put "get orb from marble altar"
wait
put "stow orb"
wait
put "stand"
wait

move "go arch"
move "go wid"
move "go stair"
wait_for /^\[.*?\]$/
move "go clock"
move "go clock"
move "go door"
put "down"
wait_for /^\[.*?\]$/
put "down"
wait_for /^\[.*?\]$/
put "down"
wait_for /^\[.*?\]$/
move "s"
move "go gate"
move "ne"