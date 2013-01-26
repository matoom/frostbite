def handle_delivery
  put "wear my #{Wield::right_noun}"
  match = { :stow => ["can't wear that"],
            :continue => ["You put", "You slide", "You attach", "You slip", "You work"] }
  result = match_wait match

  case result
    when :stow
      put "stow right"
      wait
  end
end

100.times do
  put "get ticket"
  put "contact fix"
  match = { :end => ["were you referring"],
            :continue => ["Roundtime"] }
  result = match_wait match

  case result
    when :continue
      handle_delivery
    when :end
      break
  end
end