@recover = 30
$recover_ts = -1

def spell_activate verb, spells
  return if $recover_ts.to_i > -1 && Time.now - $recover_ts < @recover
  spells.each do |spell|
    unless Spell::active.any? { |s| /#{verb} #{spell}/i =~ s}
      Observer.instance.call_event "spell_start", spell
    end
  end
end

def spell_start e
  pause Rt::value
  put "khri start #{e}"
  match = { :wait => [/\.\.wait/],
            :no_spell => ["rephrase that command"],
            :recover => ["not recovered"],
            :running => ["Roundtime", "already using the"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      spell_start e
    when :recover
      $recover_ts = Time.now
    when :no_spell
      Thread.exit
  end
end


