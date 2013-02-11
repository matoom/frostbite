class Timer

  def repeat_every(interval)
    Thread.new do
      sleep interval
      loop do
        start_time = Time.now
        yield
        elapsed = Time.now - start_time
        sleep([interval - elapsed, 0].max)
      end
    end
  end

  def run
    5.times do
      match = private_methods.find { |e| e.to_s =~ /^every_(\d+)s_do/ }
      interval = match.to_s.scan(/\d+/).first.to_i

      if match and interval > 0
        @timer_thread = repeat_every(interval) do
          pid = Process.pid
          Process.detach(pid)
          Process.kill("INT", pid)
        end

        Signal.trap("INT") do
          start_time = Time.now
          send(match)
          #echo "#{Time.now} EXIT TRAP"
          $_interrupt_time = (Time.now - start_time).floor
        end

        break
      end
      pause 0.2
    end
  end

  def terminate_thread
    if defined? @timer_thread
      @timer_thread.join
    end
  end

end