def finally_do
	put "put my compendium in my shroud"
end

put "get my compendium"

label(:study) {
	put "study my compendium"
	match = { :study => ["You begin studying", "You continue studying", "You continue to study"],
			      :turn => ["makes sense to you"],
			      :exit => ["study this chart again"]}
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
