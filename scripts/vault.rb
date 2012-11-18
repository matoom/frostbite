# desc: enter crossing vault
# requirements: need to rent or transfer a vault in crossing
# run: at the square, nw from bank

move "go square"
move "go door"

put "go rosewood arch"
match = { :second => ["The attendant steps in"],
          :continue => ["The Dwarven attendant escorts"] }
result = match_wait match

case result
  when :second
    move "go ironwood arch"
end

put "pull lev"
move "go door"
put "open vault"
