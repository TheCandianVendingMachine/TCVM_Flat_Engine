// fixedStack.inl
// A stack that is an exact size, no more no less
#include "stack.hpp"
#include "fe/feAssert.hpp"

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

