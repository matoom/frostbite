require "settings"
require "defines"

def hunt
  if Settings::get.hunt and Exp::state("perc") < 34 and Time.now - Settings::get.last_hunt > 75
    echo "Last hunt - #{Time.now - Settings.get.last_hunt} seconds ago."
    put "hunt"
    match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead|stunned/],
              :continue => [/Roundtime|unable to hunt/] }
    case match_wait match
      when :wait
        pause 0.5
        hunt
      else
        Settings::get.last_hunt = Time.now
    end
  end
end