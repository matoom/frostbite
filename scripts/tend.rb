@match_body_part = "right leg|left leg|abdomen|back|chest|right arm|left arm|right hand|left hand|neck|head|right eye|left eye"
@match_lodgeable = "bolt|arrow"

def drop items
  items.each do |item|
    put "drop #{item}"
    wait
  end
end

def drop_lodged
  drop Wield::left_noun.scan(/#{@match_lodgeable}/)
  drop Wield::right_noun.scan(/#{@match_lodgeable}/)
end

def tend wounds, dump
  wounds.each do |wound|
    wound.scan(/#{@match_body_part}/).each do |area|
      put "tend my #{area}"
      match = {:wait => [/\.\.\.wait|you may only type ahead/],
               :continue => [/too injured for you to do that|Doing your best|Roundtime/] }

      case match_wait match
        when :wait
          pause 0.5
          redo
      end

      drop_lodged if dump
    end
  end
end

def find_wounds
  put "health"
  match = {:wait => [/\.\.\.wait|you may only type ahead/],
           :bleed => [/^\s*(#{@match_body_part})\s*[^()]++$/],
           :lodged => [/lodged shallowly into/],
           :mites => [/red blood mite/],
           :match_until => [/>|no significant injuries/]}

  result = match_get_m match

  echo result

  return if result[:match_until].first.include? "no significant injuries"

  if result.has_key?(:wait)
    find_wounds
  else
    if result.has_key?(:lodged)
      tend result[:lodged], true
    end

    if result.has_key?(:mites)
      tend result[:mites], false
    end

    if result.has_key?(:bleed)
      tend result[:bleed], false
    end
  end
end

find_wounds