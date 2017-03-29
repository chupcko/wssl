BEGIN \
{
  found = 0
}

found == 0 && /^(_LIBRARY)?_FUNCTION_/ \
{
  sub(/^(_LIBRARY)?_FUNCTION_/, "")
  printf "extern %s", $0
  found = 1
  next
}

found != 0 && /^{/ \
{
  found = 0
  next
}

found != 0 \
{
  sub(/\)/, ");")
  print
}
