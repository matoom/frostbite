class Target
  @auto = false
  @filter = ["leopard", "owl", "guard"]

  def self.auto usage_msg
    unless $args.first
      $args << find
      @auto = true
      unless $args.first
        echo usage_msg
        exit
      end
    end
  end

  def self.find
    monsters = Room::monsters_bold
    room = Room::objects
    return nil if room.scan("trying to remain hidden").length + room.scan("dead").length >= monsters.size
    monsters.each do |m|
      return m.split(' ').last unless @filter.all? { |e| m.include?(e) }
    end
    return nil
  end

  def self.is_auto
    @auto
  end
end