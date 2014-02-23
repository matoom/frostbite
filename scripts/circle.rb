# desc: trains tactics
# requirements: 0
# run: hunting area

require "hunt"

while true
  hunt
  put "circle"
  wait_for_roundtime
  put "weave"
  wait_for_roundtime
end