100.times do
  put "khri meditate"
  echo "Inner magic: #{Exp::state("inner magic")}, Utility: #{Exp::state("utility")},  Augmentation: #{Exp::state("augmentation")}"
  wait_for_roundtime
end
