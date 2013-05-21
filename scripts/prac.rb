# desc: practice climbing on climbable objects
# requirements: ?
# run: climbing available

@song_type = "dirge"

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
  pause 0.2
  put "play #{@song_type} on my bones"
  wait
  put "climb practice #{$args.join(" ")}"


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

end