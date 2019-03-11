#pragma once

const int POISON = -230;
const int STACK_REALLOC_SIZE = 2;
const int STACK_INIT_SIZE = 4;
const int MODE1 = 1;
const int MODE2 = 2;

//_________________
using std::cout;
using std::cin;
using std::endl;
//_________________

template <class T>
T* my_realloc(T*,int,int);

template <class T>
class my_stack {
    T *array_;
    int size_;
    int capacity_;

public:
//___CTOR___
    my_stack() :
        array_(nullptr),
        size_(0),
        capacity_(0)
    {}

//___DTOR___
    ~my_stack()
    {
        delete[] array_;

        size_ = POISON;
        capacity_ = POISON;
        array_ = nullptr;
    }

//___FUNCS___
    int size()
    {
        return size_;
    }
    
    int capacity()
    {
        return capacity_;
    } 

    void push(T elem)
    {
        try{
            OK(MODE2);
        }

        catch (const std::exception& exc)
        {
            cout << "FUNC stack.push():" << exc.what();
        }

        if (size_ == capacity_) {
            if (capacity_ == 0) {
                try {
                    capacity_ = STACK_INIT_SIZE;
                    array_ = new T[capacity_];
                }

                catch (const std::bad_alloc& ex) {
                    cout << "can't allocate mem!" << endl;
                }
                size_ = 1;

                array_[size_ - 1] = elem;
                return;
            }

            if (capacity_ > 0) {
                array_ = my_realloc(array_, size_, capacity_ * STACK_REALLOC_SIZE);

                if (!array_)
                    return;

                capacity_ = capacity_ * STACK_REALLOC_SIZE;
                size_++;
                array_[size_ - 1] = elem;
                return;
            }
        }

        if (capacity_ > size_) {
            size_++;
            array_[size_ - 1] = elem;
        }
    }

    void pop()
    {
        try{
            OK(MODE1);
        }

        catch (const std::exception& exc)
        {
            cout << "FUNC stack.pop():" << exc.what();
        }

        if (size_ > 0) {
            size_--;
        }
    }

    T toppop()
    {
        try{
            OK(MODE1);
        }

        catch (const std::exception& exc)
        {
            cout << "FUNC stack.toppop():" << exc.what();
        }

        if (size_ > 0) {
            int size_cpy = size_;
            size_--;
            return array_[size_cpy - 1];
        }
    }

    void mul()
    {
        try{
            OK(MODE1);
        }

        catch (const std::exception& exc)
        {
            cout << "FUNC stack.mul():" << exc.what();
        }

        if (size_ == 1) {   //как должно работать умножение с одним элементом в стеке?
            return;
        }

        if (size_ > 1) {
            T a = toppop();
            T b = toppop();
            push(a*b);
            return;
        }
    }

    void div()
    {
        try{
            OK(MODE1);
        }

        catch (const std::exception& exc)
        {
            cout << "FUNC stack.div():" << exc.what();
        }

        if (size_ == 1) {     //спросить про то как должно работать деление когда один элемент в стеке
            return;
        }

        if(size_ > 1){
            T a = toppop();
            T b = toppop();

            push(b/a);
            return;
        }
    }

    void add()
    {
        try{
            OK(MODE1);
        }

        catch (const std::exception& exc)
        {
            cout << "FUNC stack.add():" << exc.what();
        }

        if (size_ == 1) {   //как должно работать сложенние с одним элементом в стеке?
            return;
        }

        if (size_ > 1) {
            T a = toppop();
            T b = toppop();
            push(a+b);
            return;
        }
    }

    void sub()
    {
        try{
            OK(MODE1);
        }

        catch (const std::exception& exc)
        {
            cout << "FUNC stack.sub():" << exc.what();
        }

        if (size_ == 1) {   //как должно работать сложенние с одним элементом в стеке?
            return;
        }

        if (size_ > 1) {
            T a = toppop();
            T b = toppop();
            push(b-a);
            return;
        }

        return;
    }

    void OK(int mode)
    {
        switch(mode){

            case(1):{
                if (capacity_ < 0) {
                    throw std::logic_error("Capacity of stack is less than zero!");
                }

                if (size_ < 0) {
                    throw std::logic_error("Size of stack is less than zero!");
                }

                if (size_ == 0) {
                    throw std::logic_error("Stack is empty!");
                }

                break;
            }

            case(2):{
                if (capacity_ < 0) {
                    throw std::logic_error("Capacity of stack is less than zero!");
                }

                if (size_ < 0) {
                    throw std::logic_error("Size of stack is less than zero!");
                }

                break;
            }

            default:{
                return;
            }
        }
    }


};

template <class T>
T* my_realloc(T* buf,int old_size ,int new_size)
{
    T *new_array = nullptr;

    try {
        new_array = new T[new_size];
    }

    catch (std::bad_alloc ex) {
        cout << "can't allocate mem!" << endl;
        return nullptr;
    }

    for(int i = 0; i < old_size; i++)
    {
        new_array[i] = buf[i];
    }

    delete [] buf;
    buf = nullptr;

    return new_array;
}
