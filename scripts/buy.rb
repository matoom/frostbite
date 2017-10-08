@item = $args.join(" ")

unless Wield::right_noun.empty?
  put "stow right"
  wait
end

unless Wield::left_noun.empty?
  put "stow left"
  wait
end

25.times do 
  put "buy #{@item}"
  match = { :price => [/\d+/] }
  result = match_get match

  price = result[:match].scan(/\d+/).first

  if price
    put "offer #{price}"
    wait
    put "stow #{@item.split.last}"
  else
    echo "*** Can't buy item! ***"
    exit
  end
end