def finally_do
  put "dir stop"
end

def directions
  put "dir #{$args.join(" ")} 100"
  m = match_get({ :go => [/Directions towards|don't know the way|Here are all the places/], :arrived => ["You're there already"] })
  return [] if m[:key].equal? :arrived
  m[:match].gsub(/.*:/, '').split(/,|\band\b/).collect(&:strip)
end

directions.each do |dir|
  if dir =~ /west|south|north|east|go|out|climb|down|up/i
    move dir.downcase
  end
end