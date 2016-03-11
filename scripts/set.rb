require "settings"

opts = $args.each_slice(2).to_a

def set_bool opt, val
  if ["true", "t", "on", "enabled", "1"].include? val
    eval "Settings::get.#{opt} = true"
    echo "#{opt}: true"
  else
    eval "Settings::get.#{opt} = false"
    echo "#{opt}: false"
  end
end

if opts.size.equal? 0
  echo Settings::get
else
  opts.each do |o|
    break if o.size != 2

    if o[0] == "hunt"
      set_bool "hunt", o[1]
    end
  end
end