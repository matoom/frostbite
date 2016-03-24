# desc: trains hiding, stalking and backstabbing for thieves
# requirements: only works for thieves cirlce 70+, pref min. hiding rt
# run: hunting area

require "target"

@rt_adjust = 0

Target::auto "*** backstab what? usage: .bs &lt;critter_name&gt; ***"

def go_wait(label, back_label)
  if label == :wait
    pause 0.5
    goto back_label
  elsif label == :pause
    pause 3
    goto back_label
  else
    goto label
  end
end

labels_start

label(:start) {
  put "face #{$args.first}"
  match = { :wait_for => ["Face what?"],
            :hide => ["You are already facing", "You turn to face", "You are too closely engaged", "facing a dead"],
            :pause => ["You are still stunned"],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :start)
}

label(:hide) {
  pause Rt::value
  put "stalk"
  match = { :wait_for => ["Stalk what?"],
            :feint => ["your stalking went unobserved", "slip into hiding to prepare", "melt into the background"],
            :stop_stalk => ["You're already stalking"],
            :hide => ["ruining your hiding"],
            :pause => ["You are still stunned"],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :hide)
}

label(:stop_stalk) {
  put "stop stalk"
  pause_for_roundtime
  goto :feint
}

label(:feint) {
  put "backstab"
  match = { :dead => COMBAT::MATCH_DEAD,
            :advance => ["would help if you were closer", "aren't close enough"],
            :hide => ["Roundtime", "hidden to backstab"],
            :start => ["You can't backstab that."],
            :wait => [/\.\.\.wait/]}
  go_wait(match_wait(match), :feint)
}

label(:advance) {
  put "advance"
  put "shiver"
  pause 3
  match = { :hide => ["begin to advance", "You are already", "begin to stealthily advance"],
            :wait => [/\.\.\.wait/]}
  go_wait(match_wait(match), :advance)
}

label(:dead) {
  load "skin"
  goto :start
}

label(:wait_for) {
  if Target::is_auto
    new_target = Target::find
    unless new.empty?
      $args.clear << new_target
      goto :start
    end
  end

  echo "*** WAITING ***<br/>"
  wait_for(/begins to advance you|closes to melee range/)
  goto :start
}

labels_end
