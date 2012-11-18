# desc: sells gems by specified pouch color
# requirements: -
# run: gem shop

if !$args.first
  echo "*** Sell what? example: .sell blue red ***"
  exit
end

gems = ["peridot", "kunzite", "chrysoprase", "ivory", "jade", "andalusite", "opal", "chalcedony", "garnet", "spinel",
        "sunstone", "citrine", "agate", "lazuli", "carnelian", "chrysoberyl", "tanzanite", "pearl", "tourmaline",
        "amber", "hematite", "beryl", "iolite", "tsavorite", "turquoise", "moonstone", "diopside", "topaz", "onyx",
        "crystal", "quartz", "jasper", "bloodstone", "zircon", "amethyst", "stone", "gem", "sapphire", "nugget", "bar",
        "morganite", "diamond"]

$args.each do |color|
  put "get my #{color} pouch"
  wait
  pause 0.5
  gems.each do |gem|
    result = :sell
    until result == :next
      put "get #{gem} from my pouch"
      match = { :sell => ["from inside"],
                :next => ["were you referring to"] }
      result = match_wait match

      if result == :sell
        put "sell my #{gem}"
        wait_for "then hands you"
      end
    end
  end
  put "stow right"
  echo "*** All gems sold in #{color} pouch! ***"
end