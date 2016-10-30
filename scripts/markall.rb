require "csv"

@armory = [{:name => "bascinet", :container => "on knight"}, {:name => "aventail", :container => "on knight"},
{:name => "vambraces", :container => "on knight"}, {:name => "greaves", :container => "on knight"},
{:name => "plate", :container => "on knight"}, {:name => "armet", :container => "on knight"},
{:name => "fauld", :container => "on knight"}, {:name => "gauntlets", :container => "on knight"},
{:name => "helm", :container => "on counter"}, {:name => "balaclava", :container => "on counter"},
{:name => "aventail", :container => "on counter"}, {:name => "vambraces", :container => "on counter"},
{:name => "gloves", :container => "on counter"}, {:name => "tasset", :container => "on counter"},
{:name => "shirt", :container => "on counter"}, {:name => "greaves", :container => "on counter"},
{:name => "hauberk", :container => "on counter"}, {:name => "helm", :container => "on rack"},
{:name => "balaclava", :container => "on rack"}, {:name => "aventail", :container => "on rack"},
{:name => "vambraces", :container => "on rack"}, {:name => "gloves", :container => "on rack"},
{:name => "tasset", :container => "on rack"}, {:name => "shirt", :container => "on rack"},
{:name => "hauberk", :container => "on rack"}, {:name => "greaves", :container => "on rack"},
{:name => "greaves", :container => "in box"}, {:name => "leathers", :container => "in box"},
{:name => "jerkin", :container => "in box"}, {:name => "tasset", :container => "in box"},
{:name => "gloves", :container => "in box"}, {:name => "aventail", :container => "in box"},
{:name => "mask", :container => "in box"}, {:name => "vambraces", :container => "in box"},
{:name => "cowl", :container => "in box"}, {:name => "target shield", :container => "on hooks"},
{:name => "oval shield", :container => "on hooks"}, {:name => "kite shield", :container => "on hooks"},
{:name => "tower shield", :container => "on hooks"}]

@weapons = [{:name => "greathammer", :container => "on table"}, {:name => "sword", :container => "on table"},
{:name => "dagger", :container => "on table"}, {:name => "kris", :container => "on table"},
{:name => "mace", :container => "on table"}, {:name => "war hammer", :container => "on table"},
{:name => "club", :container => "on table"}, {:name => "bludgeon", :container => "on table"},
{:name => "broadsword", :container => "on table"}, {:name => "cutlass", :container => "on table"},
{:name => "scimitar", :container => "on table"}, {:name => "sabre", :container => "on table"},
{:name => "spear", :container => "on counter"}, {:name => "cougar arrows", :container => "on counter"},
{:name => "boar arrows", :container => "on counter"}, {:name => "shortbow", :container => "on counter"},
{:name => "longbow", :container => "on counter"}, {:name => "crossbow", :container => "on counter"},
{:name => "bolts", :container => "on counter"}, {:name => "bola", :container => "on counter"},
{:name => "knuckles", :container => "on bench"}, {:name => "elbow spikes", :container => "on bench"},
{:name => "parry stick", :container => "on bench"}, {:name => "pike", :container => "in bin"},
{:name => "cane", :container => "in bin"}, {:name => "quarterstaff", :container => "in bin"},
{:name => "claymore", :container => "on rack"}, {:name => "greataxe", :container => "on rack"},
{:name => "sledgehammer", :container => "on rack"}, {:name => "halberd", :container => "on rack"},
{:name => "cinquedea", :container => "on rack"}, {:name => "flail", :container => "on rack"}]

@bard = [{:name => "lute"}, {:name => "mandolin"}, {:name => "lyre"}, {:name => "fiddle"}, {:name => "flute"}, {:name => "ocarina"},
{:name => "tin whistle"}, {:name => "tambourine"}, {:name => "bodhran"}, {:name => "bones"}, {:name => "pick"}, {:name => "bow"},
{:name => "tipper"}, {:name => "rag"}, {:name => "cloth"}, {:name => "repair kit"}, {:name => "kit refill"}, {:name => "six gut strings"},
{:name => "four gut strings"}, {:name => "four silk strings"}, {:name => "bodhran skin"}, {:name => "tambourine skin"},
{:name => "medium case"}, {:name => "small case"}, {:name => "silver case"}, {:name => "drum pouch"}, {:name => "long pouch"},
{:name => "nissa's ride"}, {:name => "damaris' kiss"}, {:name => "dark angel"}, {:name => "hamael's song"}]

@bard2 = [{:name => "bow", :container => "on table"}, {:name => "stick", :container => "on table"},
{:name => "maple guiro", :container => "on second table"}, {:name => "oak guiro", :container => "on second table"},
{:name => "rosewood guiro", :container => "on second table"}, {:name => "ebony guiro", :container => "on second table"},
{:name => "violin", :container => "on display"}, {:name => "mandolin", :container => "on display"},
{:name => "harp", :container => "on display"}, {:name => "bodhran", :container => "on second display"},
{:name => "drum", :container => "on second display"}, {:name => "tapani", :container => "on second display"},
{:name => "oboe", :container => "on third display"}, {:name => "flute", :container => "on third display"},
{:name => "horn", :container => "on third display"}]

@locksmith = [{:name => "ordinary lockpick"}, {:name => "stout lockpick"}, {:name => "slim lockpick"}, {:name => "lockpick ring"}]

@bathhouse = [{:name => "bathrobe", :container => "on counter"}, {:name => "scraping reed", :container => "on counter"},
{:name => "soap", :container => "in basin"}, {:name => "first towel", :container => "on rack"},
{:name => "second towel", :container => "on rack"}, {:name => "third towel", :container => "on rack"},
{:name => "fourth towel", :container => "on rack"}, {:name => "hand towel", :container => "on stand"}]

@cleric = [{:name => "chasuble"}, {:name => "clerical collar"}, {:name => "skullcap"}, {:name => "buskins"}, {:name => "sandals"},
{:name => "chalice"}, {:name => "vessel"}, {:name => "basin"}, {:name => "coffer"}, {:name => "silver vial"}, {:name => "silver bell"},
{:name => "sage"}, {:name => "lavender"}, {:name => "sirese flower"}, {:name => "red wine"}, {:name => "incense"},
{:name => "grey-brown feather"}, {:name => "blue-grey feather"}]

@general = [{:name => "linen knapsack"}, {:name => "rugged backpack"}, {:name => "hip tote"}, {:name => "herb bag"}, {:name => "coin purse"},
{:name => "small sack"}, {:name => "large sack"}, {:name => "small box"}, {:name => "large trunk"}, {:name => "envelope"},
{:name => "leather sheathe"}, {:name => "dagger sheathe"}, {:name => "canvas quiver"}, {:name => "leather scabbard"},
{:name => "torch"}, {:name => "stove"}, {:name => "heavy rope"}, {:name => "spade"}, {:name => "trowel"}, {:name => "ice pick"},
{:name => "bundling rope"}, {:name => "dried meat"}, {:name => "acanth bark"}, {:name => "taffelberries"},
{:name => "woven handkerchief"}, {:name => "linen handkerchief"}, {:name => "tinderbox"}, {:name => "flint"}, {:name => "charcoal"},
{:name => "hairbrush"}, {:name => "razor"}, {:name => "clippers"}]

@gems =[{:name => "ring"}, {:name => "band"}, {:name => "pendant"}, {:name => "lavaliere"}, {:name => "necklace"}, {:name => "chain"},
{:name => "choker"}, {:name => "barrette"}, {:name => "circlet"}, {:name => "crown"}, {:name => "hairpin"}, {:name => "bracelet"},
{:name => "armbands"}, {:name => "anklet"}, {:name => "brooch"}, {:name => "clasp"}, {:name => "pin"}, {:name => "earrings"},
{:name => "stud"}, {:name => "wedding ring"}, {:name => "engagement ring"}]

@stitchery =[{:name => "caftan"}, {:name => "dress"}, {:name => "frock"}, {:name => "gown"}, {:name => "robe"}, {:name => "blouse"},
{:name => "smock"}, {:name => "shirt"}, {:name => "tunic"}, {:name => "culottes"}, {:name => "skirt"}, {:name => "flared skirt"},
{:name => "hoop skirt"}, {:name => "tiered skirt"}, {:name => "leggings"}, {:name => "slim pants"}, {:name => "burnoose"},
{:name => "cloak"}, {:name => "hooded cloak"}, {:name => "jacket"}, {:name => "long coat"}, {:name => "shawl"},
{:name => "beret"}, {:name => "bonnet"}, {:name => "cap"}, {:name => "cloche"}, {:name => "hat"}, {:name => "hood"},
{:name => "straw hat"}, {:name => "leather belt"}, {:name => "gloves"}, {:name => "hair bow"}, {:name => "long gloves"},
{:name => "fur muff"}, {:name => "ribbons"}, {:name => "sash"}, {:name => "scarf"}, {:name => "stole"}, {:name => "vest"}]

@shoes = [{:name => "brogans"}, {:name => "field shoes"}, {:name => "moccasins"}, {:name => "pattens"}, {:name => "sandals"},
{:name => "shoes"}, {:name => "slippers"}, {:name => "snowshoes"}, {:name => "wooden clogs"}, {:name => "work shoes"},
{:name => "galoshes"}, {:name => "half boots"}, {:name => "hip boots"}, {:name => "hobnail boots"}, {:name => "jack boots"},
{:name => "riding boots"}, {:name => "thigh boots"}, {:name => "anklets"}, {:name => "boothose"}, {:name => "hose"},
{:name => "knee socks"}, {:name => "silk stockings"}, {:name => "socks"}, {:name => "tights"}]

@florist = [{:name => "Amaryllis"}, {:name => "Carnation"}, {:name => "Chrysanthemum"}, {:name => "Dahlia"}, {:name => "Iris"},
{:name => "Lily"}, {:name => "Orchid"}, {:name => "Poppy"}, {:name => "Rose"}, {:name => "Bouquet"}, {:name => "Wreath"},
{:name => "Crown"}, {:name => "Corsage"}, {:name => "Wristlet"}, {:name => "Boutonniere"}, {:name => "Preservative Potion"}]

@herbs =[{:name => "Jadice Flower"}, {:name => "Plovik Leaf"}, {:name => "Nilos Salve"}, {:name => "Hulnik Grass"}, {:name => "Nemoih Root"},
{:name => "Georin Salve"}, {:name => "Sufil Sap"}, {:name => "Yelith Root"}, {:name => "Ithor Potion"}, {:name => "Muljin Sap"},
{:name => "Junliar Stem"}, {:name => "Blocil Potion"}, {:name => "Riolur Leaf"}]

@alchemy = [{:name => "gloves", :container => "on rack"}, {:name => "goggles", :container => "on rack"},
{:name => "coat", :container => "on rack"}, {:name => "apron", :container => "on rack"},
{:name => "dracon crystals", :container => "in barrel"}, {:name => "oil", :container => "in vats"},
{:name => "grain alcohol", :container => "in vats"}, {:name => "cauldron", :container => "on counter"},
{:name => "stove", :container => "on counter"}, {:name => "toolbox", :container => "on counter"},
{:name => "small jar", :container => "on counter"}, {:name => "large jar", :container => "on counter"},
{:name => "candle mold", :container => "on table"}]

@tannery = [{:name => "first hide scraper", :container => "on counter"}, {:name => "second hide scraper", :container => "on counter"},
{:name => "third hide scraper", :container => "on counter"}, {:name => "fourth hide scraper", :container => "on counter"},
{:name => "kit", :container => "on counter"}]

@artifacts = [{:name => "parchment scroll", :container => "on scroll stand"}, {:name => "lilac scroll", :container => "on scroll stand"},
{:name => "grey scroll", :container => "on scroll stand"}, {:name => "yellow scroll", :container => "on scroll stand"},
{:name => "vellum scroll", :container => "on scroll stand"}, {:name => "off-white scroll", :container => "on scroll stand"},
{:name => "plain scroll", :container => "on scroll stand"}, {:name => "white scroll", :container => "on scroll stand"},
{:name => "fine scroll", :container => "on scroll stand"}, {:name => "ampoule", :container => "on trunk"},
{:name => "leather bag", :container => "on trunk"}, {:name => "statuette", :container => "on trunk"},
{:name => "steel staff", :container => "on trunk"}, {:name => "oaken staff", :container => "on trunk"},
{:name => "alabaster staff", :container => "on trunk"}, {:name => "glass bottle", :container => "on trunk"},
{:name => "inkwell", :container => "on trunk"}, {:name => "reticule", :container => "on trunk"},
{:name => "pouch", :container => "on trunk"}, {:name => "metal jar", :container => "on trunk"},
{:name => "sack", :container => "on trunk"}, {:name => "phial", :container => "on trunk"},
{:name => "packet", :container => "on trunk"}, {:name => "iron box", :container => "on trunk"},
{:name => "glass vial", :container => "on trunk"}, {:name => "urn", :container => "on trunk"},
{:name => "casket", :container => "on trunk"}, {:name => "cambrinth flake", :container => "in bucket"},
{:name => "cambrinth scrap", :container => "in bucket"}, {:name => "cambrinth shaving", :container => "in bucket"},
{:name => "cambrinth shard", :container => "in bucket"}, {:name => "cambrinth sliver", :container => "in bucket"},
{:name => "cambrinth ring", :container => "in bucket"}, {:name => "cambrinth anklet", :container => "in bucket"},
{:name => "cambrinth armband", :container => "in bucket"}, {:name => "cambrinth orb", :container => "in bucket"},
{:name => "talisman", :container => "on table"}]

@haberdashery = [{:name => "Doublet"}, {:name => "Surcoat"}, {:name => "Shirt"},
{:name => "Breeches"}, {:name => "Buckskins"}, {:name => "Braies"}, {:name => "Kilt"}, {:name => "Overalls"}, {:name => "Trousers"},
{:name => "Coat"}, {:name => "Greatcoat"}, {:name => "Jacket"}, {:name => "Capote"}, {:name => "Tabard"}, {:name => "Tail Coat"},
{:name => "Waistcoat"}, {:name => "Beret"}, {:name => "Calotte"}, {:name => "Hat"}, {:name => "Hood"}, {:name => "Peaked Cap"}, 
{:name => "Skullcap"}, {:name => "Tam"}, {:name => "Tricorne"}, {:name => "Ascot"}, {:name => "Belt"}, {:name => "Bowtie"},
{:name => "Cravat"}, {:name => "Cummerbund"}, {:name => "Kid Gloves"}, {:name => "Sash"}, {:name => "Tie"}, {:name => "Vest"}]

@haberdashery2 = [{:name => "shirt", :container => "on mannequin"}, {:name => "vest", :container => "on mannequin"},
{:name => "trousers", :container => "on mannequin"}, {:name => "spats", :container => "on mannequin"}, 
{:name => "blazer", :container => "on mannequin"}, {:name => "fedora", :container => "on mannequin"}]

shops = {:armory => {:room => 192, :items => @armory},
         :weapons => {:room => 191, :items => @weapons},
         :bard => {:room => 404, :items => @bard},
         :bard2 => {:room => 783, :items => @bard2},
         :locksmith => {:room => 218, :items => @locksmith},
         :bathhouse => {:room => 325, :items => @bathhouse},
         :cleric => {:room => 411, :items => @cleric},
         :general => {:room => 189, :items => @general},
         :gems => {:room => 372, :items => @gems},
         :stitchery => {:room => 439, :items => @stitchery},
         :shoes => {:room => 440, :items => @shoes},
         :florist => {:room => 661, :items => @florist},
         :herbs => {:room => 219, :items => @herbs},
         :alchemy => {:room => 226, :items => @alchemy},
         :tannery => {:room => 222, :items => @tannery},
         :artifacts => {:room => 223, :items => @artifacts},
         :haberdashery => {:room => 710, :items => @haberdashery},
         :haberdashery2 => {:room => 712, :items => @haberdashery2}}

def grab e
  case e
    when /it would be taking candy from a baby/
      $grab = "+6"
    when /it would be stealable even with your eyes closed/
      $grab = "+5"
    when /it should be trivial for one of your skills/
      $grab = "+4"
    when /it should be an easy target for you/
      $grab = "+3"
    when /it should not prove difficult for you/
      $grab = "+2"
    when /it should be more likely liftable than not/
      $grab = "+1"
    when /it should be about even odds/
      $grab = "0"
    when /it may give you some difficulty to lift/
      $grab = "-1"
    when /it may be troublesome to lift/
      $grab = "-2"
    when /you don't think well of your chances to lift it/
      $grab = "-3"
    when /it may be quite the struggle to nab/
      $grab = "-4"
    when /it would likely be futile to bother/
      $grab = "-5"
    when /you can already feel the taste of bitter failure/
      $grab = "-6"
    else
      $grab = "-999"
  end
end

def away e
  case e
    when /nobody will ever miss it/
      $away = "+6"
    when /you could just waltz on out with it/
      $away = "+5"
    when /it should be trivial to avoid attention/
      $away = "+4"
    when /it should be easy to avoid attention/
      $away = "+3"
    when /it should not prove too difficult/
      $away = "+2"
    when /it should be possible/
      $away = "+1"
    when /it's about even odds/
      $away = "0"
    when /perhaps it's a little risky/
      $away = "-1"
    when /it'll be troublesome to be unnoticed/
      $away = "-2"
    when /it's somewhat of a long shot/
      $away = "-3"
    when /you're pretty sure you'll be caught/
      $away = "-4"
    when /the shopkeep is paying far too much attention to it/
      $away = "-5"
    when /the heavy \*THUD\* of a judge's gavel echoes through your mind/
      $away = "-6"
    else
      $away = "-999"
  end
end

def conf e
  case e
    when /You feel highly confident in the accuracy of your judgement/
      $conf = "4"
    when /You feel somewhat confident in the accuracy of your judgement/
      $conf = "3"
    when /You are somewhat uncertain of the accuracy of your judgement/
      $conf = "2"
    when /You are highly uncertain of the accuracy of your judgement/
      $conf = "1"
    when /You have no idea what you're doing and feel like you just made wild guesses/
      $conf = "0"
    else
      $conf = "-999"
  end
end

def item e
  $item = /^You begin to carefully size up your chances at nabbing (a|an|some) (.*)\./.match(e).captures[1]
end

def go_to_shop room
  $args.clear << room
  load "go.rb"
end

def mark item
  put "mark #{item}"
  match_wait({ :wait => [/\.\.\.wait|Mark what|can not make marks|Roundtime/]})
end

def rank skill
  put "exp #{skill}"
  result = match_get({ :wait => [/\.\.\.wait/], :exp => [/#{skill}/i], :not_found => ["EXP HELP"]})
  case result[:key]
    when :wait, :fail
      pause 0.5
      rank skill
    when :exp
      return result[:match][/\d+/].to_i
  end
end

Observer.instance.register_event({ :item => /^^You begin to carefully size up/ })
Observer.instance.register_event({ :grab => /^Considering actually grabbing/ })
Observer.instance.register_event({ :away => /^Regarding getting away/ })
Observer.instance.register_event({ :conf => /^You (are|feel|have) (highly|somewhat|no idea).*(judgement|guesses)/ })

thievery = rank "thievery"
appraisal = rank "appraisal"

CSV.open("markall.csv", "wb") do |csv|
  shops.each do |name, shop|
    echo "going #{shop[:room]}"
    go_to_shop shop[:room]

    shop[:items].each do |item|
      $grab, $away, $conf, $item = "-999", "-999", "-999", "-"
      mark "#{item[:name]} #{item[:container]}"
      echo "#{Room::title};#{$item};#{$grab};#{$away};#{$conf};Thievery: #{thievery};Appraisal: #{appraisal}"
      csv << [Room::title, $item, $grab, $away, $conf, "Thievery: #{thievery}", "Appraisal: #{appraisal}"]
    end
  end
end