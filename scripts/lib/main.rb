require "#{File.dirname(__FILE__)}/models.rb"
require "#{File.dirname(__FILE__)}/ruby_goto.rb"

$args = []
ARGV.each do |arg|
  $args << arg
end
ARGV.clear

# auto-flushing
STDOUT.sync = true

@_file = $args.shift

$_data_queue = []

@match_rt = 0
@match_rt_adjustment = 0

@_command_thread = Thread.new { CommandThread.new.run }

# Waits for roundtime in game text and pauses for the duration.
#
# @param
# @return [void]
# @example Wait for the duration of the round time before executing next command.
#   put "hide"
#   wait_for_roundtime
#   put "unhide"
def wait_for_roundtime
  (0..1000000).each do
    $_data_queue.each_index do |i|
      if $_data_queue.at(i).match(/^Roundtime:/)
        sleep_for_rt $_data_queue.at(i)[/\d+/].to_i
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

# Matches multi regex patterns with game text
# and returns the name of the matching pattern.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Symbol] pattern name
# @example Using multi match patterns to make decisions in script.
#   match = { :retry => [/\.\.\.wait/], :next => [/you open/] }
#   result = match_wait match
#   result #=> :retry or :next
#   if result = :next
#     echo "next"
#   end
def match_wait(pattern)
  match_found = false
  match = :not_found

  (0..1000000).each do
    $_data_queue.each_index do |i|
      unless match_found
        pattern.each_pair do |k, v|
          v.each do |m|
            if $_data_queue.at(i).match(m)
              match = k
              match_found = true
              break
            end
          end
          break if match_found
        end
      end

      if $_data_queue.at(i).match(/^Roundtime:/)
        sleep_for_rt $_data_queue.at(i)[/\d+/].to_i
      end

      if match_found
        if $_data_queue.at(i).match(/^>/)
          return match
        end
      end

      $_data_queue.delete_at(i)
    end
    sleep 0.01
  end
end

# Matches multi regex patterns with game text
# and goes to defined label.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Void]
# @example Using match patterns to go to predefined labels
#   labels_start
#
#   label(:retry){
#     match = { :retry => [/\.\.\.wait/], :next => [/you open/] }
#     match_wait match
#     echo "retry"
#   }
#
#   label(:next){
#     echo "next"
#   }
#
#   labels_end
def match_wait_goto(pattern)
  match_found = false
  match = :not_found

  (0..1000000).each do
    $_data_queue.each_index do |i|
      unless match_found
        pattern.each_pair do |k, v|
          v.each do |m|
            if $_data_queue.at(i).match(m)
              match = k
              match_found = true
              break
            end
          end
          break if match_found
        end
      end

      if $_data_queue.at(i).match(/^Roundtime:/)
        sleep_for_rt $_data_queue.at(i)[/\d+/].to_i
      end

      if match_found
        if $_data_queue.at(i).match(/^>/)
          goto match
        end
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
  $_data_queue.clear
  puts "put#" + value.to_s
  STDOUT.flush
end

# Sends a command to client and waits for room title.
#
# @param [String] value command.
# @return [void]
# @example Using move command in script.
#   move "n"
#   move "e"
#   move "go gate"
def move(value)
  put value
  res = match_wait({ :room => [/^\[.*?\]$/],
                     :wait => [/\.\.\.wait/, /you may only type ahead/] })
  if res == :wait
    pause 0.5
    move value
  end
end

# Waits for a prompt character after a command is issued.
#
# @param
# @return [void]
# @example Using wait in script to run consecutive commands.
#   put "remove my shield"
#   wait
#   put "wear my shield"
#   wait
#   put "remove my shield"
def wait
  $_data_queue.clear
  wait_for(/^>/)
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
end

# Pauses for given time.
#
# @param [Integer, Float] value sleep time in seconds
# @return [Void]
def pause(value)
  sleep value
end

# Execute a ruby script file.
#
# @param [String] name name of the file
# @return [Void]
def execute(name)
  load "#{Dir.pwd}/scripts/#{name}.rb"
end

# Current match round time -- can be used in
# secondary threads while main thread is stuck in round time
#
# @param
# @return [Integer] current round time value
def get_match_rt
  @match_rt
end

# @private
def end_command_thread
  puts "end#"
end

# @private
def exit
  end_command_thread
  Kernel::exit
end

# @private
def sleep_for_rt(rt)
  if rt > 0
    rt = rt - 1 + @match_rt_adjustment
  end
  rt.downto(0) do |current_rt|
    @match_rt = current_rt
    sleep 1
  end
end

# @private
at_exit do
  if defined? finally_do
    unless @_command_thread.alive?
      @_command_thread = Thread.new { CommandThread.new.run }
    end
    sleep Rt::value
    finally_do
    end_command_thread
  end
end

# wait for round time
sleep Rt::value

# load script file here
require @_file

# end command thread after finished
end_command_thread