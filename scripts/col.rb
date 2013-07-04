# desc: collects piles of specified foraging items and cleans up after
# requirements: ??
# run: valid foraging spot

Observer.instance.register_event({ :test => "Roundtime" })

def test
  echo Exp::state "perc"
end

if $args.empty?
  echo "Specify collectable item!"
  exit!
end

def finally_do
  pause_for_roundtime
  put "kick pile"
end

100.times do
  put "collect " + $args.join(" ")
  wait_for_roundtime
  put "kick pile"
end