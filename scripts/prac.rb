# desc: practice athletics on climbable objects
# requirements: ?
# run: climbing available

@ordinal_numbers = %w(first second third fourth fifth sixth seventh eighth ninth tenth eleventh twelfth)
@song_type = "rondo"
@obj = $args.join(" ")

def finally_do
  put "stop climb"
  wait
  pause 1
  put "stop play"
  wait
  put "put my bones in my shroud"
  wait
end

put "get bones"
pause 1

100.times do
  put "play #{@song_type} on my bones"
  wait
  put "climb practice #{@ordinal_numbers[Room::count_objects(@obj)]} #{@obj}"
  wait

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
  wait
  pause 1
end