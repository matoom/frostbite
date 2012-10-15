class Inventory
  # @!attribute list
  #   @return [Array] inventory item list
  attr_accessor :list

  def initialize(inventory_string)
    @list = inventory_string.chomp.split("|")
  end
end

class Container
  # @!attribute list
  #   @return [Array] container item list
  attr_accessor :list

  def initialize(container_string)
    @list = container_string.chomp.split("|")
  end
end

class Wield
  # @!attribute left
  #   @return [String] description of the item held in left hand
  # @!attribute left_noun
  #   @return [String] game operated noun for left hand
  # @!attribute right
  #   @return [String] description of the item held in right hand
  # @!attribute right_noun
  #   @return [String] game operated noun for right hand
  attr_accessor :left, :left_noun, :right, :right_noun

  def initialize(wield_string)
    process_values(wield_string.chomp)
  end

  # Extracts values from given string.
  #
  # @param [String] wield_string data string.
  # @return [Void]
  def process_values(wield_string)
    wield_values = wield_string.split("|")

    @left = wield_values[0]
    @left_noun = wield_values[1]
    @right = wield_values[2]
    @right_noun = wield_values[3]
  end
end

class Exp
  # @!attribute rank
  #   @return [Int] rank of skill
  # @!attribute progress
  #   @return [String] current progression of the rank
  # @!attribute state
  #   @return [String] current skill state
  # @!attribute numeric_state
  #   @return [Int] current skill state as numeric value
  attr_accessor :rank, :progress, :state, :numeric_state

  def initialize(exp_string)
    process_values(exp_string.chomp)
  end

  # Extracts values from given string.
  #
  # @param [String] exp experience data.
  # @return [Void]
  def process_values(exp)
    exp_values = exp.split("|")

    @rank = exp_values[1].to_i
    @progress = exp_values[2]
    @state = exp_values[3]
    @numeric_state = exp_values[4].to_i
  end
end

class Room
  # @!attribute description
  #   @return [String] room description
  # @!attribute objects
  #   @return [String] ojects in the room
  # @!attribute players
  #   @return [String] players in the room
  # @!attribute exits
  #   @return [String] exits in the room
  attr_accessor :description, :objects, :players, :exits

  def initialize(room_string)
    room_values = room_string.chomp.split("|")

    @description = room_values[0]
    @objects = room_values[1]
    @players = room_values[2]
    @exits = room_values[3]
  end

  # Counts objects.
  #
  # @param [String] value oject name.
  # @return [Int] count of found objects in room.
  def count_objects(value)
    @objects.scan(value).length
  end
end


class Vitals
  # @!attribute health
  #   @return [Int] health value
  # @!attribute concentration
  #   @return [Int] concentration value
  # @!attribute fatigue
  #   @return [Int] fatigue value
  # @!attribute spirit
  #   @return [Int] spirit value
  attr_accessor :health, :concentration, :fatigue, :spirit

  def initialize(vitals_string)
    vitals_values = vitals_string.chomp.split("|")

    @health = vitals_values[0]
    @concentration = vitals_values[1]
    @fatigue = vitals_values[2]
    @spirit = vitals_values[3]
  end
end

class Status
  # @!attribute kneeling
  #   @return [bool] is kneeling
  # @!attribute prone
  #   @return [bool] is prone
  # @!attribute sitting
  #   @return [bool] is sitting
  # @!attribute standing
  #   @return [bool] is standing
  # @!attribute stunned
  #   @return [bool] is stunned
  # @!attribute dead
  #   @return [bool] is dead
  # @!attribute bleeding
  #   @return [bool] is bleeding
  # @!attribute hidden
  #   @return [bool] is hidden
  # @!attribute invisible
  #   @return [bool] is invisible
  # @!attribute webbed
  #   @return [bool] is webbed
  # @!attribute joined
  #   @return [bool] is joined
  attr_accessor :kneeling, :prone, :sitting, :standing, :stunned,
                :dead, :bleeding, :hidden, :invisible, :webbed, :joined

  def initialize(status_string)
    status_values = status_string.chomp.unpack('H*').first.scan(/../)

    @kneeling = status_values[0].to_i
    @prone = status_values[1].to_i
    @sitting = status_values[2].to_i
    @standing = status_values[3].to_i
    @stunned = status_values[4].to_i
    @dead = status_values[5].to_i
    @bleeding = status_values[6].to_i
    @hidden = status_values[7].to_i
    @invisible = status_values[8].to_i
    @webbed = status_values[9].to_i
    @joined = status_values[10].to_i
  end
end

# @api private
class CommandThread
  def run
    while line = gets
      if line.start_with? "game_text#"
        line.slice! "game_text#"
        $_data_queue << line
      elsif line.start_with? "exp#"
        line.slice! "exp#"
        $_exp = line
      elsif line.start_with? "room#"
        line.slice! "room#"
        $_room = line
      elsif line.start_with? "wield#"
        line.slice! "wield#"
        $_wield = line
      elsif line.start_with? "vitals#"
        line.slice! "vitals#"
        $_vitals = line
      elsif line.start_with? "status#"
        line.slice! "status#"
        $_status = line
      elsif line.start_with? "inventory#"
        line.slice! "inventory#"
        $_inventory = line
      elsif line.start_with? "container#"
        line.slice! "container#"
        $_container = line
      elsif line.start_with? "exit#"
        Kernel::abort
      end
      sleep 0.01
    end
  end
end