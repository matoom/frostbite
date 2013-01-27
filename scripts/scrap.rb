if $args.empty?
  echo "*** usage: .scrap &lt;skin&gt; ***"
  exit!
end

def finally_do
  put "stow scraper"
end

def stow item
  put "stow #{item}"
  match = { :next => ["You put your"],
            :end => ["You just can't"] }
  result = match_wait match

  case result
    when :end
      echo "*** Bag full! ***"
      exit
  end

  if Exp::state("skinning") > 32
    exit
  end
end

def get item
  put "get #{item} from my bundle"
  wait
end

def scrap item
  put "scrap #{item} with scraper"
  match = { :wait => [/\.\.\.wait|you may only type ahead 1 command/],
            :stow => ["clean as you can make it", "beyond repair"],
            :next => ["scrape your"],
            :end => ["What do you want to get?"] }
  result = match_wait match

  case result
    when :wait, :next
      scrap item
    when :stow
      stow item
      get item
      scrap item
    when :end
      exit
  end
end

put "get scraper"
wait
get $args.join("")
scrap $args.join("")

