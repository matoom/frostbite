# desc: practice climbing on climbable objects
# requirements: ?
# run: climbing available

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
  put "play battle on my bones"
  wait
  put "climb practice #{$args.join(" ")}"

  pause 120

  put "stop climb"
  wait
  put "stop play"
  wait

  if Exp::state("climb") > 32
    echo "*** Finished! ***"
    exit
  end
end