@arguments = []
ARGV.each do |arg|
  @arguments << arg
end
ARGV.clear

@_file = @arguments.shift
@_data_queue = []
@_exp = nil
@_room = nil
@_wield = nil
@_vitals = nil

class Wield
  attr_accessor :left, :left_noun, :right, :right_noun

  def initialize(wield_string)
    process_values(wield_string.chomp)
  end

  def process_values(wield_string)
    wield_values = wield_string.split("|")

    @left = wield_values[0]
    @left_noun = wield_values[1]
    @right = wield_values[2]
    @right_noun = wield_values[3]
  end
end

class Exp
  attr_accessor :rank, :state, :numeric_state, :exp

  def initialize(exp_string)
    process_values(exp_string.chomp)
  end

  def process_values(exp)
    exp_values = exp.split("|")

    @rank = exp_values[1]
    @state = exp_values[3]
    @numeric_state = exp_values[4]
  end
end

class Room
  attr_accessor :description, :objects, :players, :exits

  def initialize(room_string)
    room_values = room_string.chomp.split("|")

    @description = room_values[0]
    @objects = room_values[1]
    @players = room_values[2]
    @exits = room_values[3]
  end

  def count_objects(value)
    @objects.scan(value).length
  end
end

class Vitals
  attr_accessor :health, :concentration, :fatigue, :spirit

  def initialize(vitals_string)
    vitals_values = vitals_string.chomp.split("|")

    @health = vitals_values[0]
    @concentration = vitals_values[1]
    @fatigue = vitals_values[2]
    @spirit = vitals_values[3]
  end
end

#gets.chomp

Thread.new do
  while line = gets
    @_data_queue << line
    if line.start_with? "exp#"
      line.slice! "exp#"
      @_exp = Exp.new(line)
    elsif line.start_with? "vitals#"
      line.slice! "vitals#"
      @_vitals = Vitals.new(line)
    elsif line.start_with? "room#"
      line.slice! "room#"
      @_room = Room.new(line)
    elsif line.start_with? "wield#"
      line.slice! "wield#"
      @_wield = Wield.new(line)
    end
    sleep 0.1
  end
end

def echo(value)
  puts "echo#" + value.to_s
  STDOUT.flush
end

def get_exp(value)
  puts "get_exp#" + value
  STDOUT.flush
  while @_exp.nil?
  end
  exp_obj = @_exp
  @_exp = nil
  exp_obj
end

def get_room
  puts "get_room#"
  STDOUT.flush
  while @_room.nil?
  end
  room_obj = @_room
  @_room = nil
  room_obj
end

def get_wield
  puts "get_wield#"
  STDOUT.flush
  while @_wield.nil?
  end
  wield_obj = @_wield
  @_wield = nil
  wield_obj
end

def get_vitals
  puts "get_vitals#"
  STDOUT.flush
  while @_vitals.nil?
  end
  vitals_obj = @_vitals
  @_vitals = nil
  vitals_obj
end

def put(value)
  puts "put#" + value.to_s
  STDOUT.flush
end

#load script file here
load @_file