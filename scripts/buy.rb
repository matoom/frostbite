@item = $args.join(" ")

def stow_item
  stow = false
  if !Wield::right_noun.empty?
    stow = true
    put "stow right"
    wait
  end

  if !Wield::left_noun.empty?
    stow = true
    put "stow left"
    wait
  end

  if !stow
    echo "*** ERROR! ***"
    exit
  end
end

25.times do 
  put "buy #{@item}"
  match = { :price => [/d+/] }
  result = match_get match
  price = result.scan(/\d+/).first

  if price
    put "offer #{price}"
    wait
    stow_item
  else
    echo "*** Can't buy item! ***"
    exit
  end
end