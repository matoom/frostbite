label(:start) {
  put "exp mech"
  match = { :end => ["% mind lock", "% nearly locked"],
            :forage => ["EXP HELP"] }
  match_wait_goto match
}

label(:end) {
  echo "*** END ***"
  exit
}

label(:forage) {
  echo "*** FORAGE ***"
  exit
}