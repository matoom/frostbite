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

put "take balaclava from vault"
wait
put "wear balaclava"
wait
put "take gloves from vault"
wait
put "wear gloves"
wait
put "take shield from vault"
wait
put "wear shield"
wait
put "take hauberk from vault"
wait
put "wear hauberk"
wait

put "close vault"
move "go door"
move "go arch"
move "out"
move "out"