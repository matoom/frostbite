def meditate
  put "khri meditate"
  match = { :wait => [/\.\.\.wait/],
            :end => [/thoughts are clear/],
            :continue => ["Roundtime"] }
  case match_wait match
    when :wait
      pause_rt
      meditate
    when :end
      throw :end
  end
end

catch (:end) do
  35.times do
    echo "Inner Magic: #{Exp::state("inner magic")}, Utility: #{Exp::state("utility")},  Augmentation: #{Exp::state("augmentation")}"
    meditate
  end
end
