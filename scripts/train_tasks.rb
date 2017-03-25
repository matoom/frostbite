require "defines"
require "helper"
require "armor"

module WarkSchedule
  def self.set_up
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/ if Wield::right_noun.empty?
    put_wait 'khri start strike flight focus prowess guile hasten', /already using|lose concentration|Roundtime/
  end
end

module JuvieSchedule
  def self.set_up
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/ if Wield::right_noun.empty?
    TrainUtils::khri_offensive
  end
end

module IntSchedule
  def self.set_up
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/ if Wield::right_noun.empty?
    TrainUtils::khri_defensive
  end
end

module LocksmithTask
  def self.before
    TrainUtils::stow
  end

  def self.after
    pause 0.5
    TrainUtils::stow
  end

  def self.run
    $args.clear
    load('bli.rb')
  end
end

module OutdoorTask
  def self.before
    TrainUtils::reset
  end

  def self.run
    $args.clear << 'rock'
    load('col.rb')
  end
end

module UtilityTask
  def self.before
    put_wait 'sit', /sit down|already sitting/ unless Status::sitting
    put_wait 'hide', /Roundtime/ unless Status::hidden
    put_wait 'khri darken dampen steady shadowstep sagacity', /Roundtime|You're already using/
  end

  def self.after
    pause Rt::value
    TrainUtils::reset
    TrainUtils::reset_spell
  end

  def self.run
    load('med.rb')
  end
end

module StealthTask
  def self.run
    load('h.rb')
  end
end

module CrossbowTask
  def self.before
    put_wait 'stance custom', /now set to use your custom stance/
    put_wait 'khri steady', /already using|lose concentration|Roundtime/
    TrainUtils::stow
    put_wait 'remove crossbow', /You sling a/
  end

  def self.after
    pause Rt::value
    put_wait 'khri stop steady', /attempt to relax/
    put_wait 'wear crossbow', /You sling a/ unless Wield::right_noun.empty?
    TrainUtils::stow
    load('loot.rb')
  end

  def self.run
    load('lx.rb')
  end
end

module DebilTask
  def self.run
    load('test.rb')
  end
end

module BrawlingTask
  def self.before
    TrainUtils::stow_left
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/ if Wield::right_noun.empty?
    put_wait 'stance evasion', /now set to use your/
  end

  def self.after
    pause Rt::value
    TrainUtils::stow_left
    load('loot.rb')
  end

  def self.run
    load('brawl.rb')
  end
end

module LightThrownTask
  def self.before
    $args.clear
    TrainUtils::stow_left
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/ if Wield::right_noun.empty?
  end

  def self.run
    load('lt.rb')
  end
end

module HeavyThrownTask
  def self.before
    $args.clear
    TrainUtils::stow_left
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/ if Wield::right_noun.empty?
  end

  def self.run
    load('ht.rb')
  end
end

module TacticsTask
  def self.before
    $args.clear
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/ unless Wield::right_noun.include? TrainUtils::DEFAULT_WEAPON
    TrainUtils::stow_left
  end

  def self.after
    pause Rt::value
    load('loot.rb')
  end

  def self.run
    load('anal.rb')
  end
end

module TwohandedTask
  WEAPON = 'axe'
  CONTAINER = 'backsheath'

  def self.before
    Client::track_exp "Twohanded Edged"
    put_wait "get #{WEAPON} from my #{CONTAINER}", /You get/ unless Wield::right_noun.include? WEAPON
    put_wait 'swap', /You move/ if Wield::left_noun.include? WEAPON
    TrainUtils::stow_left
  end

  def self.after
    pause Rt::value
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/
    put_wait 'swap', /You move/
    put_wait "put #{WEAPON} in my #{CONTAINER}", /You put/
    Client::track_exp_clear
    load('loot.rb')
  end

  def self.run
    load('att.rb')
  end
end

module PoleTask
  WEAPON = 'scythe'
  CONTAINER = 'willowy toolstrap'

  def self.before
    Client::track_exp "Polearms"
    put_wait "untie my #{WEAPON}", /You remove/ unless Wield::right_noun.include? WEAPON
    put_wait 'swap', /You move/ if Wield::left_noun.include? WEAPON
    TrainUtils::stow_left
  end

  def self.after
    pause Rt::value
    put_wait "get #{TrainUtils::DEFAULT_WEAPON}", /You get/
    put_wait 'swap', /You move/
    put_wait "tie my #{WEAPON} to my #{CONTAINER}", /You attach/
    Client::track_exp_clear
    load('loot.rb')
  end

  def self.run
    load('att.rb')
  end
end

module TrainUtils
  DEFAULT_WEAPON = 'telek'

  def self.reset
    pause_rt
    until Status::standing
      put_wait 'stand', /You stand|already standing/
      pause
    end
  end

  def self.reset_spell
    if Spell::active.any?
      put_wait 'khri stop', /unable to maintain|lose concentration|can no longer|attempt to relax/
      pause
    end
  end

  def self.khri_offensive
    put_wait 'khri start strike flight focus prowess guile hasten', /already using|lose concentration|Roundtime/
  end

  def self.khri_defensive
    put_wait 'khri hasten elusion avoidance flight plunder prowess', /already using|lose concentration|Roundtime/
  end

  def self.stow
    pause_rt
    stow_left
    stow_right
    pause
  end

  def self.stow_left
    put_wait 'stow left', /Stow what|You put your/ unless Wield::left_noun.empty?
  end

  def self.stow_right
    put_wait 'stow right', /Stow what|You put your/ unless Wield::right_noun.empty?
  end
end