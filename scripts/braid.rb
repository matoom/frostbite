def braid
  put "braid my" + $args.first
  match = {:braid => "Roundtime", :forage => "need to have more material"}
  result = match_wait match

  if result == :braid
    braid
  elsif result == :forage
    forage
  else
    braid
  end
end

def forage
  put "forage" + $args.first
  match = {:braid => "You manage to find"}
  result = match_wait match

  if result == :braid
    braid
  else
    echo "*** ERROR ***"
  end
end

echo "*** STARTING TO BRAID ***"
forage
braid