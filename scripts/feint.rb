# desc: uses feint to train melee weapons while "dancing" with your opponents
# skill: 0
# run: hunting area

@match_rt_adjustment = -1

def go_wait(label_self, label)
  if label == :wait
    pause 0.5
    goto label_self
  else
    goto label
  end
end

label_start

label(:start) {
  put "feint right leg"
  match = { :wait => ["...wait", "entangled in a web"],
            :start => ["Roundtime"],
            :adv => ["aren't close enough"],
            :wait_for => ["At what are you trying to attack?"]
  }
  res = match_wait match
  go_wait(:start, res)
}

label(:adv) {
  put "advance"
  pause 2
  goto :start
}

label(:wait_for) {
  pause 10
  goto :start
}

label_end