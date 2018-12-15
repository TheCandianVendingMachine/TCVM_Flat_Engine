// nonFixedStack.inl
// A stack that is not contigious in memory at all
#include "stack.hpp"
#include "fe/feAssert.hpp"

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
void fe::nonFixedStack<TDataType>::push(TDataType value)
    {
        m_data = new node(m_data, value);
    }

template<typename TDataType>
inline TDataType fe::nonFixedStack<TDataType>::top() const
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
