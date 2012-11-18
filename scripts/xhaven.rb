# desc: travelling script from riverhaven to crossing
# requirements: 150+ in swimming
# run: east gate, riverhaven

if Room::title == "[Riverhaven, Stone Bridge]"
    execute "xfaldesu"
end

path = ["se", "s", "s", "s", "se", "s", "se", "e", "e", "e", "e", "e", "e", "e", "e",
        "e", "se", "s", "s", "go brid", "s", "se", "s", "sw", "sw", "s", "s", "go tunnel",
        "s", "s", "sw", "go flow", "sw", "sw", "s"]

path.each { |p|
  move p
}

labels_start

def go_wait(label_self, label)
  if label == :wait
    pause 0.5
    goto label_self
  else
    goto label
  end
end

label(:trail) {
  put "go trail"
  match = { :wait => ["...wait"],
            :canyon => [/\[North Road, Canyon\]/] }
  res = match_wait match

  go_wait(:trail, res)
}

label(:canyon) {
  put "e"
  match = { :wait => ["...wait"],
            :stand => ["You can't do that while lying down"],
            :continue => [/\[North Rim, Canyon\]/] }
  res = match_wait match

  if res == :stand
    goto :stand
  end

  go_wait(:canyon, res)
}

label(:stand) {
  put "stand"
  goto :canyon
}

label(:continue) {
}

labels_end


path = ["go trail", "se", "s", "s", "s", "se", "se", "se", "se", "s", "sw", "sw", "sw", "sw", "s", "s", "s", "se",
        "sw", "s", "s", "sw", "sw", "sw", "s", "s", "se", "se", "s", "sw", "s", "go bridge", "s", "s", "s", "s", "s",
        "s", "s", "se", "sw", "sw", "w", "w", "s", "sw", "s", "s", "w", "w", "s", "se", "se", "go gate", "sw", "s",
        "go gate", "sw", "s", "s", "s", "s", "s", "s", "w", "w", "w", "sw", "sw", "sw", "s", "s", "s", "se", "se",
        "s", "s", "se", "sw", "s", "s", "se", "se", "s", "sw", "sw", "s", "s", "s"]

path.each { |p|
  move p
}