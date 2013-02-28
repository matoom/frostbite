# desc: trains tactics
# requirements: 0
# run: hunting area

@hunt = true

def every_80s_do
  @hunt = true
end

def hunt
  put "hunt"
  match = { :wait => [/\.\.\.wait/],
            :no_hunt => [/unable to hunt/],
            :continue => [/Roundtime/] }

  case match_wait match
    when :no_hunt
      #@timer.stop
  end

  @hunt = false
end

while true
  if @hunt
    hunt
  end
  put "circle"
  wait_for_roundtime
  put "weave"
  wait_for_roundtime
end