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
move "go path"

labels_start

label(:search1) {
  put "sea"
  wait_for_roundtime
  put "go trail"
  match = { :search1 => ["you were referring"],
            :search2 => ["trail closes in behind"] }
  match_wait_goto match
}

label(:search2) {
  put "sea"
  wait_for_roundtime
  put "go trail"

  match = { :search2 => ["you were referring"],
            :search3 => ["trail closes in behind"] }
  match_wait_goto match
}

label(:search3) {
  put "search"
  wait_for_roundtime
  put "go trail"
  match = { :search3 => ["you were referring"],
            :search4 => ["trail closes in behind"] }
  match_wait_goto match
}

label(:search4) {
  put "sea"
  wait_for_roundtime
  put "go spot"

  match = { :search4 => ["you were referring"],
            :next => [/\[Wilderness\]/] }
  match_wait_goto match
}

label(:next) {
}

labels_end

move "s"
move "e"
move "e"
move "e"
move "e"
move "go dead"
move "se"
move "s"
move "se"
move "sw"
move "sw"
move "sw"
move "w"
move "w"
move "w"
move "w"
move "s"
move "se"
put "hide"