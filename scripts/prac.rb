# desc: practice athletics on climbable objects
# requirements: ?
# run: climbing available

@ordinal_numbers = %w(first second third fourth fifth sixth seventh eighth ninth tenth eleventh twelfth)
@song_type = "aria"
@obj = $args.join(" ")

def finally_do
  put "stop climb"
  pause 1
  put "stop play"
  pause 1
end

100.times do
  put "play #{@song_type} on my zills"
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