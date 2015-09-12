class Util
  def self.auto_target usage_msg
    unless $args.first
      $args << COMBAT::CRITTERS.select{ |critter| Room::objects.include?(critter) }.first
      unless $args.first
        echo usage_msg
        exit
      end
    end
  end
end

module GLOBAL
  ARRANGE_COUNT = 0
  HUNT = true
  SKIN = true
end

module COMBAT
  MATCH_DEAD = ["before collapsing", "deflate slightly", "stops all movement", "then grows still",
                "ceases all movement", "collapses into a massive heap","massive heap before",
                "sharp halt", "life force fades away", "slumps dead", "falls dead", "into a heap",
                "collapses over", "and lies still", "falling silent with", "before expiring with",
                "before falling still", "reducing its corpse to nothing", "before growing still"]
  CRITTERS = ["wyvern", "assassin", "magus", "thrall"]
end