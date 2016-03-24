require "defines"

class Armor
  @wearing_armor = false

  def self.wearing_armor?
    @wearing_armor = ITEMS::ARMOR.all? {|e| Inventory::list.any? {|w| w.include? e}}
    announce
    @wearing_armor
  end

  def self.announce
    unless @wearing_armor
      echo "**************************"
      echo "*** NOT Wearing armor! ***"
      echo "**************************"
    end
  end

end