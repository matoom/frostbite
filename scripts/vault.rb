# desc: enter crossing vault
# requirements: need to rent or transfer a vault in crossing
# run: at the square, nw from bank

doors = ["rosewood arch", "ironwood arch", "blackwood arch"]

def go door
    put "go #{door}"
    match = { :occupied => ["The attendant steps in"],
              :continue => ["The Dwarven attendant escorts"] }
    match_wait match
end

def enter doors
  doors.each do | door |
    return if go(door).equal?(:continue)
  end
  pause 10
  enter doors
end

move "go square"
move "go door"

enter doors

put "pull lev"
move "go door"
put "open vault"