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
#include "fe/feAssert.hpp"

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
                            template<typename ...Args>
                            node(node *p, Args &&...args) : m_previous(p), m_value(std::forward<Args>(args)...) {}
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

                    template<typename ...Args>
                    void emplace(Args &&...args);
                    void push(TDataType value);
                    const TDataType &top() const;
                    TDataType &top();
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

                    template<typename ...Args>
                    void emplace(Args &&...args);
                    void push(TDataType value);
                    const TDataType &top() const;
                    TDataType &top();
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

                    template<typename ...Args>
                    void emplace(Args &&...args);
                    void push(TDataType value);
                    const TDataType &top() const;
                    TDataType &top();
                    void pop();
                    bool empty() const;

            };

    }

template<typename TDataType>
inline fe::nonFixedStack<TDataType>::nonFixedStack() : 
    m_data(nullptr)
    {
    }

template<typename TDataType>
inline fe::nonFixedStack<TDataType>::~nonFixedStack()
    {
        while (m_data)
            {
                node *oldNode = m_data;
                m_data = m_data->m_previous;
                delete oldNode;
            }
    }

template<typename TDataType>
template<typename ...Args>
inline void fe::nonFixedStack<TDataType>::emplace(Args &&...args)
    {
        m_data = new node(m_data, std::forward<Args>(args)...);
    }

template<typename TDataType>
inline void fe::nonFixedStack<TDataType>::push(TDataType value)
    {
        m_data = new node(m_data, value);
    }

template<typename TDataType>
const inline TDataType &fe::nonFixedStack<TDataType>::top() const
    {
        FE_ASSERT(m_data, "No Data");
        return m_data->m_value;
    }

template<typename TDataType>
inline TDataType &fe::nonFixedStack<TDataType>::top()
    {
        FE_ASSERT(m_data, "No Data");
        return m_data->m_value;
    }

template<typename TDataType>
inline void fe::nonFixedStack<TDataType>::pop()
    {
        FE_ASSERT(m_data, "Stack Overflow");
        node *newNode = m_data->m_previous;
        delete m_data;
        m_data = newNode;
    }

template<typename TDataType>
inline bool fe::nonFixedStack<TDataType>::empty() const
    {
        return m_data == nullptr;
    }


template<fe::uInt64 size, typename TDataType>
inline fe::semiFixedStack<size, TDataType>::semiFixedStack() :
    m_pointer(0),
    m_nonFixedData(nullptr)
    {
    }

template<fe::uInt64 expectedSize, typename TDataType>
inline fe::semiFixedStack<expectedSize, TDataType>::~semiFixedStack()
    {
        while (m_nonFixedData)
            {
                node *data = m_nonFixedData;
                m_nonFixedData = m_nonFixedData->m_previous;
                delete data;
            }
    }

template<fe::uInt64 expectedSize, typename TDataType>
template<typename ...Args>
inline void fe::semiFixedStack<expectedSize, TDataType>::emplace(Args &&...args)
    {
        if (m_pointer < expectedSize)
            {
                m_fixedData[m_pointer] = TDataType(std::forward<Args>(args)...);
            }
        else
            {
                node *dat = new node;
                dat->m_value = TDataType(std::forward<Args>(args)...);
                dat->m_previous = m_nonFixedData;
                m_nonFixedData = dat;
            }
        m_pointer++;
    }

template<fe::uInt64 expectedSize, typename TDataType>
inline void fe::semiFixedStack<expectedSize, TDataType>::push(TDataType value)
    {
        if (m_pointer < expectedSize)
            {
                m_fixedData[m_pointer] = value;
            }
        else
            {
                node *dat = new node;
                dat->m_value = value;
                dat->m_previous = m_nonFixedData;
                m_nonFixedData = dat;
            }
        m_pointer++;
    }

template<fe::uInt64 expectedSize, typename TDataType>
const inline TDataType &fe::semiFixedStack<expectedSize, TDataType>::top() const
    {
        FE_ASSERT(m_pointer != 0, "No Data");
        if (m_pointer <= expectedSize)
            {
                return m_fixedData[m_pointer - 1];
            }

        return m_nonFixedData->m_value;
    }

template<fe::uInt64 expectedSize, typename TDataType>
inline TDataType &fe::semiFixedStack<expectedSize, TDataType>::top()
    {
        FE_ASSERT(m_pointer != 0, "No Data");
        if (m_pointer <= expectedSize)
            {
                return m_fixedData[m_pointer - 1];
            }

        return m_nonFixedData->m_value;
    }

template<fe::uInt64 expectedSize, typename TDataType>
inline void fe::semiFixedStack<expectedSize, TDataType>::pop()
    {
        FE_ASSERT(m_pointer != 0, "Stack Overflow");
        if (m_pointer > expectedSize)
            {
                node *newNode = m_nonFixedData->m_previous;
                delete m_nonFixedData;
                m_nonFixedData = newNode;
            }

        m_pointer--;
    }

template<fe::uInt64 expectedSize, typename TDataType>
inline bool fe::semiFixedStack<expectedSize, TDataType>::empty() const
    {
        return m_pointer == 0;
    }



template<fe::uInt64 size, typename TDataType>
inline fe::fixedStack<size, TDataType>::fixedStack() :
    m_pointer(0)
    {
    }

template<fe::uInt64 size, typename TDataType>
inline fe::fixedStack<size, TDataType>::~fixedStack()
    {
    }

template<fe::uInt64 size, typename TDataType>
template<typename ...Args>
inline void fe::fixedStack<size, TDataType>::emplace(Args &&...args)
    {
        FE_ASSERT(m_pointer < size, "Stack Overflow");
        m_data[m_pointer++] = TDataType(std::forward<Args>(args)...);
    }

template<fe::uInt64 size, typename TDataType>
inline void fe::fixedStack<size, TDataType>::push(TDataType value)
    {
        FE_ASSERT(m_pointer < size, "Stack Overflow");
        m_data[m_pointer++] = value;
    }

template<fe::uInt64 size, typename TDataType>
const inline TDataType &fe::fixedStack<size, TDataType>::top() const
    {
        FE_ASSERT(m_pointer != 0, "Pointer Overflow");
        return m_data[m_pointer - 1];
    }

template<fe::uInt64 size, typename TDataType>
inline TDataType &fe::fixedStack<size, TDataType>::top()
    {
        FE_ASSERT(m_pointer != 0, "Pointer Overflow");
        return m_data[m_pointer - 1];
    }

template<fe::uInt64 size, typename TDataType>
inline void fe::fixedStack<size, TDataType>::pop()
    {
        FE_ASSERT(m_pointer != 0, "Pointer Overflow");
        m_pointer--;
    }

template<fe::uInt64 size, typename TDataType>
inline bool fe::fixedStack<size, TDataType>::empty() const
    {
        return m_pointer == 0;
    }

