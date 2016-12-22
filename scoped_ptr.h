//
// Created by Надежда on 22.12.16.
//

#ifndef STRINGCALCULATOR_SCOPED_PTR_H
#define STRINGCALCULATOR_SCOPED_PTR_H

namespace sharik {

  template<class T> class scoped_ptr{

   public:
     typedef T element_type;

     explicit scoped_ptr(T * p = 0); // never throws
     ~scoped_ptr(); // never throws

     void reset(T * p = 0); // never throws

     T & operator*() const; // never throws
     T * operator->() const; // never throws
     T * get() const; // never throws

     void swap(scoped_ptr & b); // never throws
  };

  template<class T> void swap(scoped_ptr<T> & a, scoped_ptr<T> & b); // never throws

}


#endif //STRINGCALCULATOR_SCOPED_PTR_H
