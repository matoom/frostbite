@minerals = ["nugget", "bar"]

@minerals.each { |mineral|
  100.times do
    put "get #{mineral}"
    match = { :wait => [/\.\.\.wait|you may only type ahead 1 command/],
              :sell => ["You get"],
              :end => ["What were you referring"] }
    result = match_wait match

    case result
      when :wait
        pause 0.5
      when :sell
        put "sell #{mineral}"
      when :end
        break
    end
  end
}