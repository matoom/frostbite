class String
  def camelize(space)
    self.split('_').map(&:capitalize).join(space)
  end

  def separate
    self.tr('_', ' ')
  end

  def to_schedule
    self.camelize('') << 'Schedule'
  end

  def to_task
    self.camelize('') << 'Task'
  end
end

module Train
  def self.rewrite_finally(obj)
    Thread.new {
      sleep 0.5
      Object.send(:alias_method, :finally_do_old, :finally_do)
      Object.send(:define_method, :finally_do) do
        Object.send(:finally_do_old)
        obj.send 'after' if obj.respond_to? 'after'
        Observer::instance.terminate
      end
    }
  end

  def self.schedule(schedule_name, &block)
    defined = Object.const_defined? schedule_name.to_s.to_schedule
    schedule = Object.const_get(schedule_name.to_s.to_schedule) if defined
    schedule.send 'set_up' if schedule.respond_to? 'set_up'
    block.call
    schedule.send 'tear_down' if schedule.respond_to? 'tear_down'
  end

  def self.monitor(task, threshold)
    Thread.new {
      while true
        sleep 15
        exp = get_exp task
        echo "exp #{task.to_s.separate} #{exp}/#{threshold}"
        Thread::main.raise if exp > threshold
      end
    }
  end

  def self.call(obj, monitor)
    rewrite_finally obj
    begin
      while true
        obj.send('run')
        pause 1
      end
    rescue
      monitor.raise
    end
  end

  def self.get_exp(task)
    Exp::state(task.to_s.camelize(' ').gsub(/\d+/, ""))
  end

  def self.run_lifecycle(&block)
    start = Time.now
    Object.send(:define_method, :finally_do) {}
    block.call
    Object.send(:finally_do)
    Object.send(:remove_method, :finally_do)
    (Time.now - start) / 60
  end

  def self.get_threshold(obj, default)
    obj.const_defined?(:THRESHOLD) ? obj.const_get(:THRESHOLD) : default
  end

  def self.run(schedule)
    total_times = []
    schedule.each do |task|
      obj = Object.const_get(task.to_s.to_task)

      threshold = get_threshold obj, 32
      next if get_exp(task) > threshold

      echo ":: #{task} ::"
      time = Train::run_lifecycle {
        obj.send 'before' if obj.respond_to? 'before'
        call obj, monitor(task, threshold)
      }
      total_times << {task => time}
      echo "<br/>Time elapsed #{total_times} minutes.<br/>"
    end
  end
end