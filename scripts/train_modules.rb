require "defines"
require "helper"
require "armor"

def reset
  pause_rt
  until Status::standing
    put_wait "stand", /You stand|already standing/
    pause
  end
  if Spell::active.any?
    put_wait "khri stop", /unable to maintain|lose concentration|can no longer|attempt to relax/
    pause
  end
end

def stow
  sleep Rt::value
  put_wait "stow right", /Stow what|You put your/ unless Wield::right_noun.empty?
  put_wait "stow left", /Stow what|You put your/ unless Wield::left_noun.empty?
  pause
end

module Locksmith
  def self.before
    reset
  end

  def self.after
    pause 0.5
    stow
  end

  def self.do_run
    $args.clear
    load("bli.rb")
  end
end

module Outdoor
  def self.before
    reset
  end

  def self.do_run
    $args.clear << "rock"
    load("col.rb")
  end
end

module Utility
  def self.before
    put_wait "sit", /sit down|already sitting/ unless Status::sitting
    put_wait "hide", /Roundtime/ unless Status::hidden
    put_wait "khri darken dampen steady shadowstep sagacity", /Roundtime/
  end

  def self.after
    pause_rt
    reset
  end

  def self.do_run
    $args.clear
    load("med.rb")
  end
end

module Stealth
  def self.do_run
    load("h.rb")
  end
end

module Crossbow
  def self.before
    put_wait "stance custom", /now set to use your custom stance/
    stow
    put_wait "remove crossbow", /You sling a/
  end

  def self.after
    put_wait "wear crossbow", /You sling a/
    stow
    load("loot.rb")
  end

  def self.do_run
    load("lx.rb")
  end
end

module Debil
  def self.do_run
    load("test.rb")
  end
end