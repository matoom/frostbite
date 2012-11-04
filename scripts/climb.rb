path = ["go gate", "west", "west", "south", "south", "west", "west", "south", "west", "west", "west", "west", "west", "west",
        "go bridge", "west", "west", "west", "west", "go gate", "climb wall", "south", "climb embrasure", "climb wall", "north",
        "east", "climb break", "climb wall", "climb embrasure", "climb wall", "west", "climb embrasure", "go gate", "east",
        "east", "east", "east", "east", "east", "east", "east", "south", "south", "south", "south", "south", "east", "east", "southeast",
        "east", "east", "east", "east", "go gate", "climb wall", "north", "climb break", "climb wall", "climb embrasure", "climb wall",
        "south", "climb embrasure", "go bushes", "north", "northeast", "north", "north", "up", "north", "north"]

100.times do
  exp = Exp::state "climbing"

  echo "*** #{exp} ***"

  if exp > 32
    echo "*** DONE ***"
    exit
  end

  path.each do |p|
      move p
  end
end