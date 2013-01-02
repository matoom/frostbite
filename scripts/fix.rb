@items = ["coat", "gloves", "balaclava", "pants", "shield"]

put "open my backpack"

@items.each do |item|
  pause 0.2
  put "remove my #{item}"
  wait
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