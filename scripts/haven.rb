path = ["n", "n", "n", "ne", "ne", "n", "nw", "nw", "n", "n", "ne", "nw", "n", "n", "nw", "nw", "n", "n",
        "n", "ne", "ne", "ne", "e", "e", "e", "n", "n", "n", "n", "n", "n", "ne", "go gate", "n", "ne",
        "go gate", "nw", "nw", "n", "e", "e", "n", "n", "ne", "n", "e", "e", "ne", "ne", "nw", "n", "n",
        "n", "n", "n", "n", "go brid", "n", "n", "ne", "n", "nw", "nw", "n", "n", "ne", "ne", "ne",
        "n", "n", "ne", "nw", "n", "n", "n", "ne", "ne", "ne", "ne", "n", "nw", "nw", "nw", "nw", "n",
        "n", "n", "nw", "go trail", "w", "go trail", "n", "ne"]

path.each { |p|
  move p
}

echo "*** trail ***"

path = ["ne", "go flow", "ne", "n", "go tunnel", "n", "n", "n", "ne", "ne", "n", "nw", "n",
        "go brid", "n", "n", "nw", "w", "w", "w", "w", "w", "w", "w", "w", "w", "nw", "n",
        "nw", "n", "n", "n", "nw"]

path.each { |p|
  move p
}

echo "*** CROSSING FALDESU ***"
pause 2

execute "faldesu"

