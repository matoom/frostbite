# desc: Disarm boxes you have in open containers or holding in your right hand,
#       harvest only on quick and blind disarms.
# requirements: -
# run: anywhere

def ident(box)
  put "disarm my #{box} ident"
  match = { :wait => ["..wait"],
            :ident => ["fails to reveal to you what"],
            :blind => ["aged grandmother could defeat", "could do it blindfolded"],
            :quick => ["can take down any time", "a simple matter", "should not take long"],
            :normal => ["this trap is precisely at your", "only minor troubles", "has the edge on you"],
            :careful => ["have some chance of being able to", "odds are against you", "would be a longshot"],
            :hard => ["really don't have any", "prayer would be a good start", "same shot as a snowball",
                      "pitiful snowball encased", "just jump off a cliff"],
            :end => ["guess it is already disarmed", "Roundtime"] }
  result = match_wait match

  case result
    when :wait
      pause 0.5
      ident box
    when :end
      exit_script("*** Box is already disarmed! ***")
    when :hard
      exit_script("*** Unable to disarm! ***")
    else
      disarm(box, result)
  end
end

def disarm(box, method)
  put "disarm my #{box} #{method.to_s}"
  match = { :wait => ["...wait"],
            :next_method => ["not likely to be a good thing"],
            :continue => ["progress"],
            :analyze => ["Roundtime", "not yet fully disarmed"]}
  result = match_wait match

  case result
    when :wait, :continue
      pause 0.5
      disarm(box, method)
    when :next_method
      next_index = @disarm_methods.index(method.to_s)
      disarm(box, @disarm_methods.fetch(next_index + 1, "careful"))
    when :analyze
      if method == :blind or method == :quick
        analyze box
      else
        ident box
      end
    else
      exit_script "*** ERROR in - disarm(box, method)! ***"
  end
end

def analyze(box)
  put "disarm my #{box} analyze"
  match = { :wait => ["...wait"],
            :analyze => ["unable to determine"],
            :ident => ["not fully disarmed", "what could you possibly analyze"],
            :end => ["unsuitable for"],
            :harvest => ["Roundtime"] }
  result = match_wait match

  case result
    when :wait, :analyze
      pause 0.5
      analyze box
    when :ident
      itent box
    when :end
      exit_script "*** Box disarmed! ***"
    when :harvest
      harvest box
    else
      exit_script "*** ERROR in - analyze(box)! ***"
  end
end

def harvest(box)
  put "disarm my #{box} harvest"
  match = { :wait => ["...wait"],
            :harvest => ["unable to extract"],
            :end => ["inept fumblings have damaged", "mangled remnants"],
            :stow => ["Roundtime"] }
  result = match_wait match

  case result
    when :wait, :harvest
      pause 0.5
      harvest box
    when :end
      exit_script "Box disarmed! ***"
    when :stow
      put "stow left"
      exit_script "Box disarmed! ***"
    else
      exit_script "*** ERROR in - harvest(box)! ***"
  end
end

def exit_script(message)
  echo message
  exit
end


@box_types = ["chest", "trunk", "box", "skippet", "strongbox", "coffer", "crate", "casket", "caddy"]
@disarm_methods = ["blind", "quick", "normal", "careful"]

wield_right = Wield::right_noun
if wield_right != ""
  @box_types.each do |box|
    if wield_right == box
      ident box
    end
  end
end

@box_types.each_with_index do |box, index|
  put "get my first #{box}"
  match = { :wait => ["...wait"],
            :next => ["were you referring"],
            :open => ["You get"] }
  res = match_wait match

  if res == :open
    ident box
  elsif res == :wait
    pause 0.5
    --index
  end

  exit_script "*** All boxes opened! ***"
end
