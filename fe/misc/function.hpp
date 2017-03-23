// function.hpp
// a storage class for functions of any type
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include <type_traits>

namespace fe 
    {
        template<bool, typename Obj, typename Func, typename ...Args>
        struct fPtr
            {
                Obj *instance = nullptr;
                Func(*funcPtr)(Args...);

                Func operator()(Args ...args)
                    {
                        return funcPtr(args...);
                    }
            };

        template<typename Obj, typename Func, typename ...Args>
        struct fPtr<true, Obj, Func, Args...>
            {
                Obj *instance = nullptr;
                Func(Obj::*funcPtr)(Args...);

                fPtr() {}

                fPtr(Obj *instance, Func(Obj::*funcPtr)(Args...)) : instance(instance), funcPtr(funcPtr)
                    {}

                fPtr(const fPtr &other)
                    {
                        if (&other != this)
                            {
                                instance = other.instance;
                                funcPtr = other.funcPtr;
                            }
                    }

                Func operator()(Args ...args)
                    {
                        return (instance->*funcPtr)(args...);
                    }
            };

        template<typename Func, typename Obj, typename ...Args>
        struct function
            {
                fPtr<std::is_class<Obj>::value, Obj, Func, Args...> m_fPtr;

                function()
                    {
                        m_fPtr.funcPtr = nullptr;
                    }

                function(Func(*function)(Args...))
                    {
                        m_fPtr.funcPtr = function;
                    }

                function(fPtr<std::is_class<Obj>::value, Obj, Func, Args...> function)
                    {
                        m_fPtr = function;
                    }

                Func operator()(Args &&...args)
                    {
                        return m_fPtr(args...);
                    }

                operator bool()
                    {
                        return m_fPtr.funcPtr != nullptr;
                    }
            };
    }