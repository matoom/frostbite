# desc: studies a compendium of anatomy charts
# requirements: ??
# run: anywhere

def finally_do
  pause_for_roundtime
  put "put my compendium in my shroud"
end

put "get my #{$args.join} compendium"

labels_start

label(:study) {
  put "study my compendium"
  match = { :study => ["You begin studying", "You continue studying", "You continue to study"],
            :difficult => ["having a difficult"],
            :turn => ["makes sense to you"],
            :exit => ["study this chart again"] }
  match_wait_goto match
}

label(:turn) {
  put "turn my compendium"
  wait
  goto :study
}

label(:difficult) {
  echo "*** Chart too difficult to study. *** " 
  exit
}

label(:exit) {

}

labels_end