@is_armor = false

def show_warning
  echo "*** NOT Wearing armor! ***"
end

def check_armor
  put "inv armor"
  match = { :wait => [/\.\.\.wait/],
            :armor => [/INVENTORY HELP/],
            :continue => [/aren't wearing anything like/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      check_armor
    when :continue
      show_warning
    when :armor
      @is_armor	= true
  end
end

check_armor

move "w"
move "nw"
move "n"
move "nw"
move "w"
move "go path"
move "s"
move "w"
move "sw"
move "w"
move "w"
move "w"
move "nw"
move "n"
move "w"
move "nw"
move "cli cliff"
move "up"
move "up"
move "up"
move "go bridge"
move "nw"
move "e"
move "climb cliff"

if !@is_armor
  show_warning
end