require "defines"

put "open my backpack"

def move_right item
  pause 0.5
  if Wield::left_noun.include?(item)
    put "swap"
    wait
  end
end

ITEMS::ARMOR.each do |item|
  pause 0.2
  put "remove my #{item}"
  wait
  move_right item
  put "contact fix"
  match = { :next => ["aren't holding anything"],
            :stow => [/Roundtime/] }
  result = match_wait match

  case result
    when :stow
      put "stow right"
      wait
  end
end