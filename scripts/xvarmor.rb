require "defines"
require "helper"

load "vault"

ITEMS::ARMOR.each do |item|
  put_wait "take #{item} from vault", /You get|want to get/
  put_wait "wear #{item}", /You (slide|work|slip|put)|already|Wear what/
end

load "xvault"