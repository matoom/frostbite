# desc: collects piles of specified foraging items and cleans up after
# requirements: ??
# run: valid foraging spot

Observer.instance.register_event({ :test => "The Crossing, Haven's End" })
Observer.instance.register_event({ :test2 => "Roundtime" })

def test
  echo "first event"
  #wait_for_roundtime
  #put "hunt"
  #wait_for_roundtime

  #Observer.instance.terminate
end

def test2
  echo "second event"
end

if $args.empty?
  echo "Specify collectable item!"
  exit!
end

def finally_do
  pause 0.5
  put "kick pile"
end

100.times do
  put "collect " + $args.join(" ")
  wait_for_roundtime
  put "kick pile"
end