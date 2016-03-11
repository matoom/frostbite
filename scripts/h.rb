# desc: trains hiding, stalking and backstabbing for thieves
# requirements: only works for thieves cirlce 70+, pref min. hiding rt
# run: hunting area

#You quickly slip into hiding to prepare to stalk.
#You melt into the background, convinced that your attempt to hide went unobserved.
#It's hard to stalk if you aren't in a position to move around.

require "hunt"
require "combat"

@rt_adjust = -1

class String
  def numeric?
    Float(self) != nil rescue false
  end
end

Combat::auto_target "*** hide on what? usage: .h &lt;critter_name&gt;  &lt;difficulty 1 - (9)&gt; ***"

@targets = ["right leg", "back", "right arm", "right hand", "abdomen", "chest", "neck", "head", "right eye"]

difficulty_level = ($args.at(1) and $args.at(1).numeric?) ? $args.at(1).to_i : 0

@target = @targets.fetch(difficulty_level - 1)

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
  hunt
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
  #pause Rt::value
  goto :feint
}

label(:feint) {
  # legs -> back -> arms -> hands -> abdomen -> chest -> neck -> head -> eyes
  put "feint #{@target}"
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