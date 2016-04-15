require "defines"
require "helper"

# desc: collects piles of specified foraging items and cleans up after
# requirements: ??
# run: valid foraging spot

Observer.instance.register_event({ :perc => "Roundtime" })

def perc
  echo "exp perc: #{Exp::state "perc"}"
end

if $args.empty?
  echo "Specify collectable item!"
  exit!
end

def finally_do
  if Room::count_objects("pile") > 0
    sleep Rt::value
    put_wait "kick pile", /You take|could not find/
  end
end

100.times do
  break if SYSTEM::finished
  put "collect " + $args.join(" ")
  wait_for_roundtime
  put "kick pile"
end

