require "defines"
require "hunt"

def attack
  put "attack"
  match = { :wait => [/\.\.\.wait/],
            :pause => [/still stunned|entangled in a web/],
            :skin => COMBAT::MATCH_DEAD,
            :wait_for => ["trying to attack"],
            :adv => ["You aren't close enough to attack."],
            :continue => ["Roundtime"] }

  case match_wait match
    when :wait
      pause 0.4
    when :skin
      load "skin"
    when :wait_for
      echo "*** WAITING ***"
      wait_for(/begins to advance on you|closes to melee range/)
    when :adv
      put "advance"
      pause 2
    when :pause
      pause 3
  end
end

10000.times do
  hunt
  attack
end