require "#{File.dirname(__FILE__)}/data.rb"

class Rt
  # Roundtime
  #
  # @param
  # @return [int] roundtime value
  # @example Using round time in script.
  #   echo Rt::value
  #   => 5
  def self.value
    GameData.getRt().to_i
  end
end

class Inventory
  # Inventory items
  #
  # @param
  # @return [Array] inventory item list
  # @example Using inventory list in script.
  #   echo Inventory::list
  #   => ["a lumpy bundle", "an origami-paper envelope", "a heavy burlap haversack",
  #       "a simple belt knife", "a sturdy troll-skin herb pouch"]
  def self.list
    GameData.getInventory().to_s.split(", ")
  end
end

class Container
  # Container items
  #
  # @param
  # @return [Array] container item list
  # @example Using container list in script.
  #   echo Container::list
  #   => ["a rock", "a brown pouch", "a misshaped brass chest", "a mud-stained steel chest",
  #       "a blue gem pouch"]
  def self.list
    GameData.getContainer().to_s.split(", ")
  end
end

class Wield
  # Wield right
  #
  # @param
  # @return [String]
  # @example Using wield right in script.
  #   echo Wield::right
  #   => fuzzy sharks
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
  # @example Using wield left in script.
  #   echo Wield::left
  #   => ""
  def self.left()
    GameData.getWieldLeft().to_s
  end

  # Wield left noun
  #
  # @param
  # @return [String]
  # @example Using wield left noun in script.
  #   echo Wield::left_noun
  #   => ""
  def self.left_noun()
    GameData.getWieldLeftNoun().to_s
  end
end

class Exp
  # Exp rank
  #
  # @param
  # @return [int] rank of skill
  # @example Using exp rank in script.
  #   echo Exp::rank
  #   => 222
  def self.rank(exp_string)
    GameData.getExpRank(exp_string.to_s.downcase).to_i
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
    GameData.getExpState(exp_string.to_s.downcase).to_i
  end
end

class Room
  # Room title.
  #
  # @param
  # @return [string] room title
  # @example Using room title in script.
  #   echo Room::title
  #   => [Mycthengelde, Flatlands]
  def self.title
    GameData.getRoomTitle().to_s
  end

  # Room description.
  #
  # @param
  # @return [string] room description
  # @example Using room description in script.
  #   echo Room::description
  #   => Well-worn paths lead through a grove of trees to a gate in The Crossing's
  #      western wall.  Now and again you hear birds ...
  def self.description
    GameData.getRoomDescription().to_s
  end

  # Room objects.
  #
  # @param
  # @return [string] room objects
  # @example Using room objects in script.
  #   echo Room::objects
  #   => You also see a musk hog.
  def self.objects
    GameData.getRoomObjects().to_s
  end

  # Room players.
  #
  # @param
  # @return [string] room players
  # @example Using room players in script.
  #   echo Room::players
  #   => ""
  def self.players
    GameData.getRoomPlayers().to_s
  end

  # Room exits.
  #
  # @param
  # @return [string] room exits
  # @example Using room exits in script.
  #   echo Room::exits
  #   => Obvious paths: northwest.
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
  # @example Using concentration value in script.
  #   echo Vitals::concentration
  #   => 100
  def self.concentration
    GameData.getConcentration().to_i
  end

  # Fatigue
  #
  # @param
  # @return [int] fatigue value 0 - 100
  # @example Using fatigue value in script.
  #   echo Vitals::fatigue
  #   => 100
  def self.fatigue
    GameData.getFatigue().to_i
  end

  # Health
  #
  # @param
  # @return [int] spirit value 0 - 100
  # @example Using spirit value in script.
  #   echo Vitals::spirit
  def self.spirit
    GameData.getSpirit().to_i
  end
end

class Status
  # Standing
  #
  # @param
  # @return [bool] is standing
  # @example Echoing standing status to main window
  #   echo Status::standing
  #   => 1
  def self.standing
    !GameData.getStanding().to_i.zero?
  end

  # Kneeling
  #
  # @param
  # @return [bool] is kneeling
  # @example Echoing kneeling status to main window
  #   echo Status::kneeling
  #   => 0
  def self.kneeling
    !GameData.getKneeling().to_i.zero?
  end

  # Sitting
  #
  # @param
  # @return [bool] is sitting
  # @example Echoing sitting status to main window
  #   echo Status::sitting
  #   => 0
  def self.sitting
    !GameData.getSitting().to_i.zero?
  end

  # Prone
  #
  # @param
  # @return [bool] is prone
  # @example Echoing prone status to main window
  #   echo Status::prone
  #   => 0
  def self.prone
    !GameData.getProne().to_i.zero?
  end

  # Stunned
  #
  # @param
  # @return [bool] is stunned
  # @example Echoing stunned status to main window
  #   echo Status::stunned
  #   => 0
  def self.stunned
    !GameData.getStunned().to_i.zero?
  end

  # Dead
  #
  # @param
  # @return [bool] is dead
  # @example Echoing dead status to main window
  #   echo Status::dead
  #   => 0
  def self.dead
    !GameData.getDead().to_i.zero?
  end

  # Bleeding
  #
  # @param
  # @return [bool] is bleeding
  # @example Echoing bleeding status to main window
  #   echo Status::bleeding
  #   => 1
  def self.bleeding
    !GameData.getBleeding().to_i.zero?
  end

  # Hidden
  #
  # @param
  # @return [bool] is hidden
  # @example Using is hidden in script.
  #   if Status::hidden
  #     put unhide
  #   end
  #   => 1
  def self.hidden
    !GameData.getHidden().to_i.zero?
  end

  # Invisible
  #
  # @param
  # @return [bool] is ivisible
  # @example Echoing invisible status to main window
  #   echo Status::invisible
  #   => 0
  def self.invisible
    !GameData.getInvisible().to_i.zero?
  end

  # Webbed
  #
  # @param
  # @return [bool] is webbed
  # @example Echoing webbed status to main window
  #   echo Status::webbed
  #   => 0
  def self.webbed
    !GameData.getWebbed().to_i.zero?
  end

  # Joined
  #
  # @param
  # @return [bool] is joined
  # @example Echoing joined status to main window
  #   echo Status::joined
  #   => 1
  def self.joined
    !GameData.getJoined().to_i.zero?
  end
end

# @private
class CommandThread
  def run
    while line = gets
      if line.start_with? "game_text#"
        line.slice! "game_text#"
        $_data_queue << line

        if $_observer_started
          $_observer_queue << line
        end
      elsif line.start_with? "exit#"
        Kernel::abort
      end
      sleep 0.01
    end
  end
end