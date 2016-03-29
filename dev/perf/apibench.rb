require 'socket'
require 'dl'
require 'dl/import'
require 'benchmark'

start = Time.now

module ApiSettings
  def self.port
    File.open("api.ini", 'r') do |inFile|
      inFile.each_line do |line|
        return line.partition('=').last.to_i if line.start_with? "port"
      end
    end
  end
end

BasicSocket.do_not_reverse_lookup = true
$_api_socket = TCPSocket.open('127.0.0.1', ApiSettings::port)
$_api_socket.setsockopt(Socket::IPPROTO_TCP, Socket::TCP_NODELAY, 1)
finish = Time.now

puts finish - start

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
    $_api_socket.gets('\0').chomp('\0').to_s.split(", ")
  end
end

# @api private
module GameData
  extend DL::Importer
  dlload 'shared.dll'

  extern 'char* getContainer()'
  extern 'int getRt()'
end

n = 50000
Benchmark.bm do |x|
  x.report("Socket RT") {
    n.times do Rt::value end
  }
  x.report("Socket Container") {
    n.times do Container::list end
  }
end