Observer.instance.register_event({ :exit => ["already under the effects", "you think that you've grasped"] })

def exit
  pause_for_roundtime
  Kernel::exit
end

subject = $args.join(" ")

case subject
  when /thievery|thiev/
    put "drop 1 copper kronar"
    put "appraise focus coin"
    wait_for_roundtime
  when /crossbow|cross|xbow/
    put "appraise focus crossbow"
    wait_for_roundtime
  when /perception|perc/
    put "appraise focus north"
    wait_for_roundtime
  when /magic/
    put "appraise focus khri"
    wait_for_roundtime
  when /shield/
    put "appraise focus shield"
    wait_for_roundtime
  when /debilitation|debil/
    put "appraise focus guile"
    wait_for_roundtime
  when /utility|util/
    put "appraise focus sagacity"
    wait_for_roundtime
  else
    Kernel::exit
end

pause 1

$args.clear << "rock"

load "col.rb"
