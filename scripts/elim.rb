def backstab target
  put "backstab #{target}"
  match = match_wait({:rt => [/Roundtime/],
                      :hide => [/must be hidden/],
                      :violence => [/no violence here/],
                      :advance => [/were closer/],
                      :lost => [/were referring/],
                      :wait => [/\.\.\.wait/]})

  case match
    when :wait
      pause 0.1
      backstab target
    when :rt
    else
      error_message match
  end
end

def grapple target
  put "grapple #{target}"
  match = match_wait({:rt => [/Roundtime/],
                      :violence => [/no violence here/],
                      :advance => [/must be closer/],
                      :lost => [/were referring/],
                      :wait => [/\.\.\.wait/],
                      :dead => [/is already/]})
  case match
    when :wait
      pause 0.1
      grapple target
    when :rt
    else
      error_message match
  end
end

def error_message match
  echo "*** #{match} ***"
  exit
end

if Status::hidden
  put "khri eliminate"
  backstab $args.first
  grapple $args.first
else
  echo "*** have to be hidden! ***"
end
