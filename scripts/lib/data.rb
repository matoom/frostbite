require 'dl'
require 'dl/import'

# @api private
module GameData
  extend DL::Importer
  dlload 'data.dll'

  extern 'int getExpRank(const char[])'
  extern 'int getExpState(const char[])'

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