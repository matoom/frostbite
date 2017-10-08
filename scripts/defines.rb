module SYSTEM
  class << self
    attr_accessor :finished
  end
  self.finished = false
end

module GLOBAL
  class << self
    attr_accessor :arrange_count, :skin
  end
  self.arrange_count = 0
  self.skin = true
end

module COMBAT
  MATCH_DEAD = ["before collapsing", "deflate slightly", "stops all movement", "then grows still",
                "ceases all movement", "collapses into a massive heap","massive heap before",
                "sharp halt", "life force fades away", "slumps dead", "falls dead", "into a heap",
                "collapses over", "and lies still", "falling silent with", "before expiring with",
                "reducing its corpse to nothing", "before growing still", "and collapses to its knees",
                " leaving behind a bloody mass of scorched robes", "falls limp with a final sputtering sigh",
                "life within him having been extinguished", "then grows still, its body rapidly cooling",
                "cry of rage and mingled terror before falling still", "one last time before snuffing out forever"]
  DAMAGE = ["light hit", "good hit", "good strike", "solid hit", "hard hit", "strong hit", "heavy strike", "very heavy hit",
            "extremely heavy hit", "powerful strike", "massive strike", "awesome strike", "vicious strike", "earth-shaking strike",
            "demolishing hit", "spine-rattling strike", "devastating hit", "devastating hit (That'll leave a mark!)",
            "overwhelming strike", "obliterating hit", "annihilating strike", "cataclysmic strike", "apocalyptic strike"]
end

module ITEMS
  ARMOR = ["leathers", "gloves", "balaclava", "shield"]
end

module LOCKSMITH
  BOX_TYPES = ["chest", "trunk", "box", "skippet", "strongbox", "coffer", "crate", "casket", "caddy"]
  DISARMED_BOX_CONTAINER = "haversack"
end