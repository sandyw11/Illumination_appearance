#ifndef _TEMPARRAY_H_
#define _TEMPARRAY_H_

/****************************************/
/* Runtime-determined fixed size arrays */
/****************************************/

namespace detail {

  template<typename T>
    class tempArray {

  public:
    /////////////////
    // Constructor //
    /////////////////
    tempArray(unsigned int size)         { data = new T[size]; }
    ~tempArray(void)                     { delete[] data; }

    /////////////
    // Methods //
    /////////////
             operator T*(void) const     { return &data[0]; }
             operator T*(void)           { return &data[0]; }

    const T& operator[](int index) const { return data[index]; }
    T&       operator[](int index)       { return data[index]; }

    //////////////////
    // Private Data //
    //////////////////
  private:
    T *data;
  };

}

#define tempArray(type, name, size)    ::detail::tempArray<type> name(size)
#define tempArrayBegin(type, name, size) (type*)(&(name[0]))
#define tempArrayEnd(type, name, size)   (type*)(&(name[size]))

#endif /* _TEMPARRAY_ */
