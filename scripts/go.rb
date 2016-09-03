def run script
  if script.eql? "sandspit"
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

room = Map::current_room
echo room

moves = Map::path room[:zone], room[:id], $args.first.to_i
echo moves

moves.each do |m|
  if m.start_with? "script" 
    run m[7, m.length]
  else
    move m
  end
end