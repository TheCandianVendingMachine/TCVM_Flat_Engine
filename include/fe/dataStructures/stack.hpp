// stack.hpp
/* A non-contiguous memory allocated stack
    Expected Properties of All Stacks:
        Arbitrary data
        LIFO
        Short lifespan
        Fast
        Non-Copyable
        Non-Moveable
        Temporary
        No exceptions
*/
#pragma once
#include "fe/typeDefines.hpp"
#include "singlyLinkedList.hpp"

namespace fe
    {
        template<typename TDataType>
        class nonFixedStack
            {
                private:
                    struct node
                        {
                            node *m_previous = nullptr;
                            TDataType m_value;
                            node(node *p, TDataType v) : m_previous(p), m_value(v) {}
                            ~node() {}
                        };
                    // The top of the non-fixed stack
                    node *m_data;

                public:
                    nonFixedStack();
                    ~nonFixedStack();
                    nonFixedStack(const nonFixedStack<TDataType>&) = delete;
                    nonFixedStack(nonFixedStack<TDataType>&&) = delete;
                    nonFixedStack<TDataType> &operator=(const nonFixedStack<TDataType>&) = delete;
                    nonFixedStack<TDataType> &operator=(nonFixedStack<TDataType>&&) = delete;

                    void push(TDataType value);
                    TDataType top() const;
                    void pop();
                    bool empty() const;
            };

        // A stack that is contigious to the defined amount. Contigious memory until size reached
        template<fe::uInt64 expectedSize, typename TDataType>
        class semiFixedStack
            {
                private:
                    struct node 
                        {
                            TDataType m_value;
                            node *m_previous = nullptr;
                        };

                    TDataType m_fixedData[expectedSize];
                    // The top of the non-fixed stack
                    node *m_nonFixedData;
                    unsigned int m_pointer;

                public:
                    semiFixedStack();
                    ~semiFixedStack();
                    semiFixedStack(const semiFixedStack<expectedSize, TDataType>&) = delete;
                    semiFixedStack(semiFixedStack<expectedSize, TDataType>&&) = delete;
                    semiFixedStack<expectedSize, TDataType> &operator=(const semiFixedStack<expectedSize, TDataType>&) = delete;
                    semiFixedStack<expectedSize, TDataType> &operator=(semiFixedStack<expectedSize, TDataType>&&) = delete;

                    void push(TDataType value);
                    TDataType top() const;
                    void pop();
                    bool empty() const;
            };

        // A stack that holds exactly the amount defined. Contigious memory
        template<fe::uInt64 size, typename TDataType>
        class fixedStack
            {
                private:
                    TDataType m_data[size];
                    unsigned int m_pointer;

                public:
                    fixedStack();
                    ~fixedStack();
                    fixedStack(const fixedStack<size, TDataType>&) = delete;
                    fixedStack(fixedStack<size, TDataType>&&) = delete;
                    fixedStack<size, TDataType> &operator=(const fixedStack<size, TDataType>&) = delete;
                    fixedStack<size, TDataType> &operator=(fixedStack<size, TDataType>&&) = delete;

                    void push(TDataType value);
                    TDataType top() const;
                    void pop();
                    bool empty() const;

            };

        #include "fixedStack.inl"
        #include "semiFixedStack.inl"
        #include "nonFixedStack.inl"
    }
