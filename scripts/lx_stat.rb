class Stats
  @load_time = 6
  @start_time = Time.now
  @shot_count = @total_shot_count = 0.0
  @kill_count = @time = @total_time = 0

  def self.register_shot
    @shot_count += 1
  end

  def self.register_kill
    self.count_totals
    self.report_stats
  end

  def self.reset_timer
    @start_time = Time.now
    @shot_count = 0
  end

  def self.report_total_stats
    echo "Total kills: #{@kill_count}"
    echo "Avg. Time to kill: #{@total_time / @kill_count}"
    echo "Avg. Shots to kill: #{@total_shot_count / @kill_count}"
  end

  def self.report_stats
    echo "Time to kill: #{@time}"
    echo "Shots to kill: #{@shot_count}"
  end

  private

  def self.count_totals
    @total_shot_count += @shot_count
    @time = Time.now - @start_time - @load_time
    @total_time += @time
    @kill_count += 1
  end
end