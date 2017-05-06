require 'singleton'

class Observer
  include Singleton

  # @private
  def initialize
    @events = []
    @timers = {}
    @terminated = true
    @interrupted = false
  end

  # @private
  def init
    @terminated = false
    @observer_thread = Thread.new {
      $_api_observer_started = true
      until @terminated or @events.size < 1
        while text = sync_read
          return unless text
          @events.each do |event|
            event.each_pair do |k, v|
              if text.match(v)
                sleep 0.1 while @interrupted
                observer_event k, text
              end
            end
          end
        end
        sleep 0.1
      end
      reset_data
    }
  end

  # Call event from code
  # @param [hash] event callback method name
  # @param [string] text var string passed on to event method
  # @return [void]
  def call_event(event, text)
    t = Thread.new do
      sleep 0.1 while @interrupted
      observer_event event, text
    end
    t.join
  end

  # Register timer event
  # @param [symbol] event observer event/callback method name
  # @param [number] time event interval in seconds
  # @param [string] text user data
  def register_timer(time, event, text = '')
    remove_timer(event)
    @timers.store(event, Thread.new do
      sleep 0.1 while @interrupted
      while true
        observer_event event, text
        sleep time
      end
    end)
  end

  # Remove timer event
  # @param [symbol] event observer event/callback method name
  def remove_timer(event)
    if @timers.key?(event)
      timer = @timers.fetch(event)
      timer.terminate if timer
      @timers.delete(event)
    end
  end

  # Register an observer event
  #
  # @param [hash] event observer event/callback method name with match text
  # @return [void]
  # @example Registering an event.
  #   Observer.instance.register_event({ :hello => "Roundtime" })
  #
  #   def hello
  #     echo "hello"
  #   end
  def register_event(event)
    init if @terminated
    event.each { |k, v|
      if v.is_a?(Array)
        event[k] = Regexp.new(v.join('|'))
      end
    }
    @events << event
  end

  # Terminate the observer execution
  #
  # @return [void]
  # @example Terminating the observer
  #   Observer.instance.terminate
  #
  def terminate
    if @observer_thread
      @observer_thread.terminate
    end
    stop_timers
    reset_data
  end

  # Stop observer gracefully and finish
  # events before exit
  #
  # @return [void]
  # @example Stopping the observer
  #   Observer.instance.stop
  #
  def stop
    @terminate = true
    stop_timers
  end

  def reset_data
    @events.clear
    @terminate = true
    @interrupted = false
    $_api_observer_started = false
    sync_clear
  end

  def stop_timers
    @timers.each do |e, t|
      t.terminate
    end
  end

  # Synchronize round times with main
  # thread / pause for the duration of
  # round time before calling the block
  #
  # @param [hash] block block reference
  # @return [void]
  # @example synchronize round times
  #   Observer::sync_rt { echo "ready" }
  def self.sync_rt(&block)
    pause_rt
    block.call
  end

  # @private
  def sync_read
    $_api_gets_mutex.synchronize do
      $_api_observer_queue.shift
    end
  end

  # @private
  def sync_clear
    $_api_gets_mutex.synchronize do
      $_api_observer_queue.clear
    end
  end

  # @private
  # measure "block" execute time and
  # register as api interrupt
  def sync_main(&block)
    interrupt_start = Time.now
    block.call
    $_api_interrupt_time = (Time.now - interrupt_start).floor
  end

  # @private
  # register software interrupt "INT" and
  # call event method for the duration of the interrupt, always
  # waits until all matching functions are finished working
  # before calling the interrupt
  def observer_event(method_name, text)
    configure_trap method_name, text
    sleep 0.1 while $_api_exec_state != :none
    Process.kill("INT", Process.pid)
  end

  # @private
  def configure_trap(method_name, text)
    @interrupted = true
    Signal.trap("INT") do
      interrupt = Thread.new {
        sync_main { call_method method_name, text }
      }
      interrupt.join
      @interrupted = false
    end
  end

  # @private
  def call_method(method_name, text)
    if method(method_name).parameters.empty?
      send(method_name)
    else
      send(method_name, text)
    end
  end

  private :init, :sync_read, :observer_event, :call_method, :sync_main
end