require "defines"

@start_time = Time.now
@attack_count = 0

10000.times do
  put "claw"
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
      @attack_count = @attack_count + 1
      echo "Time to kill: #{Time.now - @start_time}"
      echo "Swings to kill: #{@attack_count}"
      @start_time = Time.now
      @attack_count = 0
      load "skin"
    when :wait_for
      echo "*** WAITING ***"
      wait_for(/begins to advance on you|closes to melee range/)
    when :adv
      put "advance"
      pause 2
    when :pause
      pause 3
    when :continue
      @attack_count = @attack_count + 1
  end
end