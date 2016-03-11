require "defines"

class Combat
  @auto = false

  def self.auto_target usage_msg
    unless $args.first
      $args << find_target
      @auto = true
      unless $args.first
        echo usage_msg
        exit
      end
    end
  end

  def self.find_target
    COMBAT::CRITTERS.select{ |critter| Room::objects.include?(critter) }.first
  end

  def self.auto
    @auto
  end

end