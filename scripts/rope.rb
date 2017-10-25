@instrument = "zills"
@song = "fantasia"
@style = "masterful"

Client::track_exp("athletic")
Client::track_exp("arcana")
Client::track_exp("perform")

def get_rope
  put_wait "get rope", /You get/
end

def stow_rope
  put_wait "stow rope", /You put/
end

def stop_practice
  put_wait "stop climb", /You stop|You weren't/
  put_wait "stop play", /You stop/
end

def start_practice
  put_wait "play #{@song} #{@style} on my #{@instrument}", /You/
  put_wait "climb my rope", /begin to climb/
end

def finally_do
  Client::track_exp_clear
  stop_practice
  stow_rope
end

get_rope

100.times do
  put_wait 'gaze crystal', /You gaze/ unless Exp::state("arcana") > 30
  start_practice
  6.times do
    pause 20
    echo "Athletics: #{Exp::state("athletic")}/32"
    if Exp::state("Athletics") > 32
      echo "<br/>*** Finished! ***<br/>"
      exit
    end
  end
  stop_practice
end