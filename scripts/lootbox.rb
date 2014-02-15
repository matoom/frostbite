# desc: loots boxes
# requirements: stow container set up
# run: anywhere

@box_types = ["chest", "trunk", "box", "skippet", "strongbox", "coffer", "crate", "casket", "caddy"]
@container = "backpack"
@junk = ["runestone", "flower", "leaf", "grass", "mortar", "pestle"]

def finally_do
end

def open
  put "open my #{@wield_right}"
  match = { :wait => [/\.\.wait/],
            :continue => ["already open", "You open"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      open
    when :continue
      list_items
  end
end

def list_items
  put "look in my #{@wield_right}"
  match = { :match => ["In the"] }
  contents = match_get match

  items = contents[:match].split(/,|\band\b/).collect { |s| s.split.last.delete('.') }

  if contents == ""
    dismantle
  else
    loot items
  end
end

def loot items
  echo items.inspect

  items.each do |item|
    put "get #{item} from my #{@wield_right}"
    wait
    if !item.include?("coin")
      put "put #{item} in my #{@container}"
      wait
    end
  end

  dismantle
end

def dismantle
  put "dismantle my #{@wield_right} thump"
end

def exit_script message
  echo message
  exit
end

@wield_right = Wield::right_noun
if !@box_types.any?{ |box_type| @wield_right.include? box_type }
  exit_script("*** Need to be holding a box in the right hand! ***")
end

open

=begin
>open my tru
You open the steel trunk...

In the steel trunk you see a small grey diamond and a huge grey hematite.



>l in my tru
In the copper trunk you see a medium purple-blue sapphire, a copper coin, some bronze coins and some gold coins.
=end