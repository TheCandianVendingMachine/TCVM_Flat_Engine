// semiFixedStack.inl
// A stack that is contigious to an exact size, and then is non-contigious afterward
#include "stack.hpp"
#include "fe/feAssert.hpp"

template<fe::uInt64 size, typename TDataType>
inline fe::semiFixedStack<size, TDataType>::semiFixedStack() :
    m_pointer(0),
    m_nonFixedData(nullptr)
    {
    }

template<fe::uInt64 size, typename TDataType>
inline fe::semiFixedStack<size, TDataType>::~semiFixedStack()
    {
        while (m_nonFixedData)
            {
                node *data = m_nonFixedData;
                m_nonFixedData = m_nonFixedData->m_previous;
                delete data;
            }
    }

template<fe::uInt64 size, typename TDataType>
inline void fe::semiFixedStack<size, TDataType>::push(TDataType value)
    {
        if (m_pointer < size)
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

template<fe::uInt64 size, typename TDataType>
inline TDataType fe::semiFixedStack<size, TDataType>::top() const
    {
        FE_ASSERT(m_pointer != 0, "No Data");
        if (m_pointer <= size)
            {
                return m_fixedData[m_pointer - 1];
            }

        return m_nonFixedData->m_value;
    }

template<fe::uInt64 size, typename TDataType>
inline void fe::semiFixedStack<size, TDataType>::pop()
    {
        FE_ASSERT(m_pointer != 0, "Stack Overflow");
        if (m_pointer > size)
            {
                node *newNode = m_nonFixedData->m_previous;
                delete m_nonFixedData;
                m_nonFixedData = newNode;
            }

        m_pointer--;
    }

template<fe::uInt64 size, typename TDataType>
inline bool fe::semiFixedStack<size, TDataType>::empty() const
    {
        return m_pointer == 0;
    }

