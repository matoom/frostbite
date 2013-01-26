@tiers = ["big bronze valve that seems oversized", "an obscure opening beside", "a narrow crevice beside", "a large pipe, just big"]
@destination = $args.first

room = Room::description

def from2to1
  move "go open"
  move "down"
  move "e"
  move "se"
  move "ne"
  move "go pass"
end

def from3to1
  move "go crevice"
  move "e"
  move "se"
  move "down"
  move "s"
  move "down"
  move "e"
  move "se"
  move "ne"
  move "go pass"
end

def from4to1
  move "go pipe"
  move "s"
  move "climb rung"
  move "s"
  move "s"
  move "se"
  move "e"
  move "down"
  move "se"
  move "ne"
  move "ne"
  move "se"
  move "down"
  move "e"
  move "se"
  move "ne"
  move "go pass"
end

def from1to2
  move "go pass"
  move "sw"
  move "nw"
  move "w"
  move "up"
  move "go open"
end

def from3to2
  move "go crev"
  move "e"
  move "se"
  move "down"
  move "s"
  move "go open"
end

def from4to2
  move "go pipe"
  move "s"
  move "climb rung"
  move "s"
  move "s"
  move "se"
  move "e"
  move "down"
  move "se"
  move "ne"
  move "ne"
  move "se"
  move "go open"
end

def from1to3
  move "go pass"
  move "sw"
  move "nw"
  move "w"
  move "up"
  move "n"
  move "up"
  move "nw"
  move "w"
  move "go crev"
end

def from2to3
  move "go open"
  move "n"
  move "up"
  move "nw"
  move "w"
  move "go crev"
end

def from4to3
  move "go pipe"
  move "s"
  move "climb rung"
  move "s"
  move "s"
  move "se"
  move "e"
  move "down"
  move "se"
  move "ne"
  move "ne"
  move "se"
  move "n"
  move "up"
  move "nw"
  move "w"
  move "go crev"
end

def from1to4
  move "go pass"
  move "sw"
  move "nw"
  move "w"
  move "up"
  move "w"
  move "nw"
  move "nw"
  move "s"
  move "sw"
  move "up"
  move "w"
  move "nw"
  move "n"
  move "n"
  move "climb ladder"
  move "n"
  move "n"
end

def from2to4
  move "go open"
  move "w"
  move "nw"
  move "nw"
  move "s"
  move "sw"
  move "up"
  move "w"
  move "nw"
  move "n"
  move "n"
  move "climb ladder"
  move "n"
  move "n"
end

def from3to4
  move "go crev"
  move "e"
  move "se"
  move "down"
  move "s"
  move "w"
  move "nw"
  move "nw"
  move "s"
  move "sw"
  move "up"
  move "w"
  move "nw"
  move "n"
  move "n"
  move "climb ladder"
  move "n"
  move "n"
end

def go tier, dest
  send("from#{tier}to#{dest}");
end

@tiers.each_with_index { |tier, i|
  if room.include?(tier)
    go i + 1, @destination
    break
  end
}
