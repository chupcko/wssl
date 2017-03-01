BEGIN \
{
  found = 0
}

found == 0 && /^_INCLUDE_BEGIN_$/ \
{
  found = 1
  next
}

found != 0 && /^_INCLUDE_END_$/ \
{
  found = 0
  next
}

found != 0
