require "defines"

@@rt_list = Array.new

Observer.instance.register_event({ :on_roundtime => "Roundtime" })

def on_roundtime
  @@rt_list << Rt::value
  echo "rt: #{Rt::value}, count: #{@@rt_list.length}"
end

def finally_do
  echo @@rt_list.length
  result = {}
  @@rt_list.uniq.each{|element| result[element] = @@rt_list.count(element)}
  echo result.inspect
end

10000.times do
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