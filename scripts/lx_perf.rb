require "defines"
require "lx_stat"

Observer.instance.register_event({ :perf_aim => /^You begin to target/ })
Observer.instance.register_event({ :perf_fire => "you fire" })
Observer.instance.register_event({ :perf_miss => /evades,|dodges,|barely blocks with|A (\w|\s|'|-)+ of the (\w|\s|'|-)+ with/ })
Observer.instance.register_event({ :perf_dead => COMBAT::MATCH_DEAD })
Observer.instance.register_event({ :perf_dmg => /"#{COMBAT::DAMAGE.map{ |e| "you fire.*" + e }.join("|")}"/ })

def perf_aim
  Stats::reset_timer
end

def perf_fire
  Stats::register_shot
end

def perf_miss
  Stats::register_miss
end

def perf_dead
  Stats::register_kill
end

def perf_dmg e
  catch :break do
    COMBAT::DAMAGE.each_with_index do |dmg, i|
      if e.include? dmg
        Stats::register_damage i + 1
        throw :break
      end
    end
  end
end

def finally_do
  Stats::report_total_stats
end
