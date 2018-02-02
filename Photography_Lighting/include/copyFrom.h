#ifndef _COPYFROM_H_
#define _COPYFROM_H_

/////////////////////////////////////////////////////////////////////
// Similar to STL copy, but instead of giving the source begin and //
// end iterators, this version relies on the destination begin and //
// end to determine the flow.                                      //
/////////////////////////////////////////////////////////////////////

template<typename InputIterator, typename OutputIterator>
   void copyFrom(InputIterator input, OutputIterator first, OutputIterator last)
{
  // copy
  for( ; first != last; ++first, ++input)
    *first = *input;

  // done.
}

#endif /* _COPYFROM_H_ */
