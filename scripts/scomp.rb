@color = "cyan"

def start
  put "look in my #{@color} pouch"
  match = { :special => ["black cube", "stoppered vial", "lumpy green runestone", "glass sphere"],
            :sell => ["you see", "There is nothing"],
            :wait => [/\.\.\.wait/] }
  res = match_wait match

  case res
    when :wait
      pause 0.4
      start
    when :special
      special_component
    when :sell
      put "get my #{@color} pouch"
      wait
      put "give my pouch to Ragge"
      wait
      put "put my pouch in my backpack"
  end
end

def special_component
  echo "*** Special component found! ***"
  exit
end

start

