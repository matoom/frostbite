class Target
  @auto = false
  @filter = ["leopard", "owl"]

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
    return nil if Room::count_objects("trying to remain hidden") >= monsters.size
    monsters.each do |m|
      return m.split(' ').last unless @filter.all? { |e| m.include?(e) }
    end
    return nil
  end

  def self.is_auto
    @auto
  end
end