path = ["nw", "nw", "nw", "nw",  "nw", "nw", "nw", "go bower gate", "nw", "n", "n",
        "nw", "nw", "n", "n", "nw", "nw", "ne", "n", "ne", "ne", "ne"]

path.each { |p|
  move p
}

labels_start

label(:go) {
  put "ne"
  match = { :noweb => ["Along the north, the trees and shrubs"],
            :web => ["Roundtime", "You can't do that while", /\.\.\.wait/] }
  match_wait_goto match
}

label(:web) {
  match = { :go => ["Using your escape", "The webs break apart and fall away"] }
  match_wait_goto match
}

label(:noweb) {
}

labels_end

path = ["nw", "nw", "n", "n", "ne", "n", "ne", "nw", "n", "ne", "ne",
        "ne", "n", "n", "nw", "nw", "n", "ne", "ne", "ne", "ne", "n",
        "go boulder", "go root", "ne", "e", "ne", "down"]

path.each { |p|
  move p
}

path = ["n", "ne", "ne", "n", "up", "ne", "ne", "north", "north"]

path.each { |p|
  move p
}

path = ["climb step", "out", "s", "e", "go ware", "up", "n", "out", "s",
        "s", "go bridge", "ne"]

path.each { |p|
  move p
}