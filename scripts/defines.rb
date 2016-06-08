module SYSTEM
  class << self
    attr_accessor :finished
  end
  self.finished = false
end

module GLOBAL
  ARRANGE_COUNT = 0
  SKIN = true
end

module COMBAT
  MATCH_DEAD = ["before collapsing", "deflate slightly", "stops all movement", "then grows still",
                "ceases all movement", "collapses into a massive heap","massive heap before",
                "sharp halt", "life force fades away", "slumps dead", "falls dead", "into a heap",
                "collapses over", "and lies still", "falling silent with", "before expiring with",
                "before falling still", "reducing its corpse to nothing", "before growing still", 
		"and collapses to its knees", " leaving behind a bloody mass of scorched robes", 
		"falls limp with a final sputtering sigh"]
  CRITTERS = ["wyvern", "assassin", "magus", "thrall", "archimage", "vindicator", "slayer"]
end

module ITEMS
  ARMOR = ["leathers", "gloves", "balaclava", "shield"]
end

module LOCKSMITH
  class << self
    attr_accessor :current_box
  end

  self.current_box = ""

  BOX_TYPES = ["chest", "trunk", "box", "skippet", "strongbox", "coffer", "crate", "casket", "caddy"]
  DISARMED_BOX_CONTAINER = "haversack"
end