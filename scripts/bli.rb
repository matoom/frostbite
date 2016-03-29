require "defines"
require "helper"

def finally_do
  sleep Rt::value
  stow_pick
  stow_box
end

def stow_pick
  if Wield::left_noun.eql? "lockpick" or Wield::right_noun.eql? "lockpick"
    put_wait "put my lock in my shroud", /You put|referring to/
  end
end

def stow_box
  unless LOCKSMITH::current_box.empty?
    put_wait "put my #{LOCKSMITH::current_box} in my #{LOCKSMITH::DISARMED_BOX_CONTAINER}", /You put|referring to/
  end
end

def get_pick
  put "get my lock from my shroud"
  match = { :wait => [/\.\.\.wait/],
            :continue => [/You get a/],
            :end => [/were you referring/] }
  case match_wait match
    when :wait
      pause 0.5
      get_pick
    when :end
      echo "*** Out of lockpicks! ***"
      exit!
  end
end

def get_box
  wield_right = Wield::right_noun
  if wield_right != ""
    LOCKSMITH::BOX_TYPES.each do |box|
      if wield_right == box
        LOCKSMITH::current_box = box
        return
      end
    end
  end

  LOCKSMITH::BOX_TYPES.each_with_index do |box|
    put "get my first #{box} from my #{LOCKSMITH::DISARMED_BOX_CONTAINER}"
    match = {:wait => [/\.\.\.wait/],
             :next => ["were you referring"],
             :open => ["You get"]}
    res = match_wait match

    case res
      when :open
        LOCKSMITH::current_box = box
        break
      when :wait
        pause 0.5
        redo
    end
  end
end

put "open my #{LOCKSMITH::DISARMED_BOX_CONTAINER}"
wait

get_box
get_pick

catch (:done) do
  1000.times do
    break if SYSTEM::finished
    put "pick my #{$args.join(" ")} blind"
    match = { :continue => [/\.\.\.wait|You are unable to|You discover another|soft click/],
              :get_pick => [/more appropriate tool/],
              :loot => [/not even locked/], :done => [/Pick what/] }
    res = match_wait match

    case res
      when :get_pick
        get_pick
        pause 1
      when :loot
        stow_pick
        load "lootbox"
        throw :done
      when :done
        exit!
    end
  end
end