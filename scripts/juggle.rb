# desc: juggles 10 times
# requirements: -
# run: anywhere

if $args.empty?
  echo "Item name missing!"
  exit!
end

def finally_do
  pause_rt
  put "put my #{$args.first} in my backpack"
end

100.times do
  put "juggle my " + $args.join(" ")
  wait_for_roundtime
end

