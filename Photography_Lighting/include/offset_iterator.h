#ifndef _OFFSET_ITR_H_
#define _OFFSET_ITR_H_

#include <iterator>
#include <boost/iterator/iterator_facade.hpp> 
#include <boost/iterator/iterator_traits.hpp>


///////////////////////////////////////////////////////////////////
// OffsetIterator return the (*itr)[n] when dereferencing (itr). //
// This is usefull for example to iterate over a particular color//
// channel in an image.                                          //
///////////////////////////////////////////////////////////////////

template<typename T>
class offset_iterator;


/*****************************************************************/


template<typename Iterator> 
class const_offset_iterator : public boost::iterator_facade< const_offset_iterator<Iterator>, const typename std::iterator_traits<Iterator>::value_type::value_type, std::random_access_iterator_tag >
{ 
 public:
  typedef std::random_access_iterator_tag                                       category;
  typedef const typename std::iterator_traits<Iterator>::value_type::value_type value_type;
  typedef value_type&                                                           reference;
  typedef value_type*                                                           pointer;
  typedef signed int                                                            difference_type;

  const_offset_iterator(void) : _itr(0), _offset(0) {}
  explicit const_offset_iterator(const Iterator& itr, signed int offset) : _itr(itr), _offset(offset) {}
  const_offset_iterator(const const_offset_iterator& src) : _itr(src._itr), _offset(src._offset) {}

  operator Iterator() const { return _itr; }

 private:
  friend class boost::iterator_core_access;
  friend class offset_iterator<Iterator>;

  bool equal(const const_offset_iterator<Iterator>& other) const { return (this->_itr == other._itr) && (this->_offset == other._offset); }

  void increment(void)             { this->_itr++; }
  void decrement(void)             { this->_itr--; }
  void advance(difference_type& n) { this->_itr += n; }

  difference_type distance_to(const const_offset_iterator<Iterator>& other) const { return std::distance(this->_itr, other._itr); }

  reference dereference(void) const { return (*_itr)[_offset]; }

  Iterator _itr;
  signed int _offset;
}; 


/******************************************************************/


template<typename Iterator> 
class offset_iterator : public boost::iterator_facade< offset_iterator<Iterator>, typename std::iterator_traits<Iterator>::value_type::value_type, std::random_access_iterator_tag >
{ 
 public:
  typedef std::random_access_iterator_tag                                       category;
  typedef typename std::iterator_traits<Iterator>::value_type::value_type       value_type;
  typedef value_type&                                                           reference;
  typedef value_type*                                                           pointer;
  typedef signed int                                                            difference_type;

  offset_iterator(void) : _itr(0), _offset(0) {}
  explicit offset_iterator(const Iterator& itr, signed int offset) : _itr(itr), _offset(offset) {}
  offset_iterator(const offset_iterator& src) : _itr(src._itr), _offset(src._offset) {}

  operator const_offset_iterator<Iterator>() const { return const_offset_iterator<Iterator>(_itr, _offset); }
  operator Iterator() const { return _itr; }

 private:
  friend class boost::iterator_core_access;

  bool equal(const offset_iterator<Iterator>& other) const       { return (this->_itr == other._itr) && (this->_offset == other._offset); }
  bool equal(const const_offset_iterator<Iterator>& other) const { return (this->_itr == other._itr) && (this->_offset == other._offset); }

  void increment(void)             { this->_itr++; }
  void decrement(void)             { this->_itr--; }
  void advance(difference_type& n) { this->_itr += n; }

  difference_type distance_to(const offset_iterator<Iterator>& other) const       { return std::distance(this->_itr, other._itr); }
  difference_type distance_to(const const_offset_iterator<Iterator>& other) const { return std::distance(this->_itr, other._itr); }

  reference dereference(void) const { return (*_itr)[_offset]; }

  Iterator _itr;
  signed int _offset;
}; 


#endif /* _OFFSET_ITR_H_ */
