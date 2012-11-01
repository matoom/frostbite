def forage
  put "forage " + $args.first
  match = { :braid => ["You manage to find"] }
  result = match_wait match

  if result == :braid
    braid
  else
    echo "*** ERROR ***"
  end
end

def braid
  put "braid my " + $args.first
  match = { :end => ["Roundtime: 8"],
            :braid => ["Roundtime"],
            :forage => ["need to have more material"] }
  result = match_wait match

  if result == :braid
    braid
  elsif result == :forage
    forage
  elsif result == :end
    echo "*** END ***"
    abort
  else
    braid
  end
end

echo "*** STARTING TO BRAID ***"
forage
