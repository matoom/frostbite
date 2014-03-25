require "defines"
require "hunt"

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
            :redo => [/evades,|dodges,|A (\w|\s|'|-)+ of the (\w|\s|'|-)+ with/],
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
  hunt
  execute_seq extract_seq analyze
end