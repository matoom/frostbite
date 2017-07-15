require 'socket'

file_path = 'mock.xml'

server = TCPServer.new("127.0.0.1", 7900)

loop {
  client = server.accept

  while line = client.gets
    line = line.chomp

    puts line

    if line.start_with? "K"
      client.puts "AqD^xTbQeVgasm]kEXjJDzX\ferZLMaD\n"
    elsif line.start_with? "A"
      client.puts "A\tTEST\tKEY\t830aeadfdc81a8e4cb629994880db3f3\tTEST\n"
    elsif line.start_with? "M"
      client.puts "M	CS	CyberStrike 	DR	DragonRealms	DRD	DragonRealms Development	DRF	DragonRealms The Fallen	DRT	DragonRealms Prime Test	DRX	DragonRealms Platinum	GS3	GemStone IV	GS4D	GemStone IV Development	GSF	GemStone IV Shattered	GST	GemStone IV Prime Test	GSX	GemStone IV Platinum\n"
    elsif line.start_with? "F"
      client.puts "F	NORMAL"
    elsif line.start_with? "G"
      client.puts "G	DragonRealms	NORMAL	0		ROOT=sgc/dr	MKTG=info/default.htm	MAIN=main/default.htm	GAMEINFO=information/default.htm	PLAYINFO=main/default.htm	MSGBRD=message/default.htm	CHAT=chat/default.htm	FILES=files/default.htm	COMMING=main/default.htm	STUFF=main/comingsoon.htm	BILLINGFAQ=account/default.htm	BILLINGOPTIONS=offer/payment.htm	LTSIGNUP=https://account.play.net/simunet_private/cc-signup.cgi	BILLINGINFO=http://account.play.net/simunet_private/acctInfo.cgi?key={KEY}&SITE=sgc	GAMES=main/games.htm	FEEDBACK=feedback/default.htm	MAILFAIL=/sgc/dr/feedback/mailfail.htm	MAILSUCC=/sgc/dr/feedback/mailsent.htm	MAILSERVE=SGC	SIGNUP=http://ad-track.play.net/sgc/signup_redirect.cgi	SIGNUPA=http://ad-track.play.net/sgc/signup_again.cgi"
    elsif line.start_with? "P"
      client.puts "P	DR	1495	DR.EC	250	DR.P	2500"
    elsif line.start_with? "C"
      client.puts "C	3	16	1	1	W_ACCTNAME_000	Char1	W_ACCTNAME-002	Char2	W_ACCTNAME_003	-Char3-"
    elsif line.start_with? "L"
      client.puts "L	OK	UPPORT=5535	GAME=STORM	GAMECODE=DR	FULLGAMENAME=StormFront	GAMEFILE=STORMFRONT.EXE	GAMEHOST=127.0.0.1	GAMEPORT=7900	KEY=830aeadfdc81a8e4cb629994880db3f3"
    elsif line.start_with? "<c>/FE:FROSTBITE"
      i, chunk = 0, ''
      File.open(file_path).read.each_line do |row|
        if row.include? "<prompt"
          chunk += row.gsub!(/\n$/, "\r\n")
          client.puts chunk
          chunk.clear
          sleep 0.2
        else
          chunk += row.gsub!(/\n$/, "\r\n")
        end
        i += 1
      end
    end

    break if line =~ /^\s*$/
  end
}
