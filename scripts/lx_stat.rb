class Stats
  @start_time = Time.now
  @shot_count = @total_miss_count = @total_shot_count = 0.0
  @kill_count = @miss_count = @current_time = @total_time = 0

  def self.register_shot
    @shot_count += 1
  end

  def self.register_miss
    @miss_count += 1
  end

  def self.register_kill
    self.count_totals
    self.report_stats
    @shot_count = 0
    @miss_count = 0
    @current_time = 0
  end

  def self.reset_timer
    @current_time += Time.now - @start_time
    @start_time = Time.now
  end

  def self.report_total_stats
    if @kill_count > 0
      echo "Total kills: #{@kill_count}"
      echo "Total shots: #{@total_shot_count}"
      echo "Total missed: #{@total_miss_count}"
      echo "Avg time to kill: #{@total_time / @kill_count}"
      echo "Avg shots to kill: #{@total_shot_count / @kill_count}"
      echo "Avg missed shots: #{@total_miss_count / @kill_count}"
      if @total_miss_count > 0
        echo "Miss rate: #{((@total_miss_count * 100) / @total_shot_count).round(2)}%"
      else
        echo "Miss rate: 0%"
      end
    end
  end

  def self.report_stats
    echo "Time to kill: #{@current_time}"
    echo "Shots to kill: #{@shot_count}"
    echo "Missed shots: #{@miss_count}"
  end

  private

  def self.count_totals
    @total_shot_count += @shot_count
    @total_miss_count += @miss_count
    @total_time += @current_time
    @kill_count += 1
  end
end