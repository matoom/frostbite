if Status::hidden
  put "khri eliminate"
  put "backstab #{$args.first}"  
  wait_for_roundtime
  put "thrust #{$args.first}"
else
  echo "*** have to be hidden! ***"
end