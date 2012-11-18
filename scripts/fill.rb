# desc: fills gem pouch by specified color
# requirements: -
# run: anywhere

put "take my #{$args.join(" ")} pouch from my backpack"
wait
put "fill my #{$args.join(" ")} pouch with my backpack"
wait
put "put my pouch in my backpack"