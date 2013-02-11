# desc: collects piles of specified foraging items and cleans up after
# requirements: ??
# run: valid foraging spot

if $args.empty?
  echo "Item name missing!"
  exit!
end

def finally_do
  put "kick pile"
end

#def every_10s_do
#  put "exp perception"
=begin
  match = { :end => [/% mind lock/, /% nearly locked/],
            :forage => [/EXP HELP/] }

  case match_wait match
    when :end
      exit
  end
=end
#end

100.times do
  put "collect " + $args.join(" ")
  wait_for_roundtime
  put "kick pile"
end