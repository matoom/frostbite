require 'socket'
require "erb"

class Rt
  # Round time
  #
  # @return [int] roundtime value
  # @example Using round time in script.
  #   echo Rt::value
  #   => 5
  def self.value
    $_api_socket.puts "GET RT\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end
end

class Ct
  # Cast time
  #
  # @return [int] cast time value
  # @example Using cast time in script.
  #   echo Ct::value
  #   => 5
  def self.value
    $_api_socket.puts "GET CT\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end
end

class Spell
  # Active spells
  #
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
  # @return [String]
  # @example Using wield right noun in script.
  #   put "put my #{Wield::right_noun} in my backpack"
  def self.right_noun()
    $_api_socket.puts "GET WIELD_RIGHT_NOUN\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Wield left
  #
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
  # @param [String] exp_string exp field name
  # @return [int] rank of skill
  # @example Using exp rank in script.
  #   echo Exp::rank
  #   => 222
  def self.rank(exp_string)
    $_api_socket.puts "GET EXP_RANK?#{ERB::Util.url_encode(exp_string.to_s.downcase)}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # Exp state
  #
  # @param [String] exp_string exp field name
  # @return [int] learning state */34
  # @example Using exp state in script.
  #   if Exp::state("climbing") == 34
  #     exit
  #   end
  def self.state(exp_string)
    $_api_socket.puts "GET EXP_STATE?#{ERB::Util.url_encode(exp_string.to_s.downcase)}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # List of internal exp names
  #
  # @return [Array] list of exp names
  # @example Using exp state in script.
  #   echo Exp::names
  #   >> ["Teaching", "Light Crossbow", "Scholarship", "First Aid", "Climbing"]
  def self.names()
    $_api_socket.puts "GET EXP_NAMES\n"
    $_api_socket.gets('\0').chomp('\0').to_s.split("\n")
  end
end

class Room
  # Room title.
  #
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

  # Monsters bold.
  #
  # @return [Array] list of bold monsters in room
  # @example Using monsters bold in script.
  # @note set MonsterBold
  #   echo Room::monsters_bold
  #   => ["a ship's rat"]
  def self.monsters_bold
    $_api_socket.puts "GET ROOM_MONSTERS_BOLD\n"
    $_api_socket.gets('\0').chomp('\0').to_s.split("\n")
  end
end

class Vitals
  # Health
  #
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
  # @return [bool] is joined
  # @example Echoing joined status to main window
  #   echo Status::joined
  #   => 1
  def self.joined
    $_api_socket.puts "GET JOINED\n"
    $_api_socket.gets('\0').chomp('\0').to_i.eql?(1)
  end
end

class Map

  # Path
  #
  # @param [String] zone zone id
  # @param [Integer] from start room id
  # @param [Integer] to destination room id
  # @return [Array] list of moves to destination (empty if path not found)
  # @example Find path between rooms
  #   echo Map::path 1, 1, 5
  #   => ["south", "south", "east"]
  def self.path(zone, from, to)
    $_api_socket.puts "MAP_GET PATH?#{ERB::Util.url_encode(zone)}&#{ERB::Util.url_encode(from)}&#{ERB::Util.url_encode(to)}\n"
    $_api_socket.gets('\0').chomp('\0').split(",")
  end

  # Zones
  #
  # @return [Array] list of available zones
  # @example get zones
  #   echo Map::zones
  #   => ["1", "14c", "1k", "2a", "31", "7"]
  def self.zones
    $_api_socket.puts "MAP_GET ZONES\n"
    $_api_socket.gets('\0').chomp('\0').split(",")
  end

  # Current room
  #
  # @return [Hash] room object (:zone=>"", :level=>-1, :id=>-1)
  # @example get current room
  #   echo Map::current_room.inspect
  #   => {:zone=>"", :level=>-1, :id=>-1}
  def self.current_room
    $_api_socket.puts "MAP_GET CURRENT_ROOM\n"
    eval($_api_socket.gets('\0').chomp('\0').to_s)
  end

  # Find room by note
  #
  # @param [String] note room note
  # @return [Hash] room object (:zone=>"", :level=>-1, :id=>-1)
  # @example find room
  #   room = Map::find_room "NE Gate"
  #   echo room[:id]
  #   => 171
  def self.find_room(note)
    $_api_socket.puts "MAP_GET FIND_ROOM?#{note}\n"
    eval($_api_socket.gets('\0').chomp('\0').to_s)
  end
end

class Client
  # Character name
  #
  # @return [String] current character name
  def self.char_name
    $_api_socket.puts "MAP CHAR_NAME\n"
    eval($_api_socket.gets('\0').chomp('\0').to_s)
  end

  # Connect prime
  #
  # @param [String] name character name
  # @param [String] user account user name
  # @param [String] pass account password
  # @return [int] 1 on success; 0 on fail
  def self.connect_prime(name, user, pass)
    Client.connect(API::AUTH_GAMES[:prime], name, user, pass)
  end

  # Connect test
  #
  # @param [String] name character name
  # @param [String] user account user name
  # @param [String] pass account password
  # @return [int] 1 on success; 0 on fail
  def self.connect_test(name, user, pass)
    Client.connect(API::AUTH_GAMES[:test], name, user, pass)
  end

  # Connect fallen
  #
  # @param [String] name character name
  # @param [String] user account user name
  # @param [String] pass account password
  # @return [int] 1 on success; 0 on fail
  def self.connect_fallen(name, user, pass)
    Client.connect(API::AUTH_GAMES[:fallen], name, user, pass)
  end

  # Connect plat
  #
  # @param [String] name character name
  # @param [String] user account user name
  # @param [String] pass account password
  # @return [int] 1 on success; 0 on fail
  def self.connect_plat(name, user, pass)
    Client.connect(API::AUTH_GAMES[:plat], name, user, pass)
  end

  # Connect game
  #
  # @param [String] game game instance code (DR, DRT, DRX, DRF)
  # @param [String] name character name
  # @param [String] user account user name
  # @param [String] pass account password
  # @return [int] 1 on success; 0 on fail
  def self.connect(game, name, user, pass)
    Client.connect_host(API::AUTH_HOST, API::AUTH_PORT, game, name, user, pass)
  end

  # Connect host
  #
  # @param [String] host game host
  # @param [String] port game port
  # @param [String] game game instance code (DR, DRT, DRX, DRF)
  # @param [String] name character name
  # @param [String] user account user name
  # @param [String] pass account password
  # @return [int] 1 on success; 0 on fail
  def self.connect_host(host, port, game, name, user, pass)
    $_api_socket.puts "CLIENT CONNECT?#{ERB::Util.url_encode(host)}&#{ERB::Util.url_encode(port)}&#{ERB::Util.url_encode(user)}&#{ERB::Util.url_encode(pass)}&#{ERB::Util.url_encode(game)}&#{ERB::Util.url_encode(name)}\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Track exp row in exp window
  #
  # @param [String] name internal exp name (see #Exp::names)
  # @return [int] 1 on success; 0 on fail
  def self.track_exp(name)
    $_api_socket.puts "CLIENT TRACK_EXP?#{ERB::Util.url_encode(name)}\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # Clear all tracked exp in exp window
  #
  # @return [int] 1 on success; 0 on fail
  def self.track_exp_clear
    $_api_socket.puts "CLIENT TRACK_EXP_CLEAR\n"
    $_api_socket.gets('\0').chomp('\0').to_s
  end

  # List stream windows in client
  #
  # @return [Array] unique window names
  def self.window_list
    $_api_socket.puts "CLIENT WINDOW_LIST\n"
    $_api_socket.gets('\0').chomp('\0').to_s.split("\n")
  end

  # Add stream window to client
  #
  # @param [String] name unique window id (see #Client::window_list)
  # @param [String] title window title
  # @return [int] 1 on success; 0 on fail
  def self.window_add(name, title)
    $_api_socket.puts "CLIENT WINDOW_ADD?#{ERB::Util.url_encode(name)}&#{ERB::Util.url_encode(title)}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # Remove stream window
  #
  # @param [String] name unique window id (see #Client::window_list)
  # @return [int] 1 on success; 0 on fail
  def self.window_remove(name)
    $_api_socket.puts "CLIENT WINDOW_REMOVE?#{ERB::Util.url_encode(name)}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # Clear stream window
  #
  # @param [String] name unique window id (see #Client::window_list)
  # @return [int] 1 on success; 0 on fail
  def self.window_clear(name)
    $_api_socket.puts "CLIENT WINDOW_CLEAR?#{ERB::Util.url_encode(name)}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # Write to stream window
  #
  # @param [String] name unique window id (see #Client::window_list)
  # @param [String] text html formatted text
  # @return [int] 1 on success; 0 on fail
  def self.window_write(name, text)
    $_api_socket.puts "CLIENT WINDOW_WRITE?#{ERB::Util.url_encode(name)}&#{ERB::Util.url_encode(text)}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end

  # System tray notification message
  #
  # @param [String] msg html formatted text
  # @return [int] 1 on success; 0 on fail
  def self.notify(msg)
    $_api_socket.puts "CLIENT TRAY_WRITE?#{ERB::Util.url_encode(msg)}\n"
    $_api_socket.gets('\0').chomp('\0').to_i
  end
end