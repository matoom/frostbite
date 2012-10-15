require "#{File.dirname(__FILE__)}/models.rb"

$args = []
ARGV.each do |arg|
  $args << arg
end
ARGV.clear

STDOUT.sync = true

@_file = $args.shift

$_data_queue = []
$_exp, $_room, $_wield, $_vitals, $_status, $_inventory, $_container = "", "", "", "", "", "", ""

@current_rt = 0

@_command_thread = Thread.new { CommandThread.new.run }

# Waits for roundtime in game text and pauses for the duration.
#
# @param
# @return [void]
# @example Wait for the duration of the round time before executing next command.
#   put hide
#   wait_for_roundtime
#   put unhide
def wait_for_roundtime
  $_data_queue.clear
  (0..1000000).each do
    $_data_queue.each_index do |i|
      if $_data_queue.at(i).match(/^Roundtime:/)
        total_rt = $_data_queue.at(i)[/\d+/].to_i
        total_rt.downto(0) do |current_rt|
          @current_rt = current_rt
          sleep 1
        end
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
def wait_for(pattern)
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

# Matches multiple regex patterns with game text
# and returns the name of the matching pattern.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Symbol] pattern name
# @example Using multiple match patterns to make decisions in script.
#   match = {:retry => "...wait", :next => "you open"}
#   result = match_wait match
#   result #=> :retry or :next
#   if result = :next
#     echo "next"
#   end
def match_wait(pattern)
  $_data_queue.clear
  match_found = false
  match = ""
  (0..1000000).each do
    $_data_queue.each_index do |i|
      unless match_found
        pattern.each_pair do |k, v|
          if $_data_queue.at(i).match(v)
            match = k
            match_found = true
          end
        end
      end
      if $_data_queue.at(i).match(/^Roundtime:/)
        sleep $_data_queue.at(i)[/\d+/].to_i
      end
      $_data_queue.delete_at(i)
    end
    if match_found
      return match
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
# @example Using move command in script.
#   move n
#   move e
#   move go gate
def move(value)
  put value
  wait_for(/^\[.*?\]$/)
end

# Waits for a prompt character after a command is issued.
#
# @param
# @return [void]
# @example Using wait in script to run consecutive commands.
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
# @param [#to_s] value message.
# @return [void]
# @example Echoing into client main window.
#   echo "hello"
#   echo 1
def echo(value)
  puts "echo#" + value.to_s
  STDOUT.flush
end

# Gets experience data from client.
#
# @param [String] value field name.
# @return [Exp] Exp object.
# @example Using exp data in script.
#   get_exp("climbing").inspect #=> #<Exp:0x2b051b0 @rank=222, @progress="53%", @state="cogitating", @numeric_state=25>
#
#   if get_exp("climbing").numeric_state == 34
#     exit
#   end
def get_exp(value)
  $_exp.clear
  puts "get_exp#" + value
  STDOUT.flush
  while true
    unless $_exp.empty?
      return Exp.new($_exp)
    end
  end
end

# Gets room data from client.
#
# @param
# @return [Room] Room object.
# @example Using room data in script.
#   get_room.inspect #=> #<Room:0x2aee140 @description="Well-worn paths ...", @objects="You also see a musk hog, a musk hog which appears dead and a musk hog.", @players="", @exits="Obvious paths: northwest.">
#
#   if get_room.count_objects "hog" > 2
#     put "attack"
#   end
def get_room
  $_room.clear
  puts "get_room#"
  STDOUT.flush
  while true
    unless $_room.empty?
      return Room.new($_room)
    end
  end
end

# Gets wield data from client.
#
# @param
# @return [Wield] Wield object.
# @example Using wield data in script.
#   get_wield.inspect #=> #<Wield:0x2ac99c0 @left="Empty", @left_noun="", @right="fuzzy sharks", @right_noun="sharks">
#
#   put "put my #{get_wield.right} in my backpack"
def get_wield
  $_wield.clear
  puts "get_wield#"
  STDOUT.flush
  while true
    unless $_wield.empty?
      return Wield.new($_wield)
    end
  end
end

# Gets vitals data from client.
#
# @param
# @return [Vitals] Vitals object.
# @example Using vitals data in script.
#   get_vitals.inspect #=> #<Vitals:0x3aa328 @health="100", @concentration="46", @fatigue="100", @spirit="100">
#
#   if get_vitals.health < 100
#     put retreat
#   end
def get_vitals
  $_vitals.clear
  puts "get_vitals#"
  STDOUT.flush
  while true
    unless $_vitals.empty?
      return Vitals.new($_vitals)
    end
  end
end

# Gets status data from client.
#
# @param
# @return [Status] Status object.
# @example Using status data in script.
#   get_status.inspect #=> #<Status:0x3a9d10 @kneeling=0, @prone=0, @sitting=0, @standing=1, @stunned=1, @dead=0, @bleeding=1, @hidden=1, @invisible=0, @webbed=0, @joined=1>
#
#   if get_status.hidden
#     put unhide
#   end
def get_status
  $_status.clear
  puts "get_status#"
  STDOUT.flush
  while true
    unless $_status.empty?
      return Status.new($_status)
    end
  end
end


# Gets inventory list from client.
#
# @param
# @return [Status] Status object.
def get_inventory
  $_inventory.clear
  puts "get_inventory#"
  STDOUT.flush
  while true
    unless $_inventory.empty?
      return Inventory.new($_inventory)
    end
  end
end

# Gets items in main stow container from client.
#
# @param
# @return [Container] Container object.
def get_container
  $_container.clear
  puts "get_container#"
  STDOUT.flush
  while true
    unless $_container.empty?
      return Container.new($_container)
    end
  end
end

# Pauses for given time.
#
# @param [Integer, Float] value sleep time in seconds
# @return [Void]
def pause(value)
  sleep value
end

# Current match round time -- can be used in
# secondary threads while main thread is stuck in round time
#
# @param
# @return [Integer] current round time value
# @example Script is interrupted during round time.
#   def finally_do
#     sleep get_match_rt
#     put "stow right"
#   end
def get_match_rt
  @current_rt
end

at_exit do
  if defined? finally_do
    unless @_command_thread.alive?
      @_command_thread = Thread.new { CommandThread.new.run }
    end
    finally_do
    Kernel::exit!
  end
end

#load script file here
load @_file