require "defines"
require "hunt"

def finally_do
  time = Time.now - $att_start
  echo "time: #{time / 60}m"
  echo "kills: #{$att_kills}"
  echo "k/m: #{$att_kills / (time / 60)}"
end

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
      $att_kills += 1
      load "skin.rb"
    when :wait_for
      echo "*** WAITING ***"
      wait_for(/begins to advance on you|closes to melee range/)
    when :adv
      put "advance"
      pause 6
    when :pause
      pause 3
  end
end

$att_kills = 0
$att_start = Time.now

10000.times do
  hunt
  attack
end