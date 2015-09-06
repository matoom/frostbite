require 'singleton'

class Observer
  include Singleton

  # @private
  @@api_observer_thread = []
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

    @@api_observer_thread = Thread.new(@@events) { |events|
      $_api_observer_started = true
      until @@terminated
        if events.size > 0
          text = sync_read
          if text
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
      $_api_observer_started = false
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
  def sync_read
    $_api_gets_mutex.synchronize do
      $_api_observer_queue.shift
    end
  end

  # @private
  private
  def observer_event method_name
    Signal.trap("INT") do
      start_time = Time.now
      send(method_name)
      $_api_interrupt_time = (Time.now - start_time).floor
    end

    pid = Process.pid

    while $_api_exec_state != :idle
      sleep 0.1
    end

    Process.detach(pid)
    Process.kill("INT", pid)
  end

end