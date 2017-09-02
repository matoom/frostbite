def script name
  if name.eql? "sandspit"
    put "go barrel"
    match = { :ok => [/You duck quietly/],
              :second => [/can't do that/] }
    result = match_wait match
    case result
      when :second
        move "go second barrel"
    end
  end
end

def search_and_move m
  put "search"
  match = { :go => ["You search"],
            :wait => [/\.\.\.wait/] }
  result = match_wait match
  case result
    when :wait
      pause 0.5
      search_and_move m
    when :go
      move m
  end
end

def map_action e
  if e =~ /^(west|south|north|east|go|out|climb|down|up)/i
    move e
  else
    put e
    case match_wait({ :ok => [/Roundtime/, />/], :wait => [/\.\.\.wait/] })
      when :wait
        pause 0.5
        map_event m
    end
  end
end

room = Map::current_room
echo room

moves = Map::path room[:zone], room[:id], $args.first.to_i
echo moves

moves.each do |move|
  move.split(';').each do |m|
      if m.start_with? "script "
        script m[7, m.length]
      elsif m.start_with? "search "
        search_and_move m[7, m.length]
      elsif m.start_with? "rt "
        map_action m[3, m.length]
      else
        map_action m
      end
  end
end