@item = $args.join(" ")

def stow_item
  pause 0.2

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

=begin
>offer 250
Ragge sighs.  "Despite the rarity of this lockpick, I'm prepared to offer it to you for 250 kronars."

Ragge hands over your lockpick.
>stow left
You put your lockpick in your locksmith's toolbelt.
>buy stout lock
Ragge looks up from his work and says, "I've got time to pick open boxes now if you want to put them on the counter here."
>

Ragge sighs.  "Despite the rarity of this lockpick, I'm prepared to offer it to you for 250 kronars."
>
Ragge looks up from his work and says, "I've got time to pick open boxes now if you want to put them on the counter here."
*** Can't buy item! ***
[Script finished, Execution time - 00:19.]
=end

25.times do 
  put "buy #{@item}"
  match = { :price => [/d+/] }
  result = match_get match
  price = result.scan(/\d+/).first
  echo result

  if price
    put "offer #{price}"
    wait
    stow_item
  else
    echo "*** Can't buy item! ***"
    exit
  end
end