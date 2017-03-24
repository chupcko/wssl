/^_INCLUDE_BEGIN_$/, /^_INCLUDE_END_$/ \
{
  if(!/^_INCLUDE_(BEGIN|END)_$/)
    print
}
