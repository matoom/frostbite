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

  contents = match_get match

  if contents.include?("nothing in there")
    return []
  elsif contents.include?("ahead 1 command")
    get_contents(color)
  end

  contents.split(/,|\band\b/).collect { |s| s.split.last.delete('.') }
end

def sell_pouch color
  contents = get_contents color
  contents.each do |item|
    if item == "stuff"
      sell_pouch color
    end

    pause 0.4
    put "get #{item} from my #{color} pouch"
    put "sell my #{item}"
    wait
  end
end

$args.each do |color|
  put "get my #{color} pouch"
  wait

  sell_pouch color

  put "stow right"
  echo "*** All gems sold in #{color} pouch! ***"
end