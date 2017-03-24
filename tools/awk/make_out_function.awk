BEGIN \
{
  found = 0
}

found == 0 && /^_LIBRARY_FUNCTION_/ \
{
  sub(/^_LIBRARY_FUNCTION_/, "")
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
