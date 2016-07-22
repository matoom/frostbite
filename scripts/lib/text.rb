# Waits for roundtime and pauses for the duration.
#
# @return [void]
# @example Wait for the duration of round time before executing next command.
#   put "hide"
#   wait_for_roundtime
#   put "unhide"
def wait_for_roundtime
  (0..1000000).each do
    line = api_sync_read
    if line
      if line.match(/Roundtime/)
        api_sleep line[/\d+/].to_i + $rt_adjust
        return
      end
    end
    sleep 0.01
  end
end

# Runs until matching regex pattern is found in game text.
#
# @param [String] pattern regex pattern.
# @return [void]
def wait_for(pattern)
  if pattern.is_a?(Array)
    pattern = Regexp.new(pattern.join('|'))
  end

  (0..1000000).each do
    line = api_sync_read
    if line
      if line.match(pattern)
        return
      end
    end
    sleep 0.01
  end
end

# Matches regex patterns and returns the name of the matching pattern.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Symbol] name of the regex pattern
# @example Match for game text.
#   match = { :retry => [/\.\.\.wait/], :open => [/you open/] }
#   result = match_wait match
#   result #=> :retry or :open
#   case result
#     when :open
#       echo "open!"
#   end
def match_wait(pattern)
  validate_pattern pattern
  $_api_exec_state = :match_wait

  match = api_get_match pattern, Hash[pattern.keys.collect{ |v| [v, ""] }]
  match.find{ |k, v| !v.empty? }.first
end

# Matches regex patterns and returns pattern name along with matched text
#
# @param [Hash] pattern list of regex patterns and names
# @return [Hash] name and text from the matching pattern
# @example Retrieve the text from a matching pattern.
#   put "open my trunk"
#   match = { :m => [/you open/i] }
#   result = match_get match
#   result #=> {:key =>:m, :match => "You open the steel trunk..."}
def match_get(pattern)
  validate_pattern pattern

  $_api_exec_state = :match_get

  match = api_get_match pattern, Hash[pattern.keys.collect{ |v| [v, ""] }]
  m = match.find{ |k, v| !v.empty? }
  {:key => m[0], :match => m[1]}
end

# Match get for multiple lines.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Hash] found matches arranged by pattern names
# @example Retrieve text from matching patterns.
#    put "health"
#    match = {:vitals => [/Your/],
#             :scars => [/You have/],
#             :match_end => [/>|\.\.\.wait|you may only type ahead/]}
#    result = match_get_m match
#    result #=>
#    #{:vitals=>["Your body feels very beat up.", "Your spirit feels full of life."],
#    # :scars=>["You have some tiny scratches to the neck."], :match_end=>[">"]}
def match_get_m(pattern)
  validate_get_m validate_pattern pattern

  $_api_exec_state = :match_get_m

  match = api_get_match pattern, Hash[pattern.keys.collect{ |v| [v, []] }]
  match.delete_if{ |k, v| v.empty? }
end

# Matches regex patterns and calls the defined label.
#
# @param [Hash] pattern list of regex patterns and names
# @return [Void]
# @example Using match patterns and jumping to labels on a successful match.
#   labels_start
#
#   label(:retry){
#     match = { :retry => [/\.\.\.wait/], :next => [/you open/] }
#     match_wait_goto match
#   }
#
#   label(:next){
#     echo "next"
#   }
#
#   labels_end
def match_wait_goto(pattern)
  $_api_exec_state = :match_wait_goto

  match = api_get_match pattern, Hash[pattern.keys.collect{ |v| [v, ""] }]
  goto match.find{ |k, v| !v.empty? }.first
end

# Waits for a prompt character.
#
# @return [void]
# @example Using wait in script to run consecutive commands.
#   put "remove my shield"
#   wait
#   put "wear my shield"
#   wait
#   put "remove my shield"
def wait
  $_api_queue.clear
  wait_for(/>/)
end

# @private
def validate_pattern pattern
  if pattern.nil? or pattern.empty?
    raise "MatchError: match pattern is not specified."
  end
  pattern
end

# @private
def validate_get_m pattern
  unless pattern.has_key?(ApiSettings::MATCH_END_KEY)
    raise "MatchError: match pattern does not" +
              "contain '#{ApiSettings::MATCH_END_KEY}' end condition."
  end
  pattern
end

# @private
def api_match_start pattern
  if pattern.has_key?(ApiSettings::MATCH_START_KEY)
    wait_for(pattern[ApiSettings::MATCH_START_KEY]);
  end
end

# @private
def api_get_match pattern, match
  api_match_start pattern

  match_found, rt = false, 0

  (0..1000000).each do
    line = api_sync_read
    if line
      unless match_found
        match_found = api_match match, line, pattern
      end

      rt += api_read_rt line

      if match_found
        $_api_exec_state = :idle

        if />/ =~ line
          api_sleep rt
          return match
        end
      end
    end
    sleep 0.01
  end
end

# @private
def api_match match, line, pattern
  pattern.each_pair do |key, value|
    value.each do |m|
      if line.match(m)
        match[key] << line
        return pattern.has_key?(ApiSettings::MATCH_END_KEY) ?
            key == ApiSettings::MATCH_END_KEY : true
      end
    end
  end
  false
end

# @private
def api_read_rt line
  line.match(/Roundtime/) ? line[/\d+/].to_i + $rt_adjust : 0
end
