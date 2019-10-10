#pragma once

#include <cassert>
#include <iostream>
#include <cmath>
#include "my_realloc.hpp"

namespace cxx {

    static const int INIT_SIZE = 3;
    static const int VECTOR_POISON = -230;
    static const int REALC_SIZE = 3;
    static const int BOOREALC_SIZE = 1;
//********************************************
//*************template vector****************
//********************************************
#define VECOK if(OK() == -1)\
                exit(-1);

    template<typename T>
    class my_vector {
#ifdef VECTOR_OPEN_SOURCE
    public:
#endif
        T* array_;
        int size_;
        int capacity_;

        int OK() const{
            if(size_ < 0) {
                std::cerr << "vector size less than 0\n";
                return -1;
            }

            if(capacity_ < 0) {
                std::cerr << "vector capacity less than 0\n";
                return -1;
            }

            if(size_ > capacity_) {
                std::cerr << "vector size greater than vector capacity\n";
                return -1;
            }

            if(!array_ && (capacity_ != 0)) {
                std::cerr << "nullptr array\n";
                return -1;
            }

            return 0;
        }

    public:
        explicit my_vector<T>()
                :
                array_(nullptr),
                size_(0),
                capacity_(0) {}

        explicit my_vector<T>(int size)
                :
                array_(nullptr),
                size_(size),
                capacity_(size)
        {
            assert(size >= 0);
            array_ = new T[size];
        }

        my_vector<T>(const my_vector<T> &other)
                :
                array_(nullptr),
                size_(other.size_),
                capacity_(other.capacity_)
        {
            array_ = new T[capacity_];

            for(int i = 0; i < size_; i++)
                array_[i] = other.array_[i];
        }

        ~my_vector<T>() {
            delete[] array_;
            array_ = nullptr;
            size_ = capacity_ = VECTOR_POISON;
        }

        void push_back(T elem) {
            VECOK

            if(capacity_ == size_) {
                array_ = my_realloc(array_, capacity_, capacity_ + REALC_SIZE);

                if(!array_) {
                    std::cerr << "in" << __PRETTY_FUNCTION__ << std::endl;
                    return;
                }

                array_[size_] = elem;
                ++size_;
                capacity_ += REALC_SIZE;
            }

            else {
                array_[size_] = elem;
                ++size_;
            }
        }

        int size() const{
            VECOK
            return size_;
        }

        int capacity() const{
            VECOK
            return capacity_;
        }

        bool empty() {
            VECOK
            return size_ == 0;
        }

        void resize(int new_size) {
            if(new_size < 0) {
                std::cerr << "size less than 0 in" << __PRETTY_FUNCTION__ << std::endl;
                return;
            }

            VECOK

            if(new_size > capacity_) {
                array_ = my_realloc(array_, capacity_, new_size);
                capacity_ = size_ = new_size;
            } else {
                size_ = new_size;
            }
        }

        T& operator[](int index) {
            VECOK
            assert(index >= 0);
            assert(index < size_);
            return array_[index];
        }

        const T& operator[](int index) const{
            VECOK
            assert(index >= 0);
            assert(index < size_);
            return array_[index];
        }

        my_vector<T>& operator=(const my_vector<T>& other) {
            if(capacity_ == other.capacity_) {
                size_ = other.size_;
                for(int i = 0; i < size_; i++) {
                    array_[i] = other.array_[i];
                }

                return *this;
            } else {
                array_ = my_realloc(array_, capacity_ , other.capacity_);

                if(!array_)
                    throw std::bad_alloc();

                for(int i = 0; i < other.capacity_; i++)
                    array_[i] = other.array_[i];

                capacity_ = other.capacity_;
                size_ = other.size_;
                return *this;
            }
        }
    };

    template<typename T>
    bool operator==(const my_vector<T>& left, const my_vector<T>& right) {
        if(left.size() != right.size())
            return false;

        for(int i = 0; i < left.size_; i++)
            if(left.array_[i] != right.array_[i])
                return false;

        return true;
    }

    template<typename T>
    bool operator!=(const my_vector<T>& left, const my_vector<T>& right) {
        return !(left == right);
    }



//********************************************
//***************bool vector******************
//********************************************

    bool operator==(const my_vector<bool>& left, const my_vector<bool>& right);

    template<>
    class my_vector<bool> {
#ifdef VECTOR_OPEN_SOURCE
    public:
#endif
        u_char *array_;

        int chrsize_;
        int chrcapacity_;

        int size_;
        int capacity_;

        int OK() const{
            if(size_ < 0) {
                std::cerr << "vector size less than 0\n";
                return -1;
            }

            if(capacity_ < 0) {
                std::cerr << "vector capacity less than 0\n";
                return -1;
            }

            if(chrsize_ < 0) {
                std::cerr << "vector size less than 0\n";
                return -1;
            }

            if(chrcapacity_ < 0) {
                std::cerr << "vector capacity less than 0\n";
                return -1;
            }

            if(size_ > capacity_) {
                std::cerr << "vector size greater than vector capacity\n";
                return -1;
            }

            if(!array_ && (capacity_ != 0)) {
                std::cerr << "nullptr array\n";
                return -1;
            }

            return 0;
        }

    public:

        class boolvector_proxyclass {
            u_char& element_;
            uint32_t bitplace_;
        public:
            boolvector_proxyclass(u_char& element, uint32_t bitplace)
                    :
                    element_(element),
                    bitplace_(bitplace)
            {}

            operator bool() const{
                u_char mask = element_ << bitplace_;
                mask = mask >> 7U;
                return mask != 0;
            }

            boolvector_proxyclass& operator=(bool expr) {
                u_char mask = static_cast<u_char>(expr) << (7 - bitplace_);

                if(expr)
                    element_ = element_ | mask;
                else
                    element_ = element_ & (~mask);

                return *this;
            }

        };

        explicit my_vector<bool>()
                :
                array_(nullptr),
                chrsize_(0),
                chrcapacity_(0),
                size_(0),
                capacity_(0)
        {}
        
        explicit my_vector<bool>(int size)
                :
                array_(nullptr),
                chrsize_(static_cast<int>(size/8) + 1),
                chrcapacity_(static_cast<int>(size/8) + 1),
                size_(size),
                capacity_(chrcapacity_ * 8)
        {
            assert(size >= 0);
            array_ = new u_char[chrcapacity_];

            for(int i = 0; i < chrcapacity_; i++) {
                array_[i] = 0;
            }
        }

        my_vector<bool>(const my_vector<bool> &other)
                :
                array_(nullptr),
                chrsize_(other.chrsize_),
                chrcapacity_(other.chrcapacity_),
                size_(other.size_),
                capacity_(other.capacity_)
        {
            array_ = new u_char[chrcapacity_];

            for(int i = 0; i < chrsize_; i++)
                array_[i] = other.array_[i];
        }

        ~my_vector<bool>() {
            delete[] array_;
            array_ = nullptr;
            size_ = capacity_ = chrsize_ = chrcapacity_ = VECTOR_POISON;
        }

        void push_back(bool expr) {
            VECOK

            if (capacity_ == size_) {
                array_ = my_realloc<u_char>(array_, chrcapacity_, chrcapacity_ + BOOREALC_SIZE);

                if (!array_) {
                    std::cerr << "in" << __PRETTY_FUNCTION__ << std::endl;
                    return;
                }

                chrcapacity_ = chrcapacity_ + BOOREALC_SIZE;
                ++chrsize_;
                ++size_;
                capacity_ = chrcapacity_ * 8;
            } else {
                ++size_;
                if (static_cast<float>(size_ / 8) > static_cast<float>(chrsize_))
                    ++chrsize_;
            }

            uint32_t bitplace = 0;

            if (size_ > 8)
                bitplace = size_ - 8 * (chrsize_ - 1);
            else
                bitplace = size_;

            u_char mask = static_cast<u_char>(expr) << (8 - bitplace);

            if(expr)
                array_[chrsize_ - 1] = array_[chrsize_ - 1] | mask;
            else
                array_[chrsize_ - 1] = array_[chrsize_ - 1] & (~mask);
        }

        int size() const {
            VECOK
            return size_;
        }

        int capacity()const {
            VECOK
            return capacity_;
        }

        bool empty() {
            VECOK
            return size_ == 0;
        }

        void resize(int new_size) {
            if(new_size < 0) {
                std::cerr << "size less than 0 in" << __PRETTY_FUNCTION__ << std::endl;
                return;
            }

            VECOK

            if(new_size > capacity_) {
                array_ = my_realloc(array_, chrcapacity_, static_cast<int>(new_size / 8) + 1);
                chrcapacity_ = static_cast<int>(new_size / 8) + 1;
                capacity_ = chrcapacity_ * 8;
                chrsize_ = chrcapacity_;
                size_ = new_size;
            } else {
                size_ = new_size;
                chrsize_ = static_cast<int>(new_size / 8) + 1;
            }
        }

        bool operator[](int index) const {
            VECOK

            assert(index >= 0);
            assert(index < size_);

            uint32_t bitplace = static_cast<uint32_t>(index - (chrsize_ - 1) * 8);
            u_char mask = array_[static_cast<int>(index / 8)] << bitplace;
            mask = mask >> 7U;
            return mask != 0;
        }

        boolvector_proxyclass operator[](int index) {
            VECOK

            assert(index >= 0);
            assert(index < size_);

            return {array_[static_cast<int>(index / 8)], static_cast<uint32_t>(index - (chrsize_ - 1) * 8)};
        }

        void test_dump() {
            printf("==============\n");
            printf("Vector status:\n chrsize: %d\n chrcapa: %d\n size: %d\n capa: %d\nelements: ", chrsize_, chrcapacity_, size_,
                   capacity_);

            for(int i = 0; i < chrsize_; i++) {
                printf("0x%X ", array_[i]);
            }
            printf("\n==============\n");
        }

        my_vector<bool>& operator=(const my_vector<bool>& other) {
            if(chrcapacity_ == other.chrcapacity_) {
                chrsize_ = other.chrsize_;
                size_ = other.size_;

                for(int i = 0; i < chrsize_; i++)
                    array_[i] = other.array_[i];

                return *this;
            } else {
                array_ = my_realloc(array_, chrcapacity_ , other.chrcapacity_);

                if(!array_)
                    throw std::bad_alloc();

                for(int i = 0; i < other.size_; i++)
                    array_[i] = other.array_[i];

                chrcapacity_ = other.chrcapacity_;
                capacity_ = other.capacity_;
                chrsize_ = other.chrsize_;
                size_ = other.size_;

                return *this;
            }
        }
        friend bool operator==(const my_vector<bool>& left, const my_vector<bool>& right);
    };
#undef VECOK

    std::ostream& operator<<(std::ostream& os, const my_vector<bool>::boolvector_proxyclass& elem) {
        os << (bool) elem;
        return os;
    }

    bool operator==(const my_vector<bool>& left, const my_vector<bool>& right) {
        if(left.size_ != right.size_)
            return false;

        int i = 0;
        for(i = 0; i < left.chrsize_ - 1; i++)
            if(left.array_[i] != right.array_[i])
                return false;


        u_char right_lastelem = right.array_[i] >> (8 - (right.chrsize_ * 8 - right.size_));
        u_char left_lastelem = left.array_[i] >> (8 - (left.chrsize_ * 8 - left.size_));

        return left_lastelem == right_lastelem;
    }

    bool operator!=(const my_vector<bool>& left, const my_vector<bool>& right) {
        return !(left == right);
    }
    
}
