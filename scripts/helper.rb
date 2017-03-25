def pause_wait
  pause 0.5 + Rt::value
end

def put_wait action, success
  put action
  case match_wait({:pause => [/You are still stunned/],
                   :wait => [/\.\.\.wait|you may only type ahead/],
                   :continue => [success] })
    when :wait
      pause_wait
      put_wait action, success
    when :pause
      pause 3
      put_wait action, success
  end
end