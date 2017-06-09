require 'train_core'
require 'train_tasks'

schedule_name = $args.first

schedules = {
    :util => [:utility, :locksmith, :outdoor],
    :int => [:stealth, :crossbow, :brawling],
    :thrown => [:light_thrown, :heavy_thrown],
    :juvie => [:twohanded, :pole, :tactics],
    :test => [:utility, :outdoor, :utility],
    :test2 => [:debil, :outdoor]
}

if schedule_name and schedules[schedule_name.to_sym]
  tasks = schedules[schedule_name.to_sym]
  echo "Training skills - #{tasks.inspect}"
  Train::schedule(schedule_name) {
    Train::run tasks
  }
else
  echo "Schedule '#{schedule_name}' not found"
end