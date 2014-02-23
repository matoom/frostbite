def finally_do
  pause_for_roundtime
  stow_knife

  left = Wield::left_noun
  if Wield::left_noun != ""
    put "put my #{left} in my backpack"
    wait
  end
end

def prep
  put "get my knife in my toolbelt"
end

def stow_knife
  put "put my knife in my toolbelt"
end

def start
  put "get my keyblank"
  match = { :wait => [/\.\.\.wait|you may only type ahead/],
            :carve => [/You get a/],
            :end => [/What were you referring to?/] }

  case match_wait match
    when :wait
      pause 0.5
      start
    when :carve
      carve "keyblank"
    when :end
      stow_knife
      exit
  end
end

def carve item
  put "carve my #{item} with my knife"
  match = { :wait => [/\.\.\.wait|you may only type ahead/],
            :carve => [/Roundtime/],
            :start => [/muttered curse/],
            :finished => [/find a creature to carve/] }

  case match_wait match
    when :wait
      paues 0.5
      carve item
    when :carve
      carve "lockpick"
    when :start
      start
    when :finished
      finished
  end
end

def finished
  put "put my #{Wield::left_noun} in my backpack"
  start
end

prep
start