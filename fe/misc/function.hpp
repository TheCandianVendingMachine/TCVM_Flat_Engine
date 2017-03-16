// function.hpp
// a storage class for functions of any type
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"

namespace fe 
    {
        template<typename T>
        struct function {};

        template<typename Func, typename ...Args>
        struct function<Func(Args...)>
            {
                Func(*fPtr)(Args...);

                function()
                    {
                        fPtr = nullptr;
                    }

                function(Func(*function)(Args...))
                    {
                        fPtr = function;
                    }

                Func operator()(Args &&...args)
                    {
                        return fPtr(args...);
                    }

                operator bool()
                    {
                        return fPtr != nullptr;
                    }
            };

        template<typename Obj, typename Func, typename ...Args>
        struct function<Func(Obj*, Args...)>
            {
                Func(Obj::*fPtr)(Args...);
                Obj *instance;

                function()
                    {
                        fPtr = nullptr;
                    }

                function(Obj *inst, Func(Obj::*function)(Args...)) : fPtr(function), instance(inst)
                    {}

                Func operator()(Args &&...args)
                    {
                        return (instance->*fPtr)(args...);
                    }

                operator bool()
                    {
                        return fPtr != nullptr;
                    }
            };
    }