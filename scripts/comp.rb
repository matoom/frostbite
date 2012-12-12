# desc: studies a compendium of anatomy charts
# requirements: ??
# run: anywhere

def finally_do
  put "put my compendium in my shroud"
end

put "get my compendium"

labels_start

label(:study) {
  put "study my compendium"
  match = { :study => ["You begin studying", "You continue studying", "You continue to study"],
            :turn => ["makes sense to you"],
            :exit => ["study this chart again"] }
  match_wait_goto match
}

label(:turn) {
  put "turn my compendium"
  wait
  goto :study
}

label(:exit) {
  exit
}

labels_end