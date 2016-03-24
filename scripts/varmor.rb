require "defines"
require "helper"

load "vault"

ITEMS::ARMOR.each do |item|
  put_wait "remove my #{item}", /You (take|pull|loosen|work)|Remove what|referring to/
  put_wait "put my #{item} in vault", /You put|referring to/
end

load "xvault"
