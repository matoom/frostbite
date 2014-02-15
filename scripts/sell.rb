# desc: sells gems by specified pouch color
# requirements: -
# run: gem shop

if !$args.first
  echo "*** Sell what? example: .sell blue red ***"
  exit
end

def get_contents color
  put "look in my #{color} pouch"
  match = { :repeat => ["ahead 1 command"],
            :match => ["In the"],
            :empty => ["There is nothing"] }
  result = match_get match

  case result[:key]
    when :empty
      return []
    when :repeat
      get_contents(color)
  end

  result[:match].split(/,|\band\b/).collect { |s| s.split.last.delete('.') }
end

def sell_pouch color
  contents = get_contents color
  pause 0.1
  contents.each do |item|
    if item == "stuff"
      sell_pouch color
    end

    put "get #{item} from my #{color} pouch"
    sell_gem item
  end
end

def sell_gem gem
    put "sell my #{gem}"
    match = { :repeat => ["ahead 1 command"],
              :next => ["You ask", "Sell what?"] }

    case match_wait match
      when :repeat
        pause 0.5
        sell_gem gem
    end
end

$args.each do |color|
  put "get my #{color} pouch"
  wait

  sell_pouch color

  put "stow right"
  wait
  echo "*** All gems sold in #{color} pouch! ***"
end