class Armor
  @wearing_armor = false

  def self.check_armor
    put "inv armor"
    match = { :wait => [/\.\.\.wait/],
              :armor => [/INVENTORY HELP/],
              :continue => [/aren't wearing anything like/] }
    result = match_wait match

    case result
      when :wait
        pause 0.5
        check_armor
      when :armor
        @wearing_armor = true
      else
        false
    end
  end

  def self.wearing_armor?
    check_armor
    announce
  end

  def self.announce
    unless @wearing_armor
      echo "**************************"
      echo "*** NOT Wearing armor! ***"
      echo "**************************"
    end
  end

end