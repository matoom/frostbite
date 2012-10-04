exp = get_exp "climbing"
echo exp.inspect

sleep 10

room = get_room
echo room.inspect
echo room.count_objects "hog"

wield = get_wield
echo wield.inspect

sleep 10

vitals = get_vitals
echo vitals.inspect

=begin
for i in 0..5
	put "n"
	sleep 1
  if i == 4
    puts exp("skinning").inspect
  end
end
=end

echo "END"