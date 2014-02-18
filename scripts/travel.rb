@dest = $args.first

def from_xing_to_shard
  load "leth"
  load "gond"
  load "ugond"
  load "shard"
end

def from_shard_to_xing
  load "xshard"
  load "xugond"
  load "xgond"
  load "xleth"
end

def from_xing_to_haven
  load "haven"
end

def from_xing_to_ross
  load "haven"
  load "rossmans"
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

