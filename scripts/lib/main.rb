$LOAD_PATH.unshift(File.dirname(__FILE__))
$LOAD_PATH.unshift("#{File.dirname(__FILE__)}/../")

require "models"
require "ruby_goto"
require "observer"
require "text"

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

$_api_gets_mutex = Mutex.new
$_api_exec_mutex = Mutex.new

$_api_interrupt_time = -1
$_api_exec_state = :none
$_api_observer_started = false
$_api_current_rt = 0

$rt_adjust = 0

# @private
module API
  #constants
  AUTH_HOST = "eaccess.play.net"
  AUTH_PORT = "7900"
  AUTH_GAMES = {:prime => 'DR', :test => 'DRT', :plat => 'DRX', :fallen => 'DRF'}

  API_PUT_PREFIX = "put#"
  API_ECHO_PREFIX = "echo#"
  API_CMD_SUFFIX = "\n"

  MATCH_START_KEY = :match_start
  MATCH_END_KEY = :match_end

  TEXT_PREFIX = "game_text#"
  EXIT_PREFIX = "exit#"

  API_ADR = '127.0.0.1'

  class CommThread
    def run
      while line = gets
        case
          when line.start_with?(API::TEXT_PREFIX)
            write line[API::TEXT_PREFIX.size, line.size]
          when line.start_with?(API::EXIT_PREFIX)
            Kernel::abort
        end
        sleep 0.01
      end
    end

    def write(line)
      $_api_gets_mutex.synchronize do
        $_api_queue << line
        if $_api_observer_started
          $_api_observer_queue << line
        end
      end
    end
  end

  module Client
    def self.api_port
      File.open("#{File.dirname(__FILE__)}/../../api.ini", 'r') do |inFile|
        inFile.each_line do |line|
          return line.partition('=').last.to_i if line.start_with? "port"
        end
      end
    end

    def self.init
      $_api_socket = TCPSocket.open(API::API_ADR, Client::api_port)
      $_api_socket.setsockopt(Socket::IPPROTO_TCP, Socket::TCP_NODELAY, 1)
    end
  end

  def self.sync_read
    $_api_gets_mutex.synchronize do
      $_api_queue.shift
    end
  end

  def self.sync_clear
    $_api_gets_mutex.synchronize do
      $_api_queue.clear
    end
  end

  def self.sleep_rt(rt)
    rt = sync_rt rt
    if rt > 0
      sleep 1
      sleep_rt rt - 1
    end
  end

  def self.sync_rt(rt)
    $_api_current_rt = rt
    if $_api_interrupt_time != -1
      rt = [0, rt - $_api_interrupt_time].max
      $_api_interrupt_time = -1
    end
    rt
  end
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
  puts "#{API::API_PUT_PREFIX}#{dir.to_s}#{API::API_CMD_SUFFIX}"
  case match_wait({ :room => [/^\{nav\}$/],
                    :stand => [/do that while (sitting|kneeling|lying)|must be standing|cannot manage to stand/],
                    :retreat => [/if you first retreat|You are engaged|do that while engaged/],
                    :wait => [/\.\.\.wait|you may only type ahead/] })
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

# Sends a command to server.
#
# @param [String] value command.
# @return [void]
def put(value)
  $_api_gets_mutex.synchronize do
    $_api_queue.clear
    puts "#{API::API_PUT_PREFIX}#{value.to_s}#{API::API_CMD_SUFFIX}"
  end
end

# Sends a message to client main window.
#
# @param [#to_s] value message.
# @return [void]
# @example Echoing into client main window.
#   echo "hello"
#   echo 1
def echo(value)
  $_api_gets_mutex.synchronize do
    puts "#{API::API_ECHO_PREFIX}#{value.to_s}#{API::API_CMD_SUFFIX}"
  end
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
  Kernel.load name
end

# Require a script by name. Only loads file
# once during the script execution.
#
# @param [String] name name of the file
# @return [Void]
def require(name)
  Kernel.require name
end

# Current match round time -- can be used in
# secondary threads while main thread is stuck in round time
#
# @return [Integer] current round time value
def get_match_rt
  $_api_current_rt
end

# @deprecated Please use {#pause_rt} instead
# Pauses during rt and accounts for interrupted time
#
# @return [void]
def pause_for_roundtime
  pause_rt
end

# Pauses during rt and accounts for any interrupted time
#
# @return [void]
def pause_rt
  if $_api_current_rt > 0
    API::sleep_rt $_api_current_rt + $rt_adjust
  end
end

# @private
def exit
  Kernel::exit
end

# @private
def abort
  Kernel::abort
end

# @private
at_exit do
  if defined? finally_do
    unless @_api_comm_thread.alive?
      @_api_comm_thread = Thread.new { API::CommThread.new.run }
    end
    finally_do
  end
end

# init IPC
API::Client::init

# wait for round time
# before executing script
sleep Rt::value

# run script here
begin
  @_api_comm_thread = Thread.new { API::CommThread.new.run }
  Kernel.require @_file
rescue Exception => e
  raise e
end