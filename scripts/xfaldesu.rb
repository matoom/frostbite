@match_rt_adjustment = -1

label_start

label(:start) {
  move "dive river"
  goto :south_1
}

label(:south_1) {
  put "south"

  match = { :wait => ["...wait"],
            :midsection => ["You can't swim in that direction"],
            :south_1 => ["You"]}
  result = match_wait match

  if result == :wait
    pause 0.5
    goto :south_1
  elsif result == :midsection
    goto :midsection
  else
    goto :south_1
  end
}

label(:midsection) {
  put "sw"
  match = { :wait => ["...wait"],
            :north_2 => ["You"] }
  result = match_wait match

  if result == :wait
    pause 0.5
    goto :midsection
  else
    goto :south_2
  end
}

label(:south_2) {
  put "south"

  match = { :wait => ["...wait"],
            :end => ["You can't swim in that direction"],
            :south_2 => ["You"]}
  result = match_wait match

  if result == :wait
    pause 0.5
    goto :south_2
  elsif result == :end
    goto :end
  else
    goto :south_2
  end
}

label(:end) {
  move "climb bridge"
}

label_end

@match_rt_adjustment = 0