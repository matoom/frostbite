require "defines"
require "lx_stat"

Observer.instance.register_event({ :perf_aim => /^You begin to target/ })
Observer.instance.register_event({ :perf_fire => "you fire a" })
Observer.instance.register_event({ :perf_miss => /evades,|dodges,|A (\w|\s|'|-)+ of the (\w|\s|'|-)+ with/ })
Observer.instance.register_event({ :perf_dead => COMBAT::MATCH_DEAD })

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

def finally_do
  Stats::report_total_stats
end
