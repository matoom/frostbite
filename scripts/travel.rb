@dest = $args.first

def from_xing_to_shard
  load "leth.rb"
  load "gond.rb"
  load "ugond.rb"
  load "shard.rb"
end

def from_shard_to_xing
  load "xshard.rb"
  load "xugond.rb"
  load "xgond.rb"
  load "xleth.rb"
end

def from_xing_to_haven
  load "haven.rb"
end

def from_xing_to_ross
  load "haven.rb"
  load "rossmans.rb"
end

if @dest == "shard"
  case Room::title
    when /\[The Crossing, Hodierna Way\]/
      from_xing_to_shard
    else
      echo "Destination not found."
  end
elsif @dest == "crossing"
  case Room::title
    when /\[Wyvern Trail, Journey's Rest\]/
      from_shard_to_xing
#    when /\[Shard, East City Gates\]/, /\[Shard, East Bridge\]/
#      from_shard_to_xing
    else
      echo "Destination not found."
  end
elsif @dest == "haven"
  case Room::title
    when /\[Northeast Wilds, Outside Northeast Gate\]/
      from_xing_to_haven
    else
      echo "Destination not found."
  end
elsif @dest == "rossmans"
  case Room::title
    when /\[Northeast Wilds, Outside Northeast Gate\]/
      from_xing_to_ross
    else
      echo "Destination not found."
  end
end

