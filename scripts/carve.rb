def finally_do
  if Wield::right_noun != ""
    put "stow right"
    wait
  end

  if Wield::left_noun != ""
    put "stow left"
    wait
  end
end

def prep
  put "get my knife in my toolbelt"
end

def stow
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
      stow
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
  put "stow left"
  start
end

prep
start