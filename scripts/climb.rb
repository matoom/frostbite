# desc: climing script for crossing
# requirements: 300?? climbing, 60?? stamina
# run: ne gate, crossing

path = ["go gate", "w", "w", "s", "s", "w", "w", "s", "w", "w", "w", "w", "w", "w",
        "go bridge", "w", "w", "w", "w", "go gate", "climb wall", "s", "climb embrasure",
        "climb wall", "n", "e", "climb break", "climb wall", "climb embrasure", "climb wall",
        "w", "climb embrasure", "go gate", "e", "e", "e", "e", "e", "e", "e", "e", "s", "s",
        "s", "s", "s", "e", "e", "se", "e", "e", "e", "e", "go gate", "climb wall", "n",
        "climb break", "climb wall", "climb embrasure", "climb wall", "s", "climb embrasure",
        "go bushes", "n", "ne", "n", "n", "up", "n", "n"]

100.times do
  exp = Exp::state "athletics"

  echo "*** #{exp} ***"

  #if exp > 32
  #  echo "*** DONE ***"
  #  exit
  #end

  pause 2

  path.each do |p|
      move p
  end
end