def finally_do
  put "put my #{$args.first} in my backpack"
end

for i in 0..10
  put "juggle my " + $args.first
  wait_for_roundtime
end

