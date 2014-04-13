echo "***"
echo "isStanding: #{Status::standing}"
echo "isKneeling: #{Status::kneeling}"
echo "isSitting: #{Status::sitting}"
echo "isProne: #{Status::prone}"
echo "isStunned: #{Status::stunned}"
echo "isBleeding: #{Status::bleeding}"
echo "isHidden: #{Status::hidden}"
echo "isJoined: #{Status::joined}"
echo "isWebbed: #{Status::webbed}"
echo "isDead: #{Status::dead}"
echo "<br/>"
echo "*** Room ***"
echo Room::title
echo Room::description
echo Room::objects
echo Room::players
echo Room::exits
echo Room::count_objects "hog"
echo "<br/>"
echo "*** Exp ***"
echo Exp::state("percept")
echo Exp::rank("percept")
echo "<br/>"
echo "*** Inventory ***"
echo Inventory::list
echo "<br/>"
echo "*** Container ***"
echo Container::list
echo "<br/>"
echo "*** Wield ***"
echo Wield::right
echo Wield::right_noun
echo Wield::left
echo Wield::left_noun
echo "<br/>"
echo "*** Vitals ***"
echo Vitals::health
echo Vitals::concentration
echo Vitals::spirit
echo Vitals::fatigue
echo "***"