# desc: trains hiding, stalking and backstabbing for thieves
# requirements: only works for thieves cirlce 70+, pref min. hiding rt
# run: hunting area

require "target"
require "defines"

@rt_adjust = 0

Target::auto "*** backstab what? usage: .bs &lt;critter_name&gt; ***"

@kills = 0

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

Thread.new do
  while true
    activate "khri", ["guile"]
    sleep 15
  end
end

def finally_do
  time = Time.now - @start
  echo "time: #{time / 60}m"
  echo "kills: #{@kills}"
  echo "k/m: #{@kills / (time / 60)}"
end

@start = Time.now

labels_start

label(:start) {
  put "face #{$args.first}"
  match = { :wait_for => ["Face what?"],
            :hide => ["You are already facing", "You turn to face", "You are too closely engaged"],
            :pause => ["You are still stunned", "the point in facing"],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :start)
}

label(:hide) {
  pause Rt::value
  put "hide"
  match = { :wait_for => ["Stalk what?"],
            :feint => ["your stalking went unobserved", "slip into hiding to prepare", "melt into the background", "But you're already hidden"],
            :stop_stalk => ["You're already stalking"],
            :hide => ["ruining your hiding"],
            :pause => ["You are still stunned"],
            :wait => [/\.\.\.wait/] }
  go_wait(match_wait(match), :hide)
}

label(:stop_stalk) {
  put "stop stalk"
  pause_rt
  goto :feint
}

label(:feint) {
  put "backstab"
  match = { :dead => COMBAT::MATCH_DEAD,
            :advance => ["would help if you were closer", "aren't close enough"],
            :hide => ["Roundtime", "hidden to backstab"],
            :start => ["You can't backstab that.", "Backstab what?"],
            :wait => [/\.\.\.wait/]}
  go_wait(match_wait(match), :feint)
}

label(:advance) {
  put "advance"
  pause 3
  match = { :hide => ["begin to advance", "You are already", "begin to stealthily advance"],
            :wait => [/\.\.\.wait/]}
  go_wait(match_wait(match), :advance)
}

label(:dead) {
  @kills += 1
  load "skin.rb"
  goto :start
}

label(:wait_for) {
  if Target::is_auto
    new_target = Target::find
    if new_target
      $args.clear << new_target
      goto :start
    end
    pause 3
  end

  echo "*** WAITING ***<br/>"
  wait_for(/begins to advance you|closes to melee range|at you\./)
  goto :start
}

labels_end
