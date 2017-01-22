# desc: practice athletics on climbable objects
# requirements: ?
# run: climbing available

@ordinal_numbers = %w(first second third fourth fifth sixth seventh eighth ninth tenth
                      eleventh twelfth thirteenth fourteenth fifteenth sixteenth)
@song_type = "rondo"
@obj = $args.join(" ")

def finally_do
  put "stop climb"
  wait
  put "stop play"
  wait
  put "put my bones in my shroud"
  wait
end

put "get bones"

100.times do
  wait
  put "play #{@song_type} on my bones"

  wait
  put "climb practice #{@ordinal_numbers[Room::count_objects(@obj)]} #{@obj}"

  6.times do
    pause 20
    if Exp::state("Athletics") > 32
      echo "*** Finished! ***"
      exit
    end
  end

  put "stop climb"
  wait
  put "stop play"
end