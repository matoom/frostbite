require 'socket'

# @private
module ApiSettings
  #constants
  API_PUT_PREFIX = "put#"
  API_END_PREFIX = "end#"
  API_ECHO_PREFIX = "echo#"
  API_CMD_SUFFIX = "\n"

  MATCH_START_KEY = :match_start
  MATCH_END_KEY = :match_end

  API_ADR = '127.0.0.1'

  def self.api_port
    File.open("#{File.dirname(__FILE__)}/../../api.ini", 'r') do |inFile|
      inFile.each_line do |line|
        return line.partition('=').last.to_i if line.start_with? "port"
      end
    end
  end
end

module ApiSocket
  def self.init
    $_api_socket = TCPSocket.open(ApiSettings::API_ADR, ApiSettings::api_port)
    $_api_socket.setsockopt(Socket::IPPROTO_TCP, Socket::TCP_NODELAY, 1)
  end
end

class Rt
  # Roundtime
  #
  # @param
  # @return [int] roundtime value
  # @example Using round time in script.
  #   echo Rt::value
  #   => 5
  def self.value
    $_api_socket.puts "GET RT\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end
end

class Spell
  # Active spells
  #
  # @param
  # @return [Array] active spell list
  # @example Using active spells in script.
  #   echo Spell::active
  #   => ["Khri Sagacity  (6 roisaen)", "Khri Shadowstep  (34 roisaen)", "Khri Skulk"]
  def self.active
    $_api_socket.puts "GET ACTIVE_SPELLS\n"
    $_api_socket.gets('\0').chomp('\0').split("\n")
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
    $_api_socket.puts "GET INVENTORY\n"
    $_api_socket.gets('\0').chomp('\0').to_s.split("\n")
  end
end

class Container
  # Container items
  #
  # @param
  # @return [Array] container item list
  # @example Using container list in script.
  #   echo Container::list.inspect
  #   => ["a rock", "a brown pouch", "a misshaped brass chest", "a mud-stained steel chest",
  #       "a blue gem pouch"]
  def self.list
    $_api_socket.puts "GET CONTAINER\n"
    $_api_socket.gets('\0').chomp('\0').to_s.split("\n")
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
    $_api_socket.puts "GET WIELD_RIGHT\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Wield right noun
  #
  # @param
  # @return [String]
  # @example Using wield right noun in script.
  #   put "put my #{Wield::right_noun} in my backpack"
  def self.right_noun()
    $_api_socket.puts "GET WIELD_RIGHT_NOUN\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Wield left
  #
  # @param
  # @return [String]
  # @example Using wield left in script.
  #   echo Wield::left
  #   => ""
  def self.left()
    $_api_socket.puts "GET WIELD_LEFT\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Wield left noun
  #
  # @param
  # @return [String]
  # @example Using wield left noun in script.
  #   echo Wield::left_noun
  #   => ""
  def self.left_noun()
    $_api_socket.puts "GET WIELD_LEFT_NOUN\n"
    $_api_socket.gets('\0').chomp('\0').to_s
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
    $_api_socket.puts "GET EXP_RANK?#{exp_string.to_s.downcase}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
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
    $_api_socket.puts "GET EXP_STATE?#{exp_string.to_s.downcase}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
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
    $_api_socket.puts "GET ROOM_TITLE\n"
    $_api_socket.gets('\0').chomp('\0').to_s
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
    $_api_socket.puts "GET ROOM_DESC\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Room objects.
  #
  # @param
  # @return [string] room objects
  # @example Using room objects in script.
  #   echo Room::objects
  #   => You also see a musk hog.
  def self.objects
    $_api_socket.puts "GET ROOM_OBJECTS\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Room players.
  #
  # @param
  # @return [string] room players
  # @example Using room players in script.
  #   echo Room::players
  #   => ""
  def self.players
    $_api_socket.puts "GET ROOM_PLAYERS\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Room exits.
  #
  # @param
  # @return [string] room exits
  # @example Using room exits in script.
  #   echo Room::exits
  #   => Obvious paths: northwest.
  def self.exits
    $_api_socket.puts "GET ROOM_EXITS\n"
    $_api_socket.gets('\0').chomp('\0').to_s
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
    $_api_socket.puts "GET HEALTH\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # Concentration
  #
  # @param
  # @return [int] concentration value 0 - 100
  # @example Using concentration value in script.
  #   echo Vitals::concentration
  #   => 100
  def self.concentration
    $_api_socket.puts "GET CONCENTRATION\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # Fatigue
  #
  # @param
  # @return [int] fatigue value 0 - 100
  # @example Using fatigue value in script.
  #   echo Vitals::fatigue
  #   => 100
  def self.fatigue
    $_api_socket.puts "GET FATIGUE\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # Health
  #
  # @param
  # @return [int] spirit value 0 - 100
  # @example Using spirit value in script.
  #   echo Vitals::spirit
  def self.spirit
    $_api_socket.puts "GET SPIRIT\n"
    $_api_socket.gets('\0').chomp('\0').to_i
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
    $_api_socket.puts "GET STANDING\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Kneeling
  #
  # @param
  # @return [bool] is kneeling
  # @example Echoing kneeling status to main window
  #   echo Status::kneeling
  #   => 0
  def self.kneeling
    $_api_socket.puts "GET KNEELING\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Sitting
  #
  # @param
  # @return [bool] is sitting
  # @example Echoing sitting status to main window
  #   echo Status::sitting
  #   => 0
  def self.sitting
    $_api_socket.puts "GET SITTING\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Prone
  #
  # @param
  # @return [bool] is prone
  # @example Echoing prone status to main window
  #   echo Status::prone
  #   => 0
  def self.prone
    $_api_socket.puts "GET PRONE\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Stunned
  #
  # @param
  # @return [bool] is stunned
  # @example Echoing stunned status to main window
  #   echo Status::stunned
  #   => 0
  def self.stunned
    $_api_socket.puts "GET STUNNED\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Dead
  #
  # @param
  # @return [bool] is dead
  # @example Echoing dead status to main window
  #   echo Status::dead
  #   => 0
  def self.dead
    $_api_socket.puts "GET DEAD\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Bleeding
  #
  # @param
  # @return [bool] is bleeding
  # @example Echoing bleeding status to main window
  #   echo Status::bleeding
  #   => 1
  def self.bleeding
    $_api_socket.puts "GET BLEEDING\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
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
    $_api_socket.puts "GET HIDDEN\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Invisible
  #
  # @param
  # @return [bool] is ivisible
  # @example Echoing invisible status to main window
  #   echo Status::invisible
  #   => 0
  def self.invisible
    $_api_socket.puts "GET INVISIBLE\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Webbed
  #
  # @param
  # @return [bool] is webbed
  # @example Echoing webbed status to main window
  #   echo Status::webbed
  #   => 0
  def self.webbed
    $_api_socket.puts "GET WEBBED\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end

  # Joined
  #
  # @param
  # @return [bool] is joined
  # @example Echoing joined status to main window
  #   echo Status::joined
  #   => 1
  def self.joined
    $_api_socket.puts "GET JOINED\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end
end

# @private
class CommandThread
  TEXT_PREFIX = "game_text#"
  EXIT_PREFIX = "exit#"

  def run
    while line = gets
      if line.start_with? TEXT_PREFIX
        sync_write line[TEXT_PREFIX.size, line.size]
        sync_write_observer line[TEXT_PREFIX.size, line.size]
      elsif line.start_with? EXIT_PREFIX
        Kernel::abort
      end
      sleep 0.01
    end
  end

  def sync_write line
    $_api_gets_mutex.synchronize do
      $_api_queue << line
    end
  end

  def sync_write_observer line
    if $_api_observer_started
      $_api_gets_mutex.synchronize do
        $_api_observer_queue << line
      end
    end
  end
end