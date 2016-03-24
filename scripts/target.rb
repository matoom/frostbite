require "defines"

class Target
  @auto = false

  def self.auto usage_msg
    unless $args.first
      $args << find
      @auto = true
      unless $args.first
        echo usage_msg
        exit
      end
    end
  end

  def self.find
    COMBAT::CRITTERS.select{ |critter| Room::objects.include?(critter) }.first
  end

  def self.is_auto
    @auto
  end

end