class Settings
  FILE_NAME = "settings.dat"

  class SettingsObject
    def initialize
      @last_hunt = Time.now
    end
    attr_accessor :last_hunt
  end

  at_exit { self.marshal_dump }

  def self.marshal_load
    begin
      @settings = Marshal.load(File.read(FILE_NAME))
    rescue
      @settings = SettingsObject.new
    end
  end

  if @settings.nil?
    self.marshal_load
  end

  def self.marshal_dump
    open(FILE_NAME, 'wb') { |f| f.puts Marshal.dump(@settings) }
  end

  def self.get
    @settings
  end
end