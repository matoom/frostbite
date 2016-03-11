require 'dl'
require 'dl/import'

# @api private
module OS
  def OS.windows?
    (/cygwin|mswin|mingw|bccwin|wince|emx/ =~ RUBY_PLATFORM) != nil
  end

  def OS.mac?
   (/darwin/ =~ RUBY_PLATFORM) != nil
  end

  def OS.unix?
    !OS.windows?
  end

  def OS.linux?
    OS.unix? and not OS.mac?
  end
end

# @api private
module GameData
  extend DL::Importer
  if OS::windows?
    dlload 'shared.dll'
  elsif OS::linux?
    dlload "#{Dir.pwd}/libshared.so"
  elsif OS::mac?
    dlload "libshared.dylib"
  end

  extern 'int getExpRank(const char[])'
  extern 'int getExpState(const char[])'

  extern 'char* getActiveSpells()'

  extern 'char* getInventory()'
  extern 'char* getContainer()'

  extern 'char* getWieldRight()'
  extern 'char* getWieldRightNoun()'
  extern 'char* getWieldLeft()'
  extern 'char* getWieldLeftNoun()'

  extern 'int getHealth()'
  extern 'int getConcentration()'
  extern 'int getSpirit()'
  extern 'int getFatigue()'

  extern 'char* getRoomTitle()'
  extern 'char* getRoomDescription()'
  extern 'char* getRoomObjects()'
  extern 'char* getRoomPlayers()'
  extern 'char* getRoomExits()'

  extern 'unsigned char getStanding()'
  extern 'unsigned char getSitting()'
  extern 'unsigned char getKneeling()'
  extern 'unsigned char getProne()'
  extern 'unsigned char getStunned()'
  extern 'unsigned char getBleeding()'
  extern 'unsigned char getHidden()'
  extern 'unsigned char getInvisible()'
  extern 'unsigned char getWebbed()'
  extern 'unsigned char getJoined()'
  extern 'unsigned char getDead()'

  extern 'int getRt()'
end