require "settings"

echo "Last hunt was #{Time.now - Settings.get.last_hunt} seconds ago."

def hunt
  if Time.now - Settings::get.last_hunt > 120
    put "hunt"
    match = { :wait => [/\.\.\.wait|while entangled in a web|you may only type ahead/],
              :continue => [/Roundtime|unable to hunt/] }
    case match_wait match
      when :wait
        hunt
    end
    Settings::get.last_hunt = Time.now
  end
end