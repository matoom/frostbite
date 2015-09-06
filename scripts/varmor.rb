move "go square"
move "go door"

labels_start

label(:start) {
  put "go rosewood arch"
  match = { :second => [/The attendant steps in/],
            :continue => [/The Dwarven attendant escorts/] }
  match_wait_goto match
}

label(:second) {
  move "go ironwood arch"
}

label(:continue) {
}

labels_end

put "pull lev"
wait
move "go door"
put "open vault"

put "remove balaclava"
wait
put "put my balaclava in vault"
wait
put "remove gloves"
wait
put "put my gloves in vault"
wait
put "remove shield"
wait
put "put my shield in vault"
wait
put "remove hauberk"
wait
put "put my hauberk in vault"
wait

put "close vault"
move "go door"
move "go arch"
move "out"
move "out"