def active_spells_maintain(spells)
  pause_rt
  active = Spell::active
  cmd = []
  spells.split(' ').each { |spell|
    cmd << spell unless active.any? { |s| /#{spell}/i =~ s }
  }
  active_spells_start("khri #{cmd.join(' ')}") unless cmd.empty?
end

def active_spells_start(cmd)
  put cmd
  match = { :wait => [/\.\.wait/],
            :no_spell => ["rephrase that command", "not trained"],
            :running => ["Roundtime", "already using the", "not recovered from"] }
  case match_wait match
    when :wait
      pause Rt::value + 0.5
      active_spells_start cmd
    when :no_spell
      Observer::instance.remove_timer(:active_spells_maintain)
  end
end

