require "defines"

$throw_weapon = $args.join(" ")

unless $throw_weapon
  echo "Throw what?"
  exit
end

if Wield::right_noun.eql? ""
  echo "Hold melee weapon in right hand; throwing from left hand."
  exit
end

def finally_do
  pause_rt
  stow_weapon
  Client::track_exp_clear
end

def stow_weapon
  put "stow #{$throw_weapon}"
  match = { :wait => [/\.\.\.wait/],
            :stunned => [/still stunned|entangled in a web/],
            :continue => ["You stow", "You pick up", "You put your"] }
  case match_wait match
    when :wait
      pause Rt::value
      stow_weapon
    when :stunned
      pause 3
      stow_weapon
  end
end

def get_weapon
  unless Wield::left_noun.include? $throw_weapon
    put "get #{$throw_weapon}"
    match = { :wait => [/\.\.\.wait/],
              :stunned => [/still stunned|entangled in a web/],
              :continue => ["You get", "You pick up"] }
    case match_wait match
      when :wait
        pause_rt
        get_weapon
      when :stunned
        pause 3
        get_weapon
    end
  end
end

def throw
  put "throw left"
  match = { :wait => [/\.\.\.wait|entangled in a web/],
            :stunned => [/still stunned/],
            :skin => COMBAT::MATCH_DEAD,
            :wait_for => ["trying to attack"],
            :continue => ["Roundtime"] }

  case match_wait match
    when :wait_for
      get_weapon
      echo "*** WAITING ***"
      wait_for(/begins to advance you|closes to melee range|at you\./)
    when :skin
      stow_weapon
      load "skin.rb"
      get_weapon
    when :continue
      get_weapon
  end
end

get_weapon
1000.times do
  throw
end
