require "defines"
require "armor"

echo Armor::wearing_armor? ? "Armor is equipped." : ""
echo "#{Container::list.each.count { |item| LOCKSMITH::BOX_TYPES.any? { |box| item.include? box }}} boxes found in store container."