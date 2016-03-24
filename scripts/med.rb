35.times do
  put "khri meditate"
  echo "Inner Magic: #{Exp::state("inner magic")}, Utility: #{Exp::state("utility")},  Augmentation: #{Exp::state("augmentation")}"
  wait_for_roundtime
end
