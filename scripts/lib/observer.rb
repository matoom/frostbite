require 'singleton'

class Observer
  include Singleton

  # @private
  @@observer_thread = []
  # @private
  @@events = []
  # @private
  @@terminated = false

  # @private
  def initialize
    init
  end

  # @private
  def init
    @@terminated = false

    @@observer_thread = Thread.new(@@events) { |events|
      $_observer_started = true
      until @@terminated
        if events.size > 0
          while $_observer_queue.size > 0
            text = $_observer_queue.pop
            events.each do |event|
              event.each_pair do |k, v|
                if text.match(v)
                  observer_event k
                end
              end
            end
          end
        end
        pause 0.1
      end
      $_observer_started = false
    }
  end

  # Register an observer event
  #
  # @param [hash] event observer event.
  # @return [void]
  # @example Registering an event.
  #   Observer.instance.register_event({ :hello => "Roundtime" })
  #
  #   def hello
  #     echo "hello"
  #   end
  def register_event event
    @@events << event
  end

  # Terminate the observer execution
  #
  # @param
  # @return [void]
  # @example Terminating the observer
  #   Observer.instance.terminate
  #
  def terminate
    @@terminated = true
  end

  # @private
  private
  def observer_event method_name
    Signal.trap("INT") do
      start_time = Time.now
      send(method_name)
      $_interrupt_time = (Time.now - start_time).floor
    end

    pid = Process.pid

    while $_exec_status != :running
      sleep 0.1
    end

    Process.detach(pid)
    Process.kill("INT", pid)
  end

end