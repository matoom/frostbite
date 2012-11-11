require "#{File.dirname(__FILE__)}/data.rb"

class Rt
  # Roundtime
  #
  # @param
  # @return [int] roundtime value
  def self.value
    GameData.getRt().to_i
  end
end

class Inventory
  # Inventory items
  #
  # @param
  # @return [Array] inventory item list
  def self.list
    GameData.getInventory().to_s.split(", ")
  end
end

class Container
  # Container items
  #
  # @param
  # @return [Array] container item list
  def self.list
    GameData.getContainer().to_s.split(", ")
  end
end

class Wield
  # Wield right
  #
  # @param
  # @return [String]
  def self.right()
    GameData.getWieldRight().to_s
  end

  # Wield right noun
  #
  # @param
  # @return [String]
  # @example Using wield right noun in script.
  #   put "put my #{Wield::right_noun} in my backpack"
  def self.right_noun()
    GameData.getWieldRightNoun().to_s
  end

  # Wield left
  #
  # @param
  # @return [String]
  def self.left()
    GameData.getWieldLeft().to_s
  end

  # Wield left noun
  #
  # @param
  # @return [String]
  def self.left_noun()
    GameData.getWieldLeftNoun().to_s
  end
end

class Exp
  # Exp rank
  #
  # @param
  # @return [int] rank of skill
  def self.rank(exp_string)
    index = GameData.getExpIndex(exp_string.to_s)
    GameData.getExpRank(index).to_i
  end

  # Exp state
  #
  # @param
  # @return [int] learning state */34
  # @example Using exp state in script.
  #   if Exp::state("climbing") == 34
  #     exit
  #   end
  def self.state(exp_string)
    index = GameData.getExpIndex(exp_string.to_s)
    GameData.getExpState(index).to_i
  end
end

class Room
  # Room title.
  #
  # @param
  # @return [string] room title
  def self.title
    GameData.getRoomTitle().to_s
  end

  # Room description.
  #
  # @param
  # @return [string] room description
  def self.description
    GameData.getRoomDescription().to_s
  end

  # Room objects.
  #
  # @param
  # @return [string] room objects
  def self.objects
    GameData.getRoomObjects().to_s
  end

  # Room players.
  #
  # @param
  # @return [string] room players
  def self.players
    GameData.getRoomPlayers().to_s
  end

  # Room exits.
  #
  # @param
  # @return [string] room exits
  def self.exits
    GameData.getRoomExits().to_s
  end

  # Counts objects.
  #
  # @param [String] value oject name.
  # @return [Int] count of found objects in room.
  # @example Using object count in script.
  #   if Room::count_objects "hog" > 2
  #     put "attack"
  #   end
  def self.count_objects(value)
    self.objects.scan(value).length
  end
end

class Vitals
  # Health
  #
  # @param
  # @return [int] health value 0 - 100
  # @example Using health value in script.
  #   if Vitals::health < 100
  #     put retreat
  #   end
  def self.health
    GameData.getHealth().to_i
  end

  # Concentration
  #
  # @param
  # @return [int] concentration value 0 - 100
  def self.concentration
    GameData.getConcentration().to_i
  end

  # Fatigue
  #
  # @param
  # @return [int] fatigue value 0 - 100
  def self.fatigue
    GameData.getFatigue().to_i
  end

  # Health
  #
  # @param
  # @return [int] spirit value 0 - 100
  def self.spirit
    GameData.getSpirit().to_i
  end
end

class Status
  # Standing
  #
  # @param
  # @return [bool] is standing
  def self.standing
    GameData.getStanding().to_i
  end

  # Kneeling
  #
  # @param
  # @return [bool] is kneeling
  def self.kneeling
    GameData.getKneeling().to_i
  end

  # Sitting
  #
  # @param
  # @return [bool] is sitting
  def self.sitting
    GameData.getSitting().to_i
  end

  # Prone
  #
  # @param
  # @return [bool] is prone
  def self.prone
    GameData.getProne().to_i
  end

  # Stunned
  #
  # @param
  # @return [bool] is stunned
  def self.stunned
    GameData.getStunned().to_i
  end

  # Dead
  #
  # @param
  # @return [bool] is dead
  def self.dead
    GameData.getDead().to_i
  end

  # Bleeding
  #
  # @param
  # @return [bool] is bleeding
  def self.bleeding
    GameData.getBleeding().to_i
  end

  # Hidden
  #
  # @param
  # @return [bool] is hidden
  # @example Using is hidden in script.
  #   if Status::hidden
  #     put unhide
  #   end
  def self.hidden
    GameData.getHidden().to_i
  end

  # Invisible
  #
  # @param
  # @return [bool] is ivisible
  def self.invisible
    GameData.getInvisible().to_i
  end

  # Webbed
  #
  # @param
  # @return [bool] is webbed
  def self.webbed
    GameData.getWebbed().to_i
  end

  # Joined
  #
  # @param
  # @return [bool] is joined
  def self.joined
    GameData.getJoined().to_i
  end
end

# @api private
class CommandThread
  def run
    while line = gets
      if line.start_with? "game_text#"
        line.slice! "game_text#"
        $_data_queue << line
      elsif line.start_with? "exit#"
        Kernel::abort
      end
      sleep 0.01
    end
  end
end