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
  @@interrupted = false

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
                  pause 0.1 while @@interrupted
                  observer_event k, text
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
    event.each { |k, v|
      if v.is_a?(Array)
        event[k] = Regexp.new(v.join('|'))
      end
    }
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
  def observer_event(method_name, text)
    @@interrupted = true
    Signal.trap("INT") do
      start_time = Time.now
      call_method method_name, text
      $_api_interrupt_time = (Time.now - start_time).floor
      @@interrupted = false
    end

    pid = Process.pid
    sleep 0.1 while $_api_exec_state != :idle

    Process.detach(pid)
    Process.kill("INT", pid)
  end

  # @private
  def call_method(method_name, text)
    if method(method_name).parameters.empty?
      send(method_name)
    else
      send(method_name, text)
    end
  end

  private :init, :sync_read, :observer_event, :call_method

end