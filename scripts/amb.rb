# desc: trains hiding, stalking and backstabbing for thieves
# requirements: only works for thieves circle 70+, pref min. hiding rt
# run: hunting area

require "target"
require "hunt"

@ambushes = ["clout"]
@total = @ambushes.size
@count = @total - 1

Target::auto "*** ambush what? usage: .amb &lt;critter_name&gt; ***"

def go_wait(label, back_label)
  if label == :wait
    pause 0.5
    goto back_label
  else
    goto label
  end
end

labels_start

label(:face) {
  put "face #{$args.first}"
  match = { :wait_for => ["Face what?"],
            :hide => ["You are already facing", "You turn to face", "You are too closely engaged", "facing a dead"],
            :pause => ["You are still stunned"],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :start)
}

label(:hide) {
  pause Rt::value
  hunt
  put "stalk"
  match = { :wait_for => ["Stalk what?"],
            :feint => ["your stalking went unobserved", "slip into hiding to prepare", "melt into the background"],
            :stop_stalk => ["You're already stalking"],
            :hide => ["ruining your hiding"],
            :pause_stalk => ["You are still stunned"],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :hide)
}

label(:stop_stalk) {
  put "stop stalk"
  pause_for_roundtime
  goto :feint
}

label(:feint) {
  @count += 1
  put "ambush #{@ambushes[@count % @total]}"
  match = { :dead => COMBAT::MATCH_DEAD,
            :advance => ["would help if you were closer", "aren't close enough"],
            :hide => ["Roundtime", "hidden or invisible to ambush"],
            :face => ["You can't backstab that."],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :feint)
}

label(:advance) {
  put "advance"
  put "shiver"
  match = { :hide => ["begin to advance", "You are already", "begin to stealthily advance"],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :advance)
}

label(:dead) {
  load "skin.rb"
  goto :start
}

label(:pause_stalk) {
  pause 5
  goto :hide
}

label(:wait_for) {
  echo "*** WAITING ***<br/>"
  wait_for(/begins to advance you|closes to melee range/)
  goto :start
}

labels_end
