# desc: loots boxes
# requirements: stow container set up
# run: anywhere

require "defines"

@box_types = ["chest", "trunk", "box", "skippet", "strongbox", "coffer", "crate", "casket", "caddy"]
@container = "backpack"
@junk = ["runestone", "flower", "leaf", "grass", "mortar", "pestle"]

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
    unless item.include?("coin")
      put "put #{item} in my #{@container}"
      wait
    end
  end

  dismantle
end

def dismantle
  LOCKSMITH::current_box = ""
  put "dismantle my #{@wield_right} thump"
  wait
end

def exit_script message
  echo message
  exit
end

@wield_right = Wield::right_noun
unless @box_types.any?{ |box_type| @wield_right.include? box_type }
  exit_script("*** Need to be holding a box in the right hand! ***")
end

open