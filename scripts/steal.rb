@current_container = 0
@containers = ["backpack", "haversack"]

@crossing_items =
    { :bathhouse => ["fourth towel on rack", 2],
      :locksmith => ["ring", 2],
      :bard => ["silver", 2],
      :armor => ["leather cuirbouilli coat ", 2],
      :weapon => ["heavy crossbow", 2],
      :jewelry => ["platinum ring", 1],
      :tannery => ["lotion", 2],
      :alchemy => ["bucket", 1] }

@arthe_items =
    { :thread => [:none, 2],
      :odds => ["hat", 1],
      :bardic => ["peri'el's", 2],
      :bobba => ["ring mail", 1],
      :lobby => ["map", 1] }

@leth_items =
    { :alberdeen => ["quiver", 1],
      :yerui => ["statuette", 1],
      :bardic_leth => ["hat", 1],
      :origami => ["envelope on counter", 2],
      :trueflight => ["heavy crossbow", 2],
      :shack => ["pale hide shield", 2] }

@stolen_items = []
@shops_stolen_from = []

def drop name
  put "drop my #{name}"
end

def stow_item name
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
      if (@current_container.size > @containers.size)
        drop name
      else
        stow_item name
      end
  end
end

def stow_items
  left_hand = Wield::left_noun
  right_hand = Wield::right_noun

  if left_hand != ""
    stow_item left_hand
    @stolen_items << left_hand
  end
  if right_hand != ""
    stow_item right_hand
    @stolen_items << right_hand
  end
end

def do_hide
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

def take item
  put "steal #{item}"
  match = { :wait => [/\.\.\.wait|appears different about/],
            :leave => [/Guards!|begins to shout|trivial|should back off|You haven't picked|You can't steal/],
            :stow => [/You need at least one/],
            :continue => [/Roundtime/] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      take item
    when :stow
      stow_items
      take item
  end

  result
end

def steal item, amount_of
  do_hide

  amount_of.times do
    case take item
      when :leave
        break
    end
  end

  stow_items
end

def steal_crossing shop_name
  if @crossing_items[shop_name][0] != :none and !@shops_stolen_from.include?(shop_name)
    steal @crossing_items[shop_name][0], @crossing_items[shop_name][1]
    @shops_stolen_from << shop_name
  end
end

def steal_arthe shop_name
  if @arthe_items[shop_name][0] != :none and !@shops_stolen_from.include?(shop_name)
    steal @arthe_items[shop_name][0], @arthe_items[shop_name][1]
    @shops_stolen_from << shop_name
  end
end

def steal_leth shop_name
  if @leth_items[shop_name][0] != :none and !@shops_stolen_from.include?(shop_name)
    steal @leth_items[shop_name][0], @leth_items[shop_name][1]
    @shops_stolen_from << shop_name
  end
end

def prepare
  put "khri start safe hasten darken dampen shadowstep plunder"
  match = { :wait => [/\.\.\.wait/],
            :exit => [/enough to manage that/],
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

#prepare
prepare

#Crossing

move "nw"
move "w"
move "w"
move "w"
move "go bath"

steal_crossing :bathhouse

move "out"
move "w"
move "w"
move "n"
move "n"
move "go door"

steal_crossing :locksmith

move "out"
move "n"
move "e"
move "e"
move "e"
move "go shop"

steal_crossing :bard

move "out"
move "e"
move "e"
move "n"
move "go shop"

steal_crossing :armor

move "out"
move "e"
move "go shop"

steal_crossing :weapon

move "out"
move "s"
move "s"
move "e"
move "e"
move "go shop"

steal_crossing :jewelry

move "out"
move "w"
move "w"
move "n"
move "n"
move "n"
move "n"
move "n"
move "w"
move "w"
move "n"
move "n"
move "w"
move "go shop"
move "w"
move "w"

steal_crossing :tannery

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

steal_crossing :alchemy

move "out"
move "e"
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

steal_arthe :thread

move "out"
move "e"
move "go door"

steal_arthe :odds

move "out"
move "e"
move "go shop"

steal_arthe :bardic

move "out"
move "ne"
move "go entryway"

steal_arthe :bobba

move "out"
move "ne"
move "e"
move "go door"

steal_arthe :lobby

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

path = ["go trap", "go river", "w", "n", "go panel", "climb step"]

path.each { |dir|
  move dir
}

path = ["s", "s", "sw", "sw", "down", "s", "sw", "sw",
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

steal_leth :alberdeen

move "out"
move "w"
move "sw"
move "sw"
move "sw"
move "go door"

steal_leth :yerui

move "go door"
move "sw"
move "s"
move "s"
move "go knothole"
move "up"
move "go arch"

steal_leth :bardic_leth

move "go arch"
move "down"
move "out"
move "n"
move "n"
move "nw"
move "nw"
move "nw"
move "go tent"

steal_leth :origami

move "out"
move "nw"
move "w"
move "w"
move "go path"
move "go door"

steal_leth :trueflight

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

steal_leth :shack

move "out"
move "cli stair"
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

#bin items

to_guild = ["nw", "n", "n", "n", "n", "n", "n", "w", "w", "w", "w", "go brid", "w", "w", "w",
            "s", "s", "s", "s", "s", "s", "w", "w", "w", "go ruin", "w", "go space"]

to_guild.each { |dir|
  move dir
}

@stolen_items.each do |item|
  put "get #{item}"
  wait
  put "put #{item} in bin"
  wait
end