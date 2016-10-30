# desc: enter crossing vault
# requirements: need to rent a vault in crossing
# run: at the vault doors

$args << "carousel"

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
  wait_for /came through .* arch/
  enter doors
end

if Room::title.eql? "[The Crossing, Alamhif Trace]"
  move "go square"
  move "go door"
else
  load "dir.rb"
end

enter doors

put "pull lev"
move "go door"
put "open vault"
wait