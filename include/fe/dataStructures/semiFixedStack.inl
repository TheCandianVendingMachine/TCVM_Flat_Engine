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

