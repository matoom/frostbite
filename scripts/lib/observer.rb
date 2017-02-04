require 'singleton'

class Observer
  include Singleton

  # @private
  def initialize
    @api_observer_thread = []
    @events = []
    @terminated = false
    @interrupted = false
    init
  end

  # @private
  def init
    @terminated = false

    @api_observer_thread = Thread.new(@events) { |events|
      $_api_observer_started = true
      until @terminated
        if events.size > 0
          while text = sync_read
            return unless text
            events.each do |event|
              event.each_pair do |k, v|
                if text.match(v)
                  pause 0.1 while @interrupted
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

  # Call event from code
  # @param [hash] event observer event
  # @param [string] text var string passed on to event method
  # @return [void]
  def call_event(event, text)
    t = Thread.new do
      pause 0.1 while @interrupted
      observer_event event, text
    end
    t.join
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
    @events << event
  end

  # Terminate the observer execution
  #
  # @return [void]
  # @example Terminating the observer
  #   Observer.instance.terminate
  #
  def terminate
    @terminated = true
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
    @interrupted = true
    Signal.trap("INT") do
      interrupt = Thread.new {
        sync_main { call_method method_name, text }
        @interrupted = false
      }
      interrupt.join
    end

    pid = Process.pid
    sleep 0.1 while $_api_exec_state != :none

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

  private :init, :sync_read, :observer_event, :call_method, :sync_main

end