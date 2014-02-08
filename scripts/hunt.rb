require "settings"

echo "Last hunt was #{Time.now - Settings.get.last_hunt} seconds ago."

if Time.now - Settings::get.last_hunt > 120
  put "hunt"
  Settings::get.last_hunt = Time.now
end


