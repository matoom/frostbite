@recover = 30
$recover_ts = -1

def activate verb, spells
  return if $recover_ts.to_i > -1 && Time.now - $recover_ts < @recover
  spells.each do |spell|
    unless Spell::active.any? { |s| /#{verb} #{spell}/i =~ s}
      Observer.instance.call_event "activate_event", "#{verb} start #{spell}"
    end
  end
end

def activate_event e
  Observer::sync_rt {
    activate_sync e
  }
end

def activate_sync e
  put "#{e}"
  match = { :wait => [/\.\.wait/],
            :no_spell => ["rephrase that command"],
            :recover => ["not recovered"],
            :running => ["Roundtime", "already using the"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      activate_sync e
    when :recover
      $recover_ts = Time.now
    when :no_spell
      Thread.exit
  end
end


