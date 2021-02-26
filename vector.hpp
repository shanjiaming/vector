#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>


/*

class Counter {
private:
    size_t *p = nullptr;

    void try_kill() {
        if (p != nullptr) {
            if (*p == 1) {
                delete p;
            } else {
                --(*p);
            }
        }
    }

public:
    Counter() : p{new size_t(1)} {}

    Counter(const Counter &_o) : p{_o.p} { ++(*p); }

    Counter(Counter &&_o) : p{_o.p} { _o.p = nullptr; }

    Counter &operator=(const Counter &_o) {
        try_kill();
        p = _o.p;
        ++(*p);
        return *this;
    }

    Counter &operator=(Counter &&_o) {
        try_kill();
        p = _o.p;
        _o.p = nullptr;
        return *this;
    }

    ~Counter() {
        try_kill();
        p = nullptr;
    }

public:
    bool only() const {
        return (*p == 1);
    }
};


template<typename T>
class smart_ptr {
private:

private:
    T *ptr = nullptr;
    Counter cnt;

public:
    smart_ptr(T *p) : ptr(p) {}

    smart_ptr(const T &p) : ptr{new T(p)} {}

    smart_ptr(T &&p) : ptr{new T(p)} {}

    smart_ptr() : ptr(nullptr) {}

    smart_ptr(const smart_ptr &o) = default;

    smart_ptr(smart_ptr &&o) = default;

    smart_ptr &operator=(const smart_ptr &_o) {
        if (cnt.only()) {
            delete ptr;
        }
        cnt = _o.cnt;
        ptr = _o.ptr;
        return *this;
    }

    smart_ptr &operator=(smart_ptr &&_o) noexcept {
        if (cnt.only()) {
            delete ptr;
        }
        cnt = std::move(_o.cnt);
        ptr = _o.ptr;
        _o.ptr = nullptr;
        return *this;
    }

    ~smart_ptr() {
        if (ptr && cnt.only()) {
            delete ptr;
            ptr = nullptr;
        }
    }

    T *operator->() const noexcept {
        return ptr;
    }

    T &operator*() const noexcept {
        return *ptr;
    }

};
*/

/*

template<typename T>
class array_ptr {
private:

private:
    T *ptr = nullptr;
    Counter cnt;

public:
    array_ptr(T *p) : ptr(p) {}

    array_ptr(const T &p) : ptr{new T(p)} {}

    array_ptr(T &&p) : ptr{new T(p)} {}

    array_ptr() : ptr(new T) {}

    array_ptr(const array_ptr &o) = default;

    array_ptr(array_ptr &&o) = default;

    array_ptr &operator=(const array_ptr &_o) {
        if (cnt.only()) {
            delete [] ptr;
        }
        cnt = _o.cnt;
        ptr = _o.ptr;
        return *this;
    }

    array_ptr &operator=(array_ptr &&_o) noexcept {
        if (cnt.only()) {
            delete [] ptr;
        }
        cnt = std::move(_o.cnt);
        ptr = _o.ptr;
        _o.ptr = nullptr;
        return *this;
    }

    ~array_ptr() {
        if (ptr && cnt.only()) {
            delete [] ptr;
            ptr = nullptr;
        }
    }

    T *operator->() const noexcept {
        return ptr;
    }

    T &operator*() const noexcept {
        return *ptr;
    }

    T &operator[](size_t ind) const noexcept {
        return ptr[ind];
    }

};
*/


namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
private:
    T** a;
//T**a;//FIXME MemLeak
    size_t space, num;
    static const size_t InitSize = 5;

    void doubleSpace() {
        T** tmp;
//T**tmp;//FIXME MemLeak
        space *= 2;
        tmp = new T*[space];
        for (int i = 0; i < num; ++i)
            tmp[i] = a[i];
        delete[] a;
        a = tmp;
    }
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator {
        friend vector<T>;
    private:
        const vector<T> *vec;
        int pos;
    public:
        iterator(const vector<T> *v, const int p) : vec(v), pos(p) {}

        /**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const {
			return iterator(vec, pos + n);
		}
		iterator operator-(const int &n) const {
            return iterator(vec, pos - n);
        }
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const {
            if (vec != rhs.vec)
                throw invalid_iterator();
            return pos - rhs.pos;
		}
		iterator& operator+=(const int &n) {
            pos = pos + n;
            return *this;
		}
		iterator& operator-=(const int &n) {
            pos = pos - n;
            return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
		    auto tmp = *this;
		    *this += 1;
		    return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
		    return *this += 1;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
            auto tmp = *this;
            *this -= 1;
            return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
            return *this -= 1;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
            return *(vec->a[pos]);
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
            return rhs.vec == vec && rhs.pos == pos;
		}
		bool operator==(const const_iterator &rhs) const {
            return rhs.vec == vec && rhs.pos == pos;
        }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
		    return !(rhs == *this);
		}
		bool operator!=(const const_iterator &rhs) const {
            return !(rhs == *this);
        }
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator {
        friend vector<T>;
    private:
        const vector<T> *vec;
        int pos;
    public:
        const_iterator(const vector<T> *v, const int p) : vec(v), pos(p) {}

        /**
		 * return a new const_iterator which pointer n-next elements
		 * as well as operator-
		 */
        const_iterator operator+(const int &n) const {
            return const_iterator(vec, pos + n);
        }
        const_iterator operator-(const int &n) const {
            return const_iterator(vec, pos - n);
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            if (vec != rhs.vec)
                throw invalid_iterator();
            return pos - rhs.pos;
        }
        const_iterator& operator+=(const int &n) {
            pos = pos + n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            pos = pos - n;
            return *this;
        }
        /**
         * TODO iter++
         */
        const_iterator operator++(int) {
            auto tmp = *this;
            *this += 1;
            return tmp;
        }
        /**
         * TODO ++iter
         */
        const_iterator& operator++() {
            return *this += 1;
        }
        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            auto tmp = *this;
            *this -= 1;
            return tmp;
        }
        /**
         * TODO --iter
         */
        const_iterator& operator--() {
            return *this -= 1;
        }
        /**
         * TODO *it
         */
        const T& operator*() const{
            return *(vec->a[pos]);
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return rhs.vec == vec && rhs.pos == pos;
        }
        bool operator==(const const_iterator &rhs) const {
            return rhs.vec == vec && rhs.pos == pos;
        }
        /**
         * some other operator for const_iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return !(rhs == *this);
        }
        bool operator!=(const const_iterator &rhs) const {
            return !(rhs == *this);
        }
	};
	/**
	 * TODO Constructs
	 * Atleast two: default constructor, copy constructor
	 */
	vector() : space(InitSize), num(0) {
        a = new T*[InitSize];
    }
	vector(const vector &other) : space(other.space), num(other.num) {
        a = new T*[space];
        for (int i = 0; i < num; ++i) {
            a[i] = new T(*other.a[i]);
        }
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
	    clear();
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
        if (this == &other)
            return *this;
        clear();
        a = new T*[space];
        for (int i = 0; i < num; ++i) {
            a[i] = new T(*other.a[i]);
        }
        return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, space)
	 */
	T & at(const size_t &pos) {
        if (pos < 0 || pos >= num)
            throw index_out_of_bound();
        return *a[pos];
	}
	const T & at(const size_t &pos) const {
        if (pos < 0 || pos >= num)
            throw index_out_of_bound();
        return *a[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, space)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
        if (pos < 0 || pos >= num)
            throw index_out_of_bound();
        return *a[pos];
	}
	const T & operator[](const size_t &pos) const {
        if (pos < 0 || pos >= num)
            throw index_out_of_bound();
        return *a[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if space == 0
	 */
	const T & front() const {
        if (!num)
            throw container_is_empty();
        return *a[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if space == 0
	 */
	const T & back() const {
        if (!num)
            throw container_is_empty();
        return *a[num - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
	    return iterator(this, 0);
	}
	const_iterator cbegin() const {
        return const_iterator(this, 0);
    }
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
        return iterator(this, num);
    }
	const_iterator cend() const {
        return const_iterator(this, num);
    }
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
	    return !num;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
	    return num;
	}
	/**
	 * clears the contents
	 */
	void clear() {
        for (int i = 0; i < num; i++)
            delete a[i];
        delete[] a;
        a = nullptr;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
        return insert(pos.pos, value);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > space (in this situation ind can be space because after inserting the space will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
        if (ind < 0 || ind > num)
            throw index_out_of_bound();
        if (num == space)
            doubleSpace();
        for (size_t i = num; i > ind; --i)
            a[i] = a[i - 1];
        a[ind] = new T(value);
        ++num;
        return iterator(this, ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
        return erase(pos.pos);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= space
	 */
	iterator erase(const size_t &ind) {
        if (ind < 0 || ind >= num)
            throw index_out_of_bound();
        delete a[ind];
        for (size_t i = ind; i < num; ++i)
            a[i] = a[i + 1];
        --num;
        return iterator(this, ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
        if (num == space)
            doubleSpace();
        a[num++] = new T(value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if space() == 0
	 */
	void pop_back() {
        if (!num)
            throw container_is_empty();
        --num;
        delete a[num];
	}
};


}

#endif
