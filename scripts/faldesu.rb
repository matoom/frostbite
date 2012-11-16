@match_rt_adjustment = -1

label(:start) {
  move "dive river"
  goto :north_1
}

label(:north_1) {
  put "north"

  match = { :wait => ["...wait"],
            :midsection => ["You can't swim in that direction"],
            :north_1 => ["You"]}
  result = match_wait match

  if result == :wait
    pause 0.5
    goto :north_1
  elsif result == :midsection
    goto :midsection
  else
    goto :north_1
  end
}

label(:midsection) {
  put "nw"
  match = { :wait => ["...wait"],
            :north_2 => ["You"] }
  result = match_wait match

  if result == :wait
    pause 0.5
    goto :midsection
  else
    goto :north_2
  end
}

label(:north_2) {
  put "north"

  match = { :wait => ["...wait"],
            :end => ["You can't swim in that direction"],
            :north_2 => ["You"]}
  result = match_wait match

  if result == :wait
    pause 0.5
    goto :north_2
  elsif result == :end
    goto :end
  else
    goto :north_2
  end
}

label(:end) {
  move "climb bridge"
}

@match_rt_adjustment = 0