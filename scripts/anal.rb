require "defines"

=begin
< Driving in like the irresistable force of a cyclone, you lunge a haralun stiletto at a Misenseor resuscitant.  A Misenseor resuscitant dodges.  The stiletto lands a solid hit to the resuscitant right leg.

< As if effort and skill were a bad thing, you chop a two-handed sword at a Misenseor resuscitant.  A Misenseor resuscitant evades, stepping aside.

< Looking as if this were a bad idea, you chop a two-handed sword at a Misenseor resuscitant.  A Misenseor resuscitant fends off most of the sword with its decaying forearm.

< Moving in like a timid schoolboy, you chop a two-handed sword at a Misenseor resuscitant.  A Misenseor resuscitant evades, twisting easily away from the blow.

< As if fumbling muscle flab were natural, you chop a two-handed sword at a Misenseor resuscitant.  A Misenseor resuscitant dodges, leaping aside.

< You jab a haralun stiletto at a Misenseor resuscitant.  A Misenseor resuscitant deflects little of the stiletto with its decaying forearm.

< Moving poorly, you feint a haralun stiletto at a Dragon Priest assassin.  A Dragon Priest assassin knocks aside little of the stiletto with a silver q'zhalata dagger.
=end

def advance
  put "advance"
  pause 3
end

def face target
  put "face #{target}"
  match = { :wait => [/\.\.\.wait/],
            :wait_for => [/Face what?/],
            :next => [/facing a dead/],
            :continue => [/already facing|You turn to face|too closely engaged/],
            :pause => [/You are still stunned/] }

  case match_wait match
    when :wait
      pause 0.5
      face target
    when :pause, :wait_for
      pause 3
      face target
    when :next
      face "next"
  end
end

def analyze
  put "analyze"
  match = { :wait => [/\.\.\.wait/],
            :fail => [/fail to find/],
            :adv => [/closer to use tactical abilities/],
            :pause => [/still stunned|entangled in a web/],
            :continue => [/by landing a/] }
  result = match_get match

  case result[:key]
    when :wait, :fail
      pause 0.5
      analyze
    when :pause
      pause 3
      analyze
    when :adv
      advance
      analyze
    when :continue
      return result[:match]
  end
end

def extract_seq analyze_line
  analyze_line.gsub(/.*by landing|\ba\b|\./, '').split(/,|and/).collect(&:strip)
end

def do_maneuver maneuver
  put maneuver
  match = { :wait => [/\.\.\.wait/],
            :dead => COMBAT::MATCH_DEAD,
            :adv => [/aren't close enough/],
            :redo => [/evades,|dodges,|A .* of the .* with/],
            :pause => [/still stunned|entangled in a web/],
            :continue => [/Roundtime/] }
  result = match_wait match

  case result
    when :wait, :redo
      pause 0.5
      do_maneuver maneuver
    when :pause
      pause 3
      do_maneuver maneuver
    when :adv
      advance
      do_maneuver maneuver
    when :dead
      load "skin"
      face $args.first
      return :analyze
  end
end

def execute_seq maneuvers
  echo maneuvers
  maneuvers.each do |maneuver|
    case do_maneuver maneuver
      when :analyze
        break;
    end
  end
end

face $args.first
1000.times do
  execute_seq extract_seq analyze
end