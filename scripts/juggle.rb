if $args.empty?
  echo "Item name missing!"
  exit!
end

def finally_do
  sleep get_match_rt
  put "put my #{$args.first} in my backpack"
end

10.times do
  put "juggle my " + $args.first
  wait_for_roundtime
end

