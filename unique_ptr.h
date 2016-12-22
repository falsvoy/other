//
// Created by Надежда on 22.12.16.
//

#ifndef STRINGCALCULATOR_UNIQUE_PTR_H
#define STRINGCALCULATOR_UNIQUE_PTR_H

#include <iostream>
#include <memory>


namespace sharik {
	template<typename T>
	class unique_ptr {
	private:
		T *ptr;
	public:
		unique_ptr();

		unique_ptr(T *obj);

		unique_ptr(unique_ptr const &other) = delete;

		unique_ptr(unique_ptr &&other);

		~unique_ptr();

		auto operator=(unique_ptr const &other) -> unique_ptr & = delete;

		auto operator=(unique_ptr &&other) -> unique_ptr &;

		auto get() -> T &;
	};

	template<typename T, class... Args>
	auto make_unique(Args &&... args) -> unique_ptr<T> {
		return unique_ptr<T>(new T(std::forward<T>(args)...));
	}

	template<typename T>
	unique_ptr<T>::unique_ptr() : ptr(nullptr) { }

	template<typename T>
	unique_ptr<T>::unique_ptr(T *obj) : ptr(obj) { }

	template<typename T>
	unique_ptr<T>::unique_ptr(unique_ptr &&other) {
		ptr = other.ptr;
		other.ptr = nullptr;
	}

	template<typename T>
	unique_ptr<T>::~unique_ptr() {
		delete ptr;
	}

	template<typename T>
	auto unique_ptr<T>::operator=(unique_ptr &&other) -> unique_ptr & {
		ptr = other.ptr;
		other.ptr = nullptr;
		return *this;
	}

	template<typename T>
	auto unique_ptr<T>::get() -> T & {
		return *ptr;
	}
}
#endif //STRINGCALCULATOR_UNIQUE_PTR_H
