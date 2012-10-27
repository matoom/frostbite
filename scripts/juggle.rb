if $args.empty?
  echo "Item name missing!"
  exit!
end

def finally_do
  put "put my #{$args.first} in my backpack"
end

10.times do
  put "juggle my " + $args.join(" ")
  wait_for_roundtime
end

