//
// Created by Надежда on 22.12.16.
//

#ifndef STRINGCALCULATOR_SHARED_PTR_H
#define STRINGCALCULATOR_SHARED_PTR_H

#include <memory>
#include <utility>

namespace sharik {

template<typename T>
class shared_ptr {
    typedef T element_type;

public:
    // default constructor
    shared_ptr() : ptr_(nullptr), ref_count_(nullptr) {}
    // constructor from object
    explicit shared_ptr(T *ptr);
    // copy constructor
    shared_ptr(const shared_ptr<T> &copy);
    // move constructor
    shared_ptr(shared_ptr<T> &&origin);
    // assignment operator
    shared_ptr &operator=(const shared_ptr &copy);
    // move assignment operator
    shared_ptr &operator=(shared_ptr &&origin);
    // returns stored object reference
    T &operator*() const;
    // returns stored object pointer
    T *operator->() const;
    // comparison operator
    bool operator!=(const shared_ptr<T> &rhs) { return !((ptr_ == rhs.ptr_) & (ref_count_ == rhs.ref_count_)); }

    // destructor
    virtual ~shared_ptr();

    // return raw pointer to the object
    element_type *get();
    // return the number of instances referring to the object
    int use_count();
    // set references to nullptr
    void reset();

private:
    T *ptr_;
    int *ref_count_;
};

template<typename T>
shared_ptr<T>::shared_ptr(T *ptr) : ptr_(ptr), ref_count_(nullptr) {
    ref_count_ = new int(1);
}

// copy constructor
template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<element_type> &copy)
        : ptr_(copy.ptr_), ref_count_(copy.ref_count_) {
    if (ref_count_ != nullptr) {
        ++(*ref_count_);
    }
}

// move constructor
template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr<element_type> &&origin)
        : ptr_(std::move(origin.ptr_)), ref_count_(std::move(origin.ref_count_)) {
    // set to default state
    origin.reset();
}

// assignment operator
template<typename T>
shared_ptr<T> &shared_ptr<T>::operator=(const shared_ptr &copy) {
    if (this != &copy) {  // comparison operators have to be implemented
        // if the current pointer is the ONLY one referring to the object
        // it has to be deleted
        if (ref_count_ != nullptr) {
            if (*ref_count_ == 1) {
                std::cout << "destructor called on " << ptr_ << std::endl;
                delete ptr_;
                delete ref_count_;
                reset();
            }
        }
        ptr_ = copy.ptr_;
        ref_count_ = copy.ref_count_;

        if (ref_count_ != nullptr) {
            ++(*ref_count_);
        }
    }
    return *this;
}

// move assignment operator
template<typename T>
shared_ptr<T> &shared_ptr<T>::operator=(shared_ptr &&origin) {
    if (this != &origin) {  // comparison operators have to be implemented
        // if the current pointer is the ONLY one referring to the object
        // it has to be deleted
        if (ref_count_ != nullptr) {
            if (*ref_count_ == 1) {
                std::cout << "destructor called on " << ptr_ << std::endl;
                delete ptr_;
                delete ref_count_;
                reset();
            }
        }
        ptr_ = origin.ptr_;
        ref_count_ = origin.ref_count_;
        // set to default state
        origin.reset();
    }
    return *this;
}

// returns stored object reference
template<typename T>
T &shared_ptr<T>::operator*() const {
    return *ptr_;
}

// returns stored object pointer
template<typename T>
T *shared_ptr<T>::operator->() const {
    return ptr_;
}

// destructor
template<typename T>
shared_ptr<T>::~shared_ptr() {
    if (ref_count_ != nullptr) {
        if (--(*ref_count_) == 0) {
            std::cout << "destructor called on " << ptr_ << std::endl;
            delete ptr_;
            delete ref_count_;
        }
    }
    reset();
}

// return raw pointer to the object
template<typename T>
T *shared_ptr<T>::get() {
    return ptr_;
}

// return the number of instances referring to the object
template<typename T>
int shared_ptr<T>::use_count() {
    if (ref_count_ != nullptr) {
        return *ref_count_;
    }
    return 0;
}

// set references to nullptr
template<typename T>
void shared_ptr<T>::reset() {
    ptr_ = nullptr;
    ref_count_ = nullptr;
}

#endif //STRINGCALCULATOR_SHARED_PTR_H
