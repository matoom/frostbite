require "defines"

$lob_weapon = $args.join(" ")
$is_left = true

unless $lob_weapon
  echo "Throw what?"
  exit
end

if Wield::right_noun.eql? '' or Wield::right_noun.include? $lob_weapon
  $is_left = false
end

def finally_do
  pause_rt
  stow_weapon
  Client::track_exp_clear
end

def stow_weapon
  put "stow #{$lob_weapon}"
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
  unless Wield::left_noun.include? $lob_weapon
    put "get #{$lob_weapon}"
    match = { :wait => [/\.\.\.wait/],
              :stunned => [/still stunned|entangled in a web/],
              :continue => ["You get", "You pick up", "from your belt"] }
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

def lob hand
  put "lob #{hand}"
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
  lob $is_left ? "left" : "right"
end
