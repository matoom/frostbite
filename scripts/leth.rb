# desc: travelling script from crossing bank to leth town center
# requirements: !only available for thieves guild members at appropriate circle!, 30? ranks of swimming
# run: in front of crossing bank

path = ["sw", "go bridge", "n", "n", "go ware", "s"]

path.each { |p|
  move p
}

put "open trap"

path = ["go trap", "go river", "w", "n", "go panel", "climb step"]

path.each { |p|
  move p
}

path = ["s", "s", "sw", "sw", "down", "s", "sw", "sw",
        "s", "up", "sw", "w", "sw", "climb ladder", "go  gap"]

path.each { |p|
  move p
}

path = ["s", "sw", "sw", "sw", "sw", "s", "se", "se", "s", "s", "sw", "sw",
        "sw", "s", "se", "sw", "s", "sw", "s", "s", "se", "se", "sw"]

path.each { |p|
  move p
}

labels_start

label(:go) {
  put "sw"
  match = { :noweb => ["Thick trees line the route here"],
            :web => ["Roundtime", "You can't do that while", "\.\.\.wait"] }
  match_wait_goto match
}

label(:web) {
  match = { :go => ["Using your escape", "The webs break apart and fall away"] }
  match_wait_goto match
}

label(:noweb) {
}

labels_end

path = ["sw", "sw", "s", "sw", "se", "se", "s", "s", "se", "se", "s",
        "s", "se", "go bower gate", "se", "se", "se", "se", "se", "se", "se"]

path.each { |p|
  move p
}