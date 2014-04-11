Kernel.require "#{File.dirname(__FILE__)}/models.rb"
Kernel.require "#{File.dirname(__FILE__)}/ruby_goto.rb"
Kernel.require "#{File.dirname(__FILE__)}/observer.rb"

# show warnings
#$VERBOSE = true

$args = []
ARGV.each do |arg|
  $args << arg
end
ARGV.clear

# auto-flushing
STDOUT.sync = true

# script file
@_file = $args.shift

# globals
$_api_queue = []
$_api_observer_queue = []

$_api_interrupt_time = -1
$_api_exec_state = :running
$_api_observer_started = false
$_api_current_rt = 0

MATCH_START_KEY = :match_start
MATCH_END_KEY = :match_end

@_api_cmd_thread = Thread.new { CommandThread.new.run }

$rt_adjust = 0

# Waits for roundtime and pauses for the duration.
#
# @param
# @return [void]
# @example Wait for the duration of round time before executing next command.
#   put "hide"
#   wait_for_roundtime
#   put "unhide"
def wait_for_roundtime
  (0..1000000).each do
    while line = $_api_queue.shift
      if line.match(/Roundtime/)
        api_sleep line[/\d+/].to_i + $rt_adjust
        return
      end
    end
    sleep 0.01
  end
end

# Pauses for current roundtime
#
# @param
# @return [void]
def pause_for_roundtime
  if $_api_current_rt > 0
    api_sleep $_api_current_rt + $rt_adjust
  end
end

# Runs until matching regex pattern
# is found in game text.
#
# @param [String] pattern regex pattern.
# @return [void]
def wait_for(pattern)
  if pattern.is_a?(Array)
    pattern = pattern.join('|')
  end

  (0..1000000).each do
    while line = $_api_queue.shift
      if line.match(pattern)
        return
      end
    end
    sleep 0.01
  end
end

# Matches regex patterns and returns
# the name of the matching pattern.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Symbol] name of the regex pattern
# @example Match for game text.
#   match = { :retry => [/\.\.\.wait/], :open => [/you open/] }
#   result = match_wait match
#   result #=> :retry or :next
#   case result
#     when :open
#       echo "open!"
#   end
def match_wait(pattern)
  validate_pattern pattern
  $_api_exec_state = :match_wait

  if pattern.has_key?(MATCH_START_KEY)
    wait_for(pattern[MATCH_START_KEY]);
  end

  match_found = false
  match = Hash[pattern.keys.collect{ |v| [v, ""] }]

  rt = 0
  (0..1000000).each do
    while line = $_api_queue.shift
      if !match_found
        match_found = api_find_match match, line, pattern
      end

      rt += api_read_rt line

      if match_found
        $_api_exec_state = :running
        if line.match(/>$/)
          api_sleep rt
          return match.find{ |k, v| !v.empty? }.first
        end
      end
    end
    sleep 0.01
  end
end

# Matches regex patterns and returns pattern name
# along with matched text
#
# @param [Hash] pattern list of regex patterns and names
# @return [Hash] name and text from the matching pattern
# @example Retrieve the text from a matching pattern.
#   put "open my trunk"
#   match = { :m => [/you open/i] }
#   result = match_get match
#   result #=> {:key =>:m, :match => "You open the steel trunk..."}
def match_get(pattern)
  validate_pattern pattern

  $_api_exec_state = :match_get

  if pattern.has_key?(MATCH_START_KEY)
    wait_for(pattern[MATCH_START_KEY]);
  end

  match_found = false
  match = Hash[pattern.keys.collect{ |v| [v, ""] }]

  rt = 0
  (0..1000000).each do
    while line = $_api_queue.shift
      if !match_found
        match_found = api_find_match match, line, pattern
      end

      rt += api_read_rt line

      if match_found
        $_api_exec_state = :running
        if line.match(/>$/)
          api_sleep rt
          m = match.find{ |k, v| !v.empty? }
          return {:key => m[0], :match => m[1]}
        end
      end
    end
    sleep 0.01
  end
end

# Match get for multiple lines.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Hash] found matches arranged by pattern names
# @example Retrieve text from matching patterns.
#    put "health"
#    match = {:vitals => [/Your/],
#             :scars => [/You have/],
#             :match_end => [/>|\.\.\.wait|you may only type ahead/]}
#    result = match_get_m match
#    result #=>
#    #{:vitals=>["Your body feels very beat up.", "Your spirit feels full of life."],
#    # :scars=>["You have some tiny scratches to the neck."], :match_until=>[">"]}
def match_get_m(pattern)
  validate_get_m validate_pattern pattern

  $_api_exec_state = :match_get_m

  if pattern.has_key?(MATCH_START_KEY)
    wait_for(pattern[MATCH_START_KEY]);
  end

  match_found = false
  match = Hash[pattern.keys.collect{ |v| [v, []] }]

  rt = 0
  (0..1000000).each do
    while line = $_api_queue.shift
      if !match_found
        match_found = api_find_match match, line, pattern
      end

      rt += api_read_rt line

      if match_found
        $_api_exec_state = :running
        if line.match(/>$/)
          api_sleep rt
          return match.delete_if{ |k, v| v.empty? }
        end
      end
    end
    sleep 0.01
  end
end

# Matches regex patterns and goes to defined label.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Void]
# @example Using match patterns and jumping to labels on a successful match.
#   labels_start
#
#   label(:retry){
#     match = { :retry => [/\.\.\.wait/], :next => [/you open/] }
#     match_wait_goto match
#   }
#
#   label(:next){
#     echo "next"
#   }
#
#   labels_end
def match_wait_goto(pattern)
  $_api_exec_state = :match_wait_goto

  match_found = false
  match = Hash[pattern.keys.collect{ |v| [v, ""] }]

  rt = 0
  (0..1000000).each do
    while line = $_api_queue.shift
      if !match_found
        match_found = api_find_match match, line, pattern
      end

      rt += api_read_rt line

      if match_found
        $_api_exec_state = :running
        if line.match(/>$/)
          api_sleep rt
          goto match.find{ |k, v| !v.empty? }.first
        end
      end
    end
    sleep 0.01
  end
end

# Sends a command to server.
#
# @param [String] value command.
# @return [void]
def put(value)
  $_api_queue.clear
  puts "put#" + value.to_s
end

# Sends a move command to client and
# waits for navigation message.
#
# @param [String] dir command.
# @return [void]
# @example Using move command in script.
#   move "n"
#   move "e"
#   move "go gate"
def move(dir)
  puts "put#" + dir.to_s

  p = { :room => [/^\{nav\}$/],
        :stand => [/You can't do that while (sitting|kneeling|lying)|must be standing/],
        :retreat => [/if you first retreat|You are engaged|do that while engaged/],
        :wait => [/\.\.\.wait|you may only type ahead/] }

  case match_wait(p)
    when :wait
      pause 0.5
      move dir
    when :stand
      pause 0.5
      put "stand"
      move dir
    when :retreat
      put "retreat"
      put "retreat"
      move dir
  end
end

# Waits for a prompt character.
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
  $_api_queue.clear
  wait_for(/>$/)
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
# @param [Integer, Float] value sleep time in seconds, default value = 1
# @return [Void]
def pause(value = 1)
  sleep value
end

# Load a script by name.
#
# @param [String] name name of the file
# @return [Void]
def load(name)
  Kernel.load "#{Dir.pwd}/scripts/#{name}.rb"
end

# Require a script by name. Only loads file
# once during the script execution.
#
# @param [String] name name of the file
# @return [Void]
def require(name)
  Kernel.require "#{Dir.pwd}/scripts/#{name}.rb"
end

# Current match round time -- can be used in
# secondary threads while main thread is stuck in round time
#
# @param
# @return [Integer] current round time value
def get_match_rt
  $_api_current_rt
end

# @private
def validate_pattern pattern
  if pattern.nil? or pattern.empty?
	  raise "MatchError: match pattern is not specified."
  end
  pattern
end

# @private
def validate_get_m pattern
  unless pattern.has_key?(MATCH_END_KEY)
    raise "MatchError: match pattern does not" +
          "contain '#{MATCH_END_KEY}' end condition."
  end
  pattern
end

# @private
def api_find_match match, line, pattern
  pattern.each_pair do |key, value|
    value.each do |m|
      if line.match(m)
        match[key] << line
        return pattern.has_key?(MATCH_END_KEY) ?
            key == MATCH_END_KEY : true
      end
    end
  end
  false
end

# @private
def api_read_rt line
  line.match(/Roundtime/) ?
      line[/\d+/].to_i + $rt_adjust : 0
end

# @private
def api_terminate_script
  puts "end#"
end

# @private
def exit
  api_terminate_script
  Kernel::exit
end

# @private
def abort
  api_terminate_script
  Kernel::abort
end

# @private
def api_sleep(rt)
  $_api_current_rt = rt
  if rt > 0
    sleep 1
    if $_api_interrupt_time != -1
      rt = rt - $_api_interrupt_time
      $_api_interrupt_time = -1
    end
    api_sleep rt - 1
  end
end

# @private
at_exit do
  if defined? finally_do
    unless @_api_cmd_thread.alive?
      @_api_cmd_thread = Thread.new { CommandThread.new.run }
    end
    finally_do
    api_terminate_script
  end
end

# wait for round time
# before executing script
sleep Rt::value

# run script here
begin
  Kernel.require @_file
rescue Exception => e
  api_terminate_script
  raise e
ensure
  api_terminate_script
end