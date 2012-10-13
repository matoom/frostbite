$args = []
ARGV.each do |arg|
  $args << arg
end
ARGV.clear

STDOUT.sync = true

$_data_queue = []
$_end = false

@_file = $args.shift
@_exp = nil
@_room = nil
@_wield = nil
@_vitals = nil
@_status = nil

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
        elsif line.start_with? "exit#"
          $_end = true
          Kernel::abort
        end
      end
   end
end

@_command_thread = Thread.new { CommandThread.new.run }

# Stops command thread
# @param
# @return [Void]
# @api private
def stop_command_thread
  if @_command_thread.alive?
    Thread.kill(@_command_thread)
  end
end

# Start command thread to intercept incoming commands
# @param
# @return [Void]
# @api private
def start_command_thread
  if !@_command_thread.alive? && !$_end
    @_command_thread = Thread.new { CommandThread.new.run }
  end
end


# Ends command thread and aborts the script
# @param
# @return [Void]
# @api private
def exit_script
  $_end = true
  abort
end

# Waits for roundtime in game text and pauses for the duration.
#
# @param
# @return [void]
# @example using wait_for_roundtime in script
#   put hide
#   wait_for_roundtime
#   put unhide
def wait_for_roundtime
  $_data_queue.clear
  (0..1000000).each do
    $_data_queue.each_index do |i|
      #Roundtime: 4 sec.
      #Roundtime:  12 seconds.
      if $_data_queue.at(i).match(/^Roundtime:  \d+ seconds.$/)
        sleep $_data_queue.at(i)[/\d+/].to_i
        return
      end
      $_data_queue.delete_at(i)
    end
    sleep 0.01
  end
end

# Waits until a match is found in game text.
#
# @param [String] pattern regex pattern.
# @return [void]
def match_wait(pattern)
  $_data_queue.clear
  (0..1000000).each do
    $_data_queue.each_index do |i|
      if $_data_queue.at(i).match(pattern)
        return
      end
      $_data_queue.delete_at(i)
    end
    sleep 0.01
  end
end

# Sends a command to client.
#
# @param [String] value command.
# @return [void]
def put(value)
  puts "put#" + value.to_s
  STDOUT.flush
end

# Sends a command to client and waits for room title.
#
# @param [String] value command.
# @return [void]
# @example using move in script
#   move n
#   move e
#   move go gate
def move(value)
  put value
  match_wait(/^\[.*?\]$/)
end

# Waits for a prompt character after a command is issued.
#
# @param
# @return [void]
# @example using wait in script
#   put remove my shield
#   wait
#   put wear my shield
#   wait
#   put remove my shield
def wait
  match_wait(/^>/)
end

# Sends a message to client main window.
#
# @param [String] value message.
# @return [void]
def echo(value)
  puts "echo#" + value.to_s
  STDOUT.flush
end

# Gets experience data from client.
#
# @param [String] value field name.
# @return [Exp] Exp object.
# @example using exp data in script
#   get_exp("climbing").inspect #=> #<Exp:0x2b051b0 @rank=222, @progress="53%", @state="cogitating", @numeric_state=25>
#
#   if get_exp("climbing").numeric_state == 34
#     exit
#   end
def get_exp(value)
  stop_command_thread
  puts "get_exp#" + value
  STDOUT.flush
  while line = gets
    if line.start_with? "exp#"
      line.slice! "exp#"
      start_command_thread
      return Exp.new(line)
    elsif line.start_with? "exit#"
      exit_script
    end
  end
end

# Gets room data from client.
#
# @param
# @return [Room] Room object.
# @example using room data in script
#   get_room.inspect #=> #<Room:0x2aee140 @description="Well-worn paths ...", @objects="You also see a musk hog, a musk hog which appears dead and a musk hog.", @players="", @exits="Obvious paths: northwest.">
#
#   if get_room.count_objects "hog" > 2
#     put "attack"
#   end
def get_room
  stop_command_thread
  puts "get_room#"
  STDOUT.flush
  while line = gets
    if line.start_with? "room#"
      line.slice! "room#"
      start_command_thread
      return Room.new(line)
    elsif line.start_with? "exit#"
      exit_script
    end
  end
end

# Gets wield data from client.
#
# @param
# @return [Wield] Wield object.
# @example using wield data in script
#   get_wield.inspect #=> #<Wield:0x2ac99c0 @left="Empty", @left_noun="", @right="fuzzy sharks", @right_noun="sharks">
#
#   put "put my #{get_wield.right} in my backpack"
def get_wield
  stop_command_thread
  puts "get_wield#"
  STDOUT.flush
  while line = gets
    if line.start_with? "wield#"
      line.slice! "wield#"
      start_command_thread
      return Wield.new(line)
    elsif line.start_with? "exit#"
      exit_script
    end
  end
end

# Gets vitals data from client.
#
# @param
# @return [Vitals] Vitals object.
# @example using vitals data in script
#   get_vitals.inspect #=> #<Vitals:0x3aa328 @health="100", @concentration="46", @fatigue="100", @spirit="100">
#
#   if get_vitals.health < 100
#     put retreat
#   end
def get_vitals
  stop_command_thread
  puts "get_vitals#"
  STDOUT.flush
  while line = gets
    if line.start_with? "vitals#"
      line.slice! "vitals#"
      start_command_thread
      return Vitals.new(line)
    elsif line.start_with? "exit#"
      exit_script
    end
  end
end

# Gets status data from client.
#
# @param
# @return [Status] Status object.
# @example using status data in script
#   get_status.inspect #=> #<Status:0x3a9d10 @kneeling=0, @prone=0, @sitting=0, @standing=1, @stunned=1, @dead=0, @bleeding=1, @hidden=1, @invisible=0, @webbed=0, @joined=1>
#
#   if get_status.hidden
#     put unhide
#   end
def get_status
  stop_command_thread
  puts "get_status#"
  STDOUT.flush
  while line = gets
    if line.start_with? "status#"
      line.slice! "status#"
      start_command_thread
      return Status.new(line)
    elsif line.start_with? "exit#"
      exit_script
    end
  end
end

# Pauses for given time.
#
# @param [#to_i] value sleep time in seconds
# @return [Void]
def pause(value)
  sleep value.to_i
end

at_exit do
  if defined? finally_do
    finally_do
  end
end

#load script file here
load @_file