# desc: goes across faldesu river from north to south side
# requirements: 150+ swimming
# run: north side of faldesu

@rt_adjust = 0

labels_start

label(:start) {
  move "dive river"
  goto :south_1
}

label(:south_1) {
  put "south"

  match = { :wait => [/\.\.\.wait/],
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
  match = { :wait => [/\.\.\.wait/],
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

  match = { :wait => [/\.\.\.wait/],
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

labels_end

@match_rt_adjustment = 0