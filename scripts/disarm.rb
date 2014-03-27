# desc: Disarm boxes you have in open containers or holding in your right hand,
#       harvest only on quick and blind disarms.
# requirements: -
# run: anywhere

@box_types = ["chest", "trunk", "box", "skippet", "strongbox", "coffer", "crate", "casket", "caddy"]
@disarm_methods = ["blind", "quick", "normal", "careful"]
@harvest_methods = ["blind", "quick"]

@@shift_count = 0

MAX_SHIFT_COUNT = 3

Observer.instance.register_event({ :shift => "This is not likely to be a good thing." })

def shift
  @@shift_count += 1
  echo "Shifted trap mechanism! - total: #{@@shift_count}"

  if MAX_SHIFT_COUNT < @@shift_count
    exit_script("*** Unable to disarm! ***")
  end
end

@harvest = true

if $args.join(" ").include? "no.harvest"
  @harvest = false;
end

def ident(box)
  put "disarm my #{box} ident"
  match = { :wait => [/\.\.wait/],
            :ident => ["fails to reveal to you what"],
            :quick => ["should not take long", "aged grandmother could defeat", "could do it blindfolded"],
            :normal => ["a simple matter", "can take down any time", "this trap is precisely at your",
                        "only minor troubles", "has the edge on you"],
            :careful => ["have some chance of being able to", "odds are against you", "would be a longshot"],
            :hard => ["really don't have any", "prayer would be a good start", "same shot as a snowball",
                      "pitiful snowball encased", "just jump off a cliff"],
            :end => ["guess it is already disarmed", "Roundtime"] }
  result = match_wait match

  case result
    when :wait, :ident
      pause 0.5
      ident box
    when :end
        print_disarmed
        return
    when :hard
      exit_script("*** Unable to disarm! ***")
    else
      disarm(box, result)
  end
end

def get_method method
  index = @disarm_methods.index(method.to_s) + @@shift_count
  @disarm_methods.fetch(index, "careful")
end

def disarm(box, method)
  method = get_method method
  put "disarm my #{box} #{method}"
  match = { :wait => [/\.\.\.wait/],
            :re_try => ["progress"],
            :analyze => ["not yet fully disarmed"],
            :analyze_last => ["Roundtime"] }
  result = match_wait match

  case result
    when :wait, :re_try
      pause 0.5
      disarm(box, method)
    when :analyze
      @@shift_count = 0
      if @harvest and @harvest_methods.include? method
        analyze box, false
      else
        ident box
      end
    when :analyze_last
      @@shift_count = 0
      if @harvest and @harvest_methods.include? method
        analyze box, true
      else
        print_disarmed
        return
      end
  end
end

def analyze(box, last)
  put "disarm my #{box} analyze"
  match = { :wait => [/\.\.\.wait/],
            :reanalyze => ["unable to determine"],
            :ident => ["not fully disarmed", "what could you possibly analyze"],
            :end => ["unsuitable for"],
            :harvest => ["Roundtime"] }
  result = match_wait match

  case result
    when :wait, :reanalyze
      pause 0.5
      analyze box, last
    when :ident
      if last
        print_disarmed
        return
      else
        ident box
      end
    when :end
        print_disarmed
        return
    when :harvest
      harvest box, last
  end
end

def harvest(box, last)
  put "disarm my #{box} harvest"
  match = { :wait => [/\.\.\.wait/],
            :harvest => ["unable to extract"],
            :end => ["inept fumblings have damaged", "mangled remnants"],
            :stow => ["Roundtime"] }
  result = match_wait match

  case result
    when :wait, :harvest
      pause 0.5
      harvest box, last
    when :end
      if last
        print_disarmed
        return
      else
        ident box
      end
    when :stow
      put "stow left"
      if last
        print_disarmed
        return
      else
        ident box
      end
  end
end

def print_disarmed
  echo "*** box disarmed! ***<br/>"
end

def exit_script(message)
  echo message
  exit
end

wield_right = Wield::right_noun

if !wield_right.empty?
  @box_types.each do |box|
    if wield_right == box
      ident box
      break
    end
  end
elsif wield_right.empty?
  catch :break do
    @box_types.each_with_index do |box|
      put "get my first #{box}"
      match = { :wait => [/\.\.\.wait/],
                :next => ["were you referring"],
                :open => ["You get"] }

      case match_wait match
        when :open
          ident box
          throw :break
        when :wait
          pause 0.5
          redo
      end
    end
    exit_script "*** All boxes disarmed! ***"
  end
end


