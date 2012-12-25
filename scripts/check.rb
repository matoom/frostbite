move "nw"
move "w"
move "w"
move "w"
move "w"
move "w"
move "n"
move "n"
move "go door"

put "mark ring"
match = { :back => ["Roundtime"],
          :alert => ["stolen in this store recently"] }
result = match_wait match

case result
  when :alert
    echo ""
    echo "***********************"
    echo "*** RECENTLY STOLEN ***"
    echo "***********************"
    echo ""
end

move "out"
move "s"
move "s"
move "e"
move "e"
move "e"
move "e"
move "e"
move "se"


