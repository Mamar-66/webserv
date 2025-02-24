#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>


std::vector<std::string> split(const std::string& str, char delimiter);

template <typename T1,typename T2, typename T3>
class MyTriple {
    public:
        T1 first;
        T2 second;
        T3 third;

        MyTriple() : first(T1()), second(T2()), third(T3()) {}

        MyTriple(T1 f, T2 s, T3 t) : first(f), second(s), third(t) {}

        MyTriple(const MyTriple& cpy) : first(cpy.first), second(cpy.second), third(cpy.third) {}

        MyTriple& operator=(MyTriple& cpy) {
            this->first = cpy.first;
            this->second = cpy.second;
            this->third = cpy.third;
            return *this;
        }

        MyTriple& operator()(T1 f, T2 s, T3 t) {
            this->first = f;
            this->second = s;
            this->third = t;
            return *this;
        }

        void Print() const {
            std::cout << this->first << " : " << this->second << " : " << this->third << std::endl;
        }
};

template <typename T1>
class MyStack {
    private:
        std::stack<T1> stack;
    public:

        MyStack() {}
        MyStack(const MyStack& cpy) : stack(cpy.stack) {};
        MyStack& operator=(const MyStack& cpy) {
            if (this != &cpy)
                this->stack = cpy.stack;
            return *this;
        }
        ~MyStack() {}

        bool empty() {
            return stack.empty();
        }
        T1 top() {
            return stack.top();
        }
        void pop() {
            stack.pop();
        }
        void push(T1& elem) {
            stack.push(elem);
        }

        T1 popTop() {
            T1 elem = stack.top();
            stack.pop();
            return elem;
        }
        void reverse() {
            std::stack<T1> temp;
            T1 elem;
            while (!(this->stack.empty())) {
                elem = this->stack.top();
                this->stack.pop();
                temp.push(elem);
            }
            this->stack = temp;
        }
        bool operator==(const MyStack<T1>& other) const {
            MyStack<T1> cpy1 = *this;
            MyStack<T1> cpy2 = other;

            while ((!(cpy1.empty()) && !(cpy2.empty()))) {
                if (cpy1.popTop() != cpy2.popTop())
                    return false;
            }
            if (cpy1.empty() && cpy2.empty())
                return true;
            return false;
        }
        bool operator<(const MyStack<T1>& other) const {
            return this->stack < other.stack; // Utilise la comparaison lexicographique des vecteurs
        }
};

template <typename T1>
class MyVector {
    private:
        std::vector<T1> vec;

    public:
        MyVector() {}
        MyVector(const MyVector& cpy) : vec(cpy.vec) {}
        MyVector& operator=(const MyVector& cpy) {
            if (this != &cpy)
                this->vec = cpy.vec;
            return *this;
        }
        ~MyVector() {}

        MyVector(std::string& str, char sep) {
            this->vec = split(str, sep);
        }

        typename std::vector<T1>::iterator begin() {
            return vec.begin();
        }
        typename std::vector<T1>::iterator end() {
            return vec.end();
        }
        int size() const {
            return static_cast<int>(vec.size());
        }
        void push_back(const T1& elem) {
            this->vec.push_back(elem);
        }

        T1 operator[](int i) const {
            return this->vec[i];
        }
        bool operator==(const MyVector<T1>& otr) {
            if (this->size() != otr.size()) 
                return false;
            for (int i = 0; i < otr.size(); i++) {
                if (otr[i] != (*this)[i])
                    return false;
            }
            return true;
        }
        T1 operator()(const MyVector<T1>& otr) {
            for (int i = 0; i < otr.size(); ++i) {
                if (*this == otr[i])
                    return otr[i];
            }
            return "NULL";
        }
        bool operator==(const T1& elem) {
            for (int i = 0; i < this->size(); i++) {
                if (elem == (*this)[i])
                    return true;
            }
            return false;
        }
        bool operator<(const MyVector<T1>& otr) {
            return this->vec < otr.vec;
        }

        int findElem(T1& elem) {
            for (int i = 0; i < this->size(); i++) {
                if (elem == (*this)[i])
                    return i;
            }
            return -1;
        }
        void erase(T1& elem) {
            int pos = this->findElem(elem);
            if (pos >= 0)
                this->vec.erase(this->begin() + pos);
        }
};
