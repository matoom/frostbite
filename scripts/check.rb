@recently_stolen = false

move "e"
move "n"
move "n"
move "go shop"

put "mark ring"
match = { :back => ["Roundtime"],
          :alert => ["stolen in this store recently"] }
result = match_wait match

case result
  when :alert
    @recently_stolen = true
end

move "out"
move "s"
move "s"
move "w"

if @recently_stolen
  echo ""
  echo "***********************"
  echo "*** RECENTLY STOLEN ***"
  echo "***********************"
  echo ""
end
