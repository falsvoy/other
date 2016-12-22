//
// Created by Надежда on 22.12.16.
//

#ifndef STRINGCALCULATOR_MYUNIQUE_PTR_H
#define STRINGCALCULATOR_MYUNIQUE_PTR_H

#include <iostream>

template<class T>
struct deleter
{
    static void release(T *p)
    {
        if (p) {
            delete p;
            p = 0;
        }
    }
};

template<class T>
struct array_deleter
{
    static void release(T *p)
    {
        if (p) {
            delete [] p;
            p = 0;
        }
    }
};


template <class T, class D = deleter<T> >
class myUnique_ptr {

        private:
            T *_ptr;

            template <class U, class D> myUnique_ptr(myUnique_ptr<U, D> &);
            template <class U, class D> myUnique_ptr &operator=(myUnique_ptr<U, D> &);

        public:
            typedef T element_type;
            typedef D deleter_type;

            myUnique_ptr();

            explicit myUnique_ptr(T *p);

            ~myUnique_ptr();

            myUnique_ptr &operator=(myUnique_ptr u);

            template <class U>
            myUnique_ptr &operator=(myUnique_ptr<U, D> u);

            T operator*() const;

            T *operator->() const;

            T *get() const;

            T *release();

            void reset(T *p = 0);

            void swap(myUnique_ptr &u);
};

    template <class T, class D>
    myUnique_ptr<T, D>::myUnique_ptr() :
            _ptr(0)
    {}

    template <class T, class D>
    myUnique_ptr<T, D>::myUnique_ptr(T *p) :
            _ptr(p)
    {}

    template <class T, class D>
    myUnique_ptr<T, D>::~myUnique_ptr()
    {
        reset();
    }

    template <class T, class D>
    myUnique_ptr<T, D> &myUnique_ptr<T, D>::operator=(myUnique_ptr<T, D> u)
    {
        reset(u.release());
        return *this;
    }

    template <class T, class D>
    template <class U>
    myUnique_ptr<T, D> &myUnique_ptr<T, D>::operator=(myUnique_ptr<U, D> u)
    {
        reset(u.release());
        return *this;
    }

    template <class T, class D>
    T myUnique_ptr<T, D>::operator*() const
    {
        return *_ptr;
    }

    template <class T, class D>
    T *myUnique_ptr<T, D>::operator->() const
    {
        return _ptr;
    }

    template <class T, class D>
    T *myUnique_ptr<T, D>::get() const
    {
        return *_ptr;
    }

    template <class T, class D>
    T *myUnique_ptr<T, D>::release()
    {
        T *tmp = _ptr;
        _ptr = 0;
        return tmp;
    }

    template <class T, class D>
    void myUnique_ptr<T, D>::reset(T *p = 0)
    {
        if (_ptr != p) {
            if (_ptr) {
                myUnique_ptr<T, D>::deleter_type::release(_ptr);
                _ptr = p;
            }
        }
    }

    template <class T, class D>
    void myUnique_ptr<T, D>::swap(myUnique_ptr<T, D> &u)
    {
        std::swap(_ptr, u._ptr);
    }

    template <class T, class D>
    inline void swap(myUnique_ptr<T, D> &x, myUnique_ptr<T, D> &y)
    {
        x.swap(y);
    }

    template <class T1, class D1, class T2, class D2>
    bool operator==(const myUnique_ptr<T1, D1> &x, const myUnique_ptr<T2, D2> &y)
    {
        return x.get() == y.get();
    }

    template <class T1, class D1, class T2, class D2>
    bool operator!=(const myUnique_ptr<T1, D1> &x, const myUnique_ptr<T2, D2> &y)
    {
        return x.get() != y.get();
    }

    template <class T1, class D1, class T2, class D2>
    bool operator<(const myUnique_ptr<T1, D1> &x, const myUnique_ptr<T2, D2> &y)
    {
        return x.get() < y.get();
    }

    template <class T1, class D1, class T2, class D2>
    bool operator<=(const myUnique_ptr<T1, D1> &x, const myUnique_ptr<T2, D2> &y)
    {
        return x.get() <= y.get();
    }

    template <class T1, class D1, class T2, class D2>
    bool operator>(const myUnique_ptr<T1, D1> &x, const myUnique_ptr<T2, D2> &y)
    {
        return x.get() > y.get();
    }

    template <class T1, class D1, class T2, class D2>
    bool operator>=(const myUnique_ptr<T1, D1> &x, const myUnique_ptr<T2, D2> &y)
    {
        return x.get() >= y.get();
    }

#endif //STRINGCALCULATOR_MYUNIQUE_PTR_H
