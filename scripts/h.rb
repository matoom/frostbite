# desc: trains hiding, stalking and backstabbing for thieves
# requirements: only works for thieves cirlce 70+, pref min. hiding rt
# run: hunting area

@match_rt_adjustment = -1

if !$args.first
  echo "*** hide on what? usage: .h <critter_name> ***"
  exit
end

def go_wait(label, back_label)
  if label == :wait
    pause 0.5
    goto back_label
  else
    goto label
  end
end

labels_start

label(:start) {
  put "face #{$args.first}"
  match = { :wait_for => ["Face what?"],
            :hide => ["You are already facing", "You turn to face"],
            :retreat => ["You are too closely engaged"],
            :wait => [/\.\.\.wait/] }
  res = match_wait match
  go_wait(res, :start)
}

label(:hide) {
  pause Rt::value
  put "stalk"
  match = { :feint => ["your stalking went unobserved", "slip into hiding to prepare", "melt into the background"],
            :stop_stalk => ["You're already stalking"],
            :hide => ["ruining your hiding"],
            :wait => [/\.\.\.wait/] }
  res = match_wait match
  go_wait(res, :hide)
}

label(:stop_stalk) {
  put "stop stalk"
  pause Rt::value
  goto :feint
}

label(:feint) {
  # legs -> back -> arms -> hands -> abdomen -> chest -> neck -> head -> eyes
  put "feint chest"
  match = { :dead => ["and collapses"],
            :advance => ["would help if you were closer", "aren't close enough"],
            :hide => ["Roundtime"],
            :face => ["You can't backstab that."],
            :wait => [/\.\.\.wait/]}
  res = match_wait match
  go_wait(res, :feint)
}

label(:advance) {
  put "advance"
  put "shiver"
  match = { :hide => ["begin to advance", "You are already"],
            :wait => [/\.\.\.wait/]}
  res = match_wait match
  go_wait(res, :advance)
}

label(:dead) {
  put "loot"
  goto :start
}

label(:wait_for) {
  wait_for(/begins to advance you|closes to melee range/)
  goto :start
}

label(:retreat) {
  put "retreat"
  wait
  put "retreat"
  wait
  goto :start
}

labels_end