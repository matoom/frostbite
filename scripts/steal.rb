# desc: stealing in zouluren
# requirements: thieves only, 250? first aid
# run: in front of crossing bank
# recommend on first run use @mark = true and @debug_mode = true
# 840

@containers = ["backpack", "haversack"]
@khri = "khri start calm focus guile hasten darken plunder"
@mark = false
@debug_mode = false
@name = "Defol"
@pawn_threshold = 1

@crossing_items =
    {
      :bathhouse => { :item  => :none, :amount => 2 }, #[Orem's Bathhouse, Lobby]
      :locksmith => { :item => :none, :amount => 2 }, #[Ragge's Locksmithing, Salesroom] -- ring (trivial 741)
      :bard => { :item => "wyndewood fiddle", :amount => 1 }, #[The True Bard D'Or, Fine Instruments]
      :bard_private => { :item => "horn", :amount => 1 }, #[Luthier's, Private Showroom]
      :armor => { :item => "ring mail", :amount => 3 }, #[Tembeg's Armory, Salesroom] -> Chain Lorica
      :weapon => { :item => "heavy crossbow", :amount => 4 }, #[Milgrym's Weapons, Showroom]
      :jewelry => { :item => "platinum engagement ring", :amount => 1 }, #[Grisgonda's Gems and Jewels]
      :macipur => { :item => :none, :amount => 4 }, #[Marcipur's Stitchery, Workshop]  -- gold brocade long coat (trivial 809)
      :brisson => { :item => :none, :amount => 3 }, #[Brisson's Haberdashery, Sales Salon] -- gold brocade tail coat (trivial 670)
      :artificer => { :item => :none, :amount => 2 }, #[Herilo's Artifacts, Showroom] -- reticule (trivial 826)
      :tannery => { :item => :none, :amount => 2 }, #[Falken's Tannery, Supply Room]
      :alchemy => { :item => "bucket", :amount => 2 }, #[Chizili's Alchemical Goods, Salesroom]
      :emmiline_pantry => { :item => "necklace", :amount => 1 }, #[Emmiline's Cottage, Pantry]
      :emmiline_sales => { :item => "scimitar", :location => "on display", :amount => 1 }, #[Emmiline's Cottage, Sales Floor]
      :emmiline_parlor => { :item => "chart", :location => "on hook", :desc => "Rock Troll anatomy", :amount => 1 } #[Emmiline's Cottage, Parlor]
    }

@arthe_items =
    {
      :thread => { :item  => :none, :amount => 2 }, #[Quellia's Thread Shop, Sales Room]
      :odds => { :item  => "hat", :amount => 2 }, #[Odds 'n Ends, Sales Room]
      :bardic => { :item  => :none, :amount => 2 }, #[Barley Bulrush, Bardic Ballads]
      :bobba => { :item  => "ring mail", :amount => 3 }, #[Bobba's Arms and Armor]
      :lobby => { :item  => "pipe", :location => "in chest", :amount => 1 } #[Yulugri Wala, Lobby]
    }

@leth_items =
    {
      :alberdeen => { :item  => "tunic", :amount => 1, :alt => {:item => "arm pouch", :amount => 2}}, #[Alberdeen's Meats and Provisions, Front Room]
      :yerui => { :item  => "model tree", :amount => 1 }, #[Yerui's Woodcraft, Workshop]  -> ironwood staff
      :ongadine => { :item  => :none, :amount => 3 }, #[Ongadine's Garb and Gear] -- ebony silk mantle (trivial 721)
      :bardic_leth => { :item  => "Golden-hued hat", :amount => 2 }, #[Sinjian's Bardic Requisites, Workshop]
      :origami => { :item  => "case", :amount => 1, :location => "on glass shelves", :desc => "fine china origami case" }, #[Origami Boutique]
      :trueflight => { :item  => "heavy crossbow", :amount => 4, :bin => false }, #[Huyelm's Trueflight Bow and Arrow Shop, Salesroom]
      :shack => { :item  => "koummya", :amount => 1 } #[Leth Deriel, Wooden Shack]
    }

@ilaya_pier_items = [{:name => "flask", :amount => 3},
                     {:name => "skirt", :desc => "green velvet skirt", :amount => 1},
                     {:name => "skirt", :desc => "bias-cut jade silk skirt", :amount => 1},
                     {:name => "bush", :amount => 1},
                     {:name => "chimney", :amount => 1},
                     {:name => "polished broadsword", :amount => 1},
                     {:name => "ring", :desc => "delicate gold rose ring", :amount => 1},
                     {:name => "velvet skirt", :amount => 1},
                     {:name => "ring", :desc => "copper ring shaped like a pair of clasped", :amount => 2},
                     {:name => "ring", :desc => "burnished copper ring set with an amber", :amount => 1},
                     #{:name => "pendant", :desc =>"carved coral cameo pendant depicting a female", :amount => 2}, # trivial < 840
                     {:name => "pendant", :desc => "pendant of a Dwarven battle axe", :amount => 2},
                     {:name => "moonstone lily", :amount => 1},
                     {:name => "corset", :desc => "inset with petal-pink panels", :amount => 1},
                     {:name => "axe", :desc => "with a gnarled oak haft", :amount => 1},
                     {:name => "golden earrings", :amount => 1},
                     {:name => "riding cloak", :amount => 1},
                     {:name => "jar", :desc => "marble jar with a carved amethyst", :amount => 1},
                     {:name => "carnelian jar", :amount => 1},
                     {:name => "vial", :desc => "jade glass vial", :amount => 2},
                     {:name => "moonflower", :amount => 1},
                     {:name => "sapphire lily", :amount => 1},
                     {:name => "bodice", :desc => "stitched with tiny violet lilies", :amount => 1},
                     {:name => "white pearls", :amount => 1},
                     #{:name => "scraper", :desc => "scraper set with cabochon sunstones", :amount => 2}, # trivial 765
                     #{:name => "stole", :desc => "lavender linsey-woolsey stole", :amount => 2}, # trivial 760
                     {:name => "stole", :desc => "cobwebby silver lace", :amount => 1},
                     #{:name => "brass bowl", :amount => 2}, # trivial < 815
                     {:name => "bracelet", :desc => "set with heart-shaped diamonds", :amount => 1},
                     {:name => "unicorn pin", :amount => 1},
                     {:name => "sabre", :amount => 1},
                     {:name => "spidersilk sack", :amount => 1},
                     {:name => "silver bracelet", :amount => 1},
                     {:name => "bronze cauldron", :amount => 1, :dump => true}]

@ilaya_items =
    {
        :tower => {:item => "scimitar", :location => "on rack", :amount => 1, :alt =>
            {:item => "scimitar", :location => "on second rack", :amount => 1}}, #[Harbor Tower, First Floor]
        :fish => {:item => "tank", :amount => 1}, #[Fernwyk's Fish]
        :fishmonger => {:item => :none, :amount => 1}, #[Ilaya Taipa, Fishmonger's Stall]
        :pearls => {:item => "thumb ring", :amount => 1}, #[Pischic's Pearls]
        :clothing => {:item => "moonsilk fabric", :amount => 1}, #[Anyaila's Fine Clothing, Sales Floor]
        :stuff => {:item => "pottery lamp", :amount => 1}, #[Krimand's House of Stuff]
        # piers
        :backfence_gossip => {:items => @ilaya_pier_items},
        :blood_bane => {:items => @ilaya_pier_items},
        :bloody_barnacle => {:items => [], :amount => 0}, #furniture
        :ninth_life => {:items => @ilaya_pier_items},
        :dark_nighttrawler => {:items => @ilaya_pier_items},
        :drunken_sage => {:items => [], :amount => 0}, #food
        :dusktide_rising => {:items => @ilaya_pier_items},
        :winged_duck => {:items => @ilaya_pier_items},
        :fleetwing_gull => {:items => [], :amount => 0}, #food
        :golden_apple => {:items => [], :amount => 0}, #furniture
        :harper_song => {:items => @ilaya_pier_items},
        :marsh_skipper => {:items => @ilaya_pier_items},
        :merelew_wench => {:items => [], :amount => 0}, #food
        :mermaid_fall => {:items => @ilaya_pier_items},
        :moveable_feast => {:items => @ilaya_pier_items},
        :night_sky_hair => {:items => @ilaya_pier_items},
        :north_wind_skimmer => {:items => @ilaya_pier_items},
        :paper_lion => {:items => @ilaya_pier_items},
        :river_dreamer => {:items => @ilaya_pier_items},
        :rusty_barnacle => {:items => @ilaya_pier_items},
        :spinning_jenny => {:items => [], :amount => 0}, #furniture
        :talking_salmon => {:items => [], :amount => 0}, #food
        :thornberry_dart => {:items => [], :amount => 0}, #food
        :tipsy_barmaid => {:items => [], :amount => 0}, #food
        :tropic_night => {:items => @ilaya_pier_items},
        :wavecrester => {:items => @ilaya_pier_items},
        :weaving_minstrel => {:items => [], :amount => 0}, #furniture
        :whitehaven_hope => {:items => [], :amount => 0} #food
    }

@pier_shops =
    {
        :backfence_gossip => "Backfence Gossip", :blood_bane => "Blood Bane",
        :bloody_barnacle => "Bloody Barnacle", :ninth_life => "Cat's Ninth Life",
        :dark_nighttrawler => "Dark Nighttrawler", :drunken_sage => "Drunken Sage",
        :dusktide_rising => "Dusktide Rising", :winged_duck => "Dusk-winged Duck",
        :fleetwing_gull => "Fleetwing Gull", :golden_apple => "Golden Apple",
        :harper_song => "Harper's Song", :marsh_skipper => "Marsh Skipper",
        :merelew_wench => "Merelew Wench", :mermaid_fall => "Mermaid's Fall",
        :moveable_feast => "Movable Feast", :night_sky_hair => "Night Sky's Hair",
        :north_wind_skimmer => "North Wind's Skimmer", :paper_lion => "Paper Lion",
        :river_dreamer => "River Dreamer", :rusty_barnacle => "Rusty Barnacle",
        :spinning_jenny => "Spinning Jenny", :talking_salmon => "Talking Salmon",
        :thornberry_dart => "Thornberry Dart", :tipsy_barmaid => "Tipsy Barmaid",
        :tropic_night => "Tropic Night", :wavecrester => "Wavecrester",
        :weaving_minstrel => " Weaving Minstrel", :whitehaven_hope => "Whitehaven Hope"
    }

@current_container = 0
@fails = 0
@exp_before = 0
@stolen_items = []
@leave = false
@dump = false
@bin = false

@ordinal_numbers = ["first", "second", "third", "fourth", "fifth", "sixth", "seventh", "eighth",
                    "ninth", "tenth", "eleventh", "twelfth", "thirteenth", "fourteenth", "fifteenth", "sixteenth"]

@valid_pier_containers = ["crate", "box", "barrel", "trunk", "coffer", "case", "chest",
                          "tub", "basket", "bucket", "bin", "display", "tub"]

@body_parts = ["right leg", "left leg", "abdomen", "back", "chest", "right arm", "left arm",
               "right hand", "left hand", "neck", "head", "right eye", "left eye"]

def jail_check
  room = Room::title
  if room == "[Gallows Tree, Cell]" or room == "[Guard House, Jail Cell]"
    echo "*** Jailed! ***"
    echo @stolen_items.inspect
    exit
  end
end

#override
undef :move
def move(value)
  puts "put#" + value.to_s
  res = match_wait({ :room => [/^\{nav\}$/],
                     :wait => [/\.\.\.wait/, /you may only type ahead/],
                     :lost => [/can't go there|were you referring/],
                     :retreat => [/You'll have better luck if you first retreat|You are engaged|do that while engaged/],
                     :leave => [/You stop as you realize|is locked|You realize the shop is closed|You smash your nose/] })
  @leave = false
  case res
    when :wait
      pause 0.5
      move value
    when :lost
      jail_check
    when :leave
      @leave = true
    when :retreat
      put "retreat"
      put "retreat"
      move value
  end
end

def drop name
  put "drop my #{name}"
  wait
end

def stow_item name
  if (@current_container < @containers.size)
    put "put my #{name} in my #{@containers[@current_container]}"
    match = { :wait => [/\.\.\.wait|You silently slip out|appears different about/],
              :full => [/any more room|no matter how you|to fit in the/],
              :continue => [/You put|Perhaps you should|into your/] }
    result = match_wait match

    case result
      when :wait
        pause 0.5
        stow_item name
      when :full
        @current_container = @current_container + 1
        stow_item name
      when :continue
        @stolen_items << [name, @containers[@current_container], @bin]
    end
  else
    drop name
  end
end

def stow_items
  left_hand = Wield::left_noun
  right_hand = Wield::right_noun

  if left_hand != ""
    @dump ? drop(left_hand) : stow_item(left_hand)
  end

  if right_hand != ""
    @dump ? drop(right_hand) : stow_item(right_hand)
  end
end

def do_hide
  if !Status::hidden
    put "hide"
    match = { :wait => [/\.\.\.wait/],
              :continue => [/You melt|You slip|You blend|But you|ruining your|Behind what|You look around/] }
    result = match_wait match

    case result
      when :wait
        pause 0.5
        do_hide
    end
  end
end

def take item
  put "steal #{item}"
  match = { :wait => [/\.\.\.wait|appears different about/],
            :fail => [/Guards!|begins to shout/],
            :alt => [/can't steal that|You haven't picked/],
            :stow => [/You need at least one/],
            :continue => [/Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      take item
    when :stow
      stow_items
      do_hide
      take item
    when :fail
      echo "Failed attempt recorded! - total fails: #{@fails += 1}"
      raise result.to_s
    when :alt
      raise result.to_s
  end
end

def get_item_position item, location, item_desc
  put "look #{location}"
  match = { :wait => ["ahead 1 command"],
            :contents => ["On the", "In the"] }
  result = match_get match

  case result[:key]
    when :wait
      get_item_position item, location, item_desc
    when :contents
      item_position result[:match], item, item_desc
  end
end

def item_position list, item_name, item_desc
  items = list.split(/,|\band\b/).select{|item| item.include? item_name}
  items.each_with_index do |item, index|
    if item.include?(item_desc)
      return @ordinal_numbers[index]
    end
  end
end

def mark item
  put "mark #{item}"
  match_wait({ :wait => [/\.\.\.wait|Mark what|Roundtime/] })
end

def register_exp
  @exp_before = Exp::state "thievery"
end

def report_exp_gain
  exp_after = Exp::state "thievery"
  exp_gain = exp_after - @exp_before

  echo "<br/>Thievery: #{exp_after} [#{exp_gain > 0 ? "+" : ""}#{exp_gain}]<br/>"
end

def steal item, amount
  begin
    do_hide
    register_exp

    amount.times do |x|
      take item
      if x % 2 == 1 and amount != x + 1
        stow_items
        do_hide
      end
    end
  rescue Exception => e
    return e.message.to_sym
  ensure
    report_exp_gain
    stow_items
  end
end

def steal_shop shop_attrs
  if shop_attrs[:item] != :none and !@leave
    item = get_item shop_attrs

    # set extra options for items
    set_options shop_attrs

    if @debug_mode
      echo "*** #{item} => #{shop_attrs[:amount]} ***"
      echo "opts => dump: #{@dump}, bin => #{@bin}"
    end

    if @mark
      mark item
    end

    if !@debug_mode
      case steal item, shop_attrs[:amount]
        when :alt
          if shop_attrs.has_key?(:alt)
            steal_shop shop_attrs[:alt]
          end
      end
    end
  end
end

def get_item attrs
  item = attrs[:item]

  if attrs.has_key?(:location) and attrs.has_key?(:desc)
    pos = get_item_position item, attrs[:location], attrs[:desc]
    item = "#{pos} #{item} #{attrs[:location]}"
  elsif attrs.has_key?(:location)
    item = "#{item} #{attrs[:location]}"
  end

  item
end

def set_options list
  @dump = list.has_key?(:dump) ? list[:dump] : false
  @bin = list.has_key?(:bin) ? list[:bin] : false
end

def docked_shop shops
  shops.each do |k,v|
    if Room::objects.include? v
      return k
    end
  end
  :none
end

def steal_pier
  if !@leave
    shop = docked_shop @pier_shops
    if @ilaya_items[shop] and @ilaya_items[shop].has_key?(:items) and
        !@ilaya_items[shop][:items].empty?

      if @debug_mode
        echo "*** #{@pier_shops[shop]} ***"
        echo "opts => dump: #{@dump}"
        echo find_pier_item shop
      else
        echo "*** #{@pier_shops[shop]} ***"
        item = find_pier_item shop
        if !item.empty?
          steal item[:name], item[:amount]
        end
      end
    end
  end
end

def find_pier_item shop
  containers = Room::objects.split(/,|\band\b/).collect { |s| s.split.last.delete('.') }

  # map containers
  end_index = containers.length
  containers.each_with_index do |container, start_index|
    count = 1
    (start_index + 1..end_index).each do |index|
      if container == containers[index]
        containers[index] = "#{@ordinal_numbers[count]} #{container}"
        count = count + 1
      end
    end
  end

  # find items in containers
  containers.each do |container|
    item = find_stealable_item shop, container
    if !item.empty?
      return item
    end
  end
  return {}
end

def find_stealable_item shop, container
  if @valid_pier_containers.any? { |word| container.include?(word) }
    put "look in #{container}"
    match = { :contents => ["In the"],
              :redo => ["ahead 1 command"],
              :not_found => ["I could not find"] }
    result = match_get match

    case result[:key]
      when :contents
        return get_item_description shop, container, result[:match]
      when :redo
        find_stealable_item shop, container
    end
  end
  return {}
end

def get_item_description shop, container, contents
  @ilaya_items[shop][:items].each do |item|

    # set extra options for items
    set_options item

    if item.has_key?(:desc) and contents.include?(item[:desc])
      return { :name => "#{item_position contents, item[:name], item[:desc]} #{item[:name]} in #{container}",
               :amount => item[:amount] }
    elsif !item.has_key?(:desc) and contents.include?(item[:name])
      return { :name => "#{item[:name]} in #{container}",
               :amount => item[:amount] }
    end
  end
  return {}
end

def check_for_mites
  put "health"
  match = { :wait => [/\.\.\.wait|you may only type ahead/],
            :tend => [/red blood mite/],
            :continue => [/>/] }
  result = match_get match

  case result[:key]
    when :wait
      pause 0.5
      check_for_mites
    when :tend
      echo "*** Found MITES! ***"
      echo result[:match]
      tend result[:match]
      check_for_mites
  end
end

def tend wounds
  @body_parts.each do |area|
    if wounds.include? area
      put "tend my #{area}"
      wait_for_roundtime
    end
  end
end

def prepare_khri
  put @khri
  match = { :wait => [/\.\.\.wait/],
            :exit => [/enough to manage that|already using/],
            :continue => [/Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      prepare
    when :exit
      echo "*** Unable to start Khri! ***"
      exit
  end
end

def prepare_containers
  @containers.each do |container|
    put "open my #{container}"
    wait
  end
end

def prepare_hands
  if Wield::left_noun != ""
    put "stow left"
  end

  if Wield::right_noun != ""
    put "stow right"
  end
end


def prepare_armor
  put "inv armor"
  match = { :wait => [/\.\.\.wait/],
            :exit => [/INVENTORY HELP/],
            :continue => [/aren't wearing anything like/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      prepare_armor
    when :exit
      echo "*** Wearing armor! ***"
      exit
  end
end

def pawn_items
  echo "Total failed attempts recorded - #{@fails}"

  if @stolen_items.count == 0 or @fails > @pawn_threshold
    echo "Binning everything!"
    return
  end

  move "go shop"
  echo @stolen_items.inspect

  skip = ""
  @stolen_items.each_with_index do |item, i|
    if item.at(1)
      if item.at(0) == skip or item.at(2)
        next
      end
      pause 0.1
      put "get #{item.at(0)} from my #{item.at(1)}"
      wait
      put "sell my #{item.at(0)}"
      match = { :continue => ["referring to"],
                :sell => ["he hands you"],
                :redo => ["only type ahead 1 command"],
                :no_sell => ["isn't worth my time", "can't pawn"] }

      case match_wait match
        when :redo
          redo
        when :no_sell
          skip = item.at(0)
          put "put my #{item.at(0)} in my #{item.at(1)}"
          wait
        when :sell
          @stolen_items[i][1] = :pawned
          skip = ""
      end
    end
  end

  move "out"
end

def bin_items
  echo @stolen_items.inspect

  @stolen_items.each do |item|
    if item.at(1) != :pawned
      pause 0.1
      put "get #{item.at(0)} from my #{item.at(1)}"
      wait
      put "put #{item.at(0)} in bin"
      match = { :continue => ["#{@name}", "were you referring"],
                :redo => ["only type ahead 1 command"] }

      case match_wait match
        when :redo
          redo
      end
    end
  end
end

if Room::title != "[The Crossing, Hodierna Way]"
  echo "*** Need to be in front of Crossing bank! ***"
  exit
end

#prepare
unless @debug_mode
  prepare_containers

  prepare_hands

  prepare_armor

  prepare_khri
end

#Crossing

move "nw"
move "w"
move "w"
move "w"
move "go bath"

steal_shop @crossing_items[:bathhouse]

move "out"
move "w"
move "w"
move "n"
move "n"
move "go door"

steal_shop @crossing_items[:locksmith]

move "out"
move "n"
move "e"
move "e"
move "e"
move "go shop"

steal_shop @crossing_items[:bard]

move "go curtain"

steal_shop @crossing_items[:bard_private]

move "go curtain"
move "out"
move "e"
move "e"
move "n"
move "go shop"

steal_shop @crossing_items[:armor]

move "out"
move "e"
move "go shop"

steal_shop @crossing_items[:weapon]

move "out"
move "s"
move "s"
move "e"
move "e"
move "go shop"

steal_shop @crossing_items[:jewelry]

move "out"
move "n"
move "e"
move "go shop"

steal_shop @crossing_items[:macipur]

move "out"
move "w"
move "s"
move "s"
move "s"
move "w"
move "sw"
move "go bridge"
move "n"
move "n"
move "w"
move "nw"
move "w"
move "w"
move "w"
move "n"
move "go haberdashery"

steal_shop @crossing_items[:brisson]

move "out"
move "n"
move "w"
move "go shop"

steal_shop @crossing_items[:artificer]

move "out"
move "e"
move "n"
move "n"
move "ne"
move "nw"
move "n"
move "e"
move "e"
move "e"
move "n"
move "n"
move "w"
move "go shop"
move "w"
move "w"

steal_shop @crossing_items[:tannery]

move "e"
move "e"
move "out"
move "e"
move "s"
move "e"
move "e"
move "n"
move "n"
move "e"
move "s"
move "go shop"

steal_shop @crossing_items[:alchemy]

move "out"
move "e"
move "go gate"
move "n"
move "n"
move "e"
move "e"
move "go path"
move "go step"
move "go door"
move "go arch"

steal_shop @crossing_items[:emmiline_pantry]

move "go arch"
move "w"

steal_shop @crossing_items[:emmiline_parlor]

move "e"

steal_shop @crossing_items[:emmiline_sales]

move "go door"
move "go step"
move "go path"
move "w"
move "w"
move "s"
move "s"
move "go gate"
move "s"
move "e"
move "e"
move "e"
move "n"
move "n"
move "e"
move "e"
move "go gate"

#Arthe
xing_to_arthe = ["n", "n", "n", "ne", "ne", "n", "nw", "nw", "n", "n", "ne",
                 "nw", "n", "n", "e", "down", "down", "go gate", "n", "n", "n", "n", "e"]

xing_to_arthe.each do |dir|
  move dir
end

move "go door"

steal_shop @arthe_items[:thread]

move "out"
move "e"
move "go door"

steal_shop @arthe_items[:odds]

move "out"
move "e"
move "go shop"

steal_shop @arthe_items[:bardic]

move "out"
move "ne"
move "go entryway"

steal_shop @arthe_items[:bobba]

move "out"
move "ne"
move "e"
move "go door"

steal_shop @arthe_items[:lobby]

move "out"

arthe_to_xing = ["w", "sw", "sw", "w", "w", "w", "s", "s", "s", "s", "go gate", "up", "up", "w",
                 "s", "s", "se", "sw", "s", "s", "se", "se", "s", "sw", "sw", "s", "s", "s", "go gate",
                 "w", "w", "s", "s", "w", "w", "s", "w", "w", "s", "s", "s", "s", "s","s", "se"]

arthe_to_xing.each do |dir|
  move dir
end

#Leth

path = ["sw", "go bridge", "n", "n", "go ware", "s"]

path.each { |dir|
  move dir
}

put "open trap"

path = ["go trap", "go river", "w", "n", "go panel", "climb step", "s", "s", "sw", "sw", "down", "s", "sw", "sw",
        "s", "up", "sw", "w", "sw", "climb ladder", "go gap"]

path.each { |dir|
  move dir
}

path = ["s", "sw", "sw", "sw", "sw", "s", "se", "se", "s", "s", "sw", "sw",
        "sw", "s", "se", "sw", "s", "sw", "s", "s", "se", "se", "sw"]

path.each { |dir|
  move dir
}

labels_start

label(:go) {
  put "sw"
  match = { :noweb => ["Thick trees line the route here"],
            :web => ["Roundtime", "You can't do that while", /\.\.\.wait/] }
  match_wait_goto match
}

label(:web) {
  match = { :go => ["Using your escape", "The webs break apart and fall away"] }
  match_wait_goto match
}

label(:noweb) {
}

labels_end

path = ["sw", "sw", "s", "sw", "se", "se", "s", "s", "se", "se", "s",
        "s", "se", "go bower gate", "se", "se", "se", "se", "se", "se", "se"]

path.each { |dir|
  move dir
}

move "e"
move "e"
move "e"
move "e"
move "go stump"

steal_shop @leth_items[:alberdeen]

move "out"
move "w"
move "sw"
move "sw"
move "sw"
move "go door"

steal_shop @leth_items[:yerui]

move "go door"
move "sw"
move "n"
move "go hole"

steal_shop @leth_items[:ongadine]

move "out"
move "s"
move "s"
move "s"
move "go knothole"
move "up"
move "go arch"

steal_shop @leth_items[:bardic_leth]

move "go arch"
move "down"
move "out"
move "n"
move "n"
move "nw"
move "nw"
move "nw"
move "go tent"

steal_shop @leth_items[:origami]

move "out"
move "nw"
move "w"
move "w"
move "go path"
move "go door"

steal_shop @leth_items[:trueflight]

move "out"
move "go path"
move "w"
move "s"
move "se"
move "se"
move "se"
move "ne"
move "cli stair"
move "go shack"

steal_shop @leth_items[:shack]

move "out"
move "cli stair"

# go to Ilaya Taipa

move "sw"
move "sw"
move "go gate"
move "sw"
move "nw"
move "nw"
move "n"
move "nw"
move "n"
move "sw"
move "sw"
move "sw"
move "sw"
move "s"
move "s"
move "s"
move "s"
move "se"
move "sw"
move "s"
move "s"
move "s"
move "sw"
move "w"
move "w"
move "s"
move "sw"
move "sw"
move "w"
move "w"
move "nw"
move "go break"
move "go gate"

move "nw"
move "nw"
move "n"
move "n"
move "ne"
move "go tower"

steal_shop @ilaya_items[:tower]

move "out"
move "sw"
move "s"
move "go pier"

steal_pier

move "w"
move "s"
move "se"
move "se"
move "e"
move "go pier"

steal_pier

move "s"
move "se"
move "e"
move "go pier"

steal_pier

move "s"
move "e"
move "e"
move "ne"
move "ne"
move "ne"
move "n"
move "n"
move "go cottage"

steal_shop @ilaya_items[:fish]

move "out"
move "n"
move "n"
move "nw"
move "nw"
move "go stall"
move "go door"

steal_shop @ilaya_items[:fishmonger]

move "out"
move "out"
move "se"
move "se"
move "sw"
move "go shop"

steal_shop @ilaya_items[:pearls]

move "out"
move "w"
move "go shop"

steal_shop @ilaya_items[:clothing]

move "out"
move "nw"
move "go shack"

steal_shop @ilaya_items[:stuff]

move "out"
move "se"
move "e"
move "ne"
move "s"
move "s"
move "s"
move "s"
move "sw"
move "sw"
move "sw"
move "w"
move "w"
move "w"
move "nw"
move "w"

#returning to leth

move "go gate"
move "go break"
move "se"
move "e"
move "e"
move "ne"
move "ne"
move "n"
move "e"
move "e"
move "ne"
move "n"
move "n"
move "n"
move "ne"
move "nw"
move "n"
move "n"
move "n"
move "n"
move "ne"
move "ne"
move "ne"
move "ne"
move "s"
move "se"
move "s"
move "se"
move "se"
move "ne"

check_for_mites

move "go gate"
move "ne"
move "ne"

move "ne"
move "ne"
move "ne"
move "ne"
move "ne"

put "khri stop"

path = ["nw", "nw", "nw", "nw",  "nw", "nw", "nw", "go bower gate", "nw", "n", "n",
        "nw", "nw", "n", "n", "nw", "nw", "ne", "n", "ne", "ne", "ne"]

path.each { |p|
  move p
}

labels_start

label(:go) {
  put "ne"
  match = { :noweb => ["Along the north, the trees and shrubs"],
            :web => ["Roundtime", "You can't do that while", /\.\.\.wait/] }
  match_wait_goto match
}

label(:web) {
  match = { :go => ["Using your escape", "The webs break apart and fall away"] }
  match_wait_goto match
}

label(:noweb) {
}

labels_end

path = ["nw", "nw", "n", "n", "ne", "n", "ne", "nw", "n", "ne", "ne",
        "ne", "n", "n", "nw", "nw", "n", "ne", "ne", "ne", "ne", "n",
        "go boulder", "go root", "ne", "e", "ne", "down"]

path.each { |p|
  move p
}

path = ["n", "ne", "ne", "n", "up", "ne", "ne", "north", "north"]

path.each { |p|
  move p
}

path = ["climb step", "out", "s", "e", "go ware", "up", "n", "out", "s",
        "s", "go bridge", "ne"]

path.each { |p|
  move p
}

#pawn if not caught stealing
to_pawn = ["nw", "n", "n", "n", "n", "n", "n", "w", "w", "w", "w", "go brid", "s"]
to_pawn.each { |dir|
  move dir
}

pawn_items

#bin items
to_guild = ["w", "s", "s", "s", "s", "w", "w", "s", "s", "w", "w", "w", "go ruin", "w", "go space"]
to_guild.each { |dir|
  move dir
}

bin_items

pause 1

check_for_mites

put "hide"