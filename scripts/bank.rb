# desc: travels to bank in dir available areas
# requirements: -
# run: city streets

def dir
  put "dir bank 100"
  match_get({ :go => [/Directions towards|don't know the way/] }).gsub(/.*:/, '').split(/,/).collect(&:strip)
end

dir.each do |dir|
  next unless dir =~ /west|south|north|east|go/i
  move dir.downcase
end

put "dir stop"