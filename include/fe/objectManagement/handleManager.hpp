// handleManager.hpp
// stores handles to objects being pushed. Inherit off of to enable handles
#pragma once
#include "../debug/logger.hpp"
#include "../typeDefines.hpp"
#include <iterator>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <tuple>

namespace fe
    {
        /*
            The handles work by storing the object in a normal vector, and another vector full off indicies to the objects in the original vector
            When we create an object, we return the index to the handleObject which in turn has their handle pointing to the index of the wanted object
            This works because we can always assume the indicies of the handleObjects wont change, while we can remove and add objects dynamically
        */
        template<typename T, unsigned int TObjectCount>
        class handleManager
            {
                private:
                    struct handleObject
                        {
                            Handle handle; // Index to the object
                            bool active;

                            handleObject() : handle(0), active(false) {}
                            handleObject(Handle handle) : handle(handle), active(true) {}
                        };

                    // A linked list of handleObjects that hold the current handles that are NOT active. Used to quickly insert items
                    struct handleObjectList
                        {
                            handleObject *m_handle = nullptr;
                            handleObjectList *m_tail = nullptr;
                        } m_baseHandleList;

                    handleObject m_handles[TObjectCount];

                    unsigned int m_objectCount = TObjectCount;
                    unsigned int m_maxIndex;

                protected:
                    T m_objects[TObjectCount];
                    fe::Handle getHandle(T object);

                    virtual void onAdd(T *object, fe::Handle objectHandle) {}
                    virtual void onRemove(T *object, fe::Handle objectHandle) {}

                public:
                    handleManager();

                    inline unsigned int objectCount() { return m_maxIndex; }

                    Handle addObject(T object);
                    Handle addObject(T object, Handle handle);
                    void removeObject(Handle handle);
                    void removeObject(Handle handle, std::function<void(T*)> onRemoveFunc);
                    bool handleActive(Handle handle);
                    void clearAllObjects();
                    void clearAllObjects(std::function<void(T*)> onRemoveFunc);

                    typename std::remove_pointer<T>::type *getObject(Handle handle) const;
                    const T *getObjects() const;

            };

        /*
            The handles work by storing the object in a normal vector, and another vector full off indicies to the objects in the original vector
            When we create an object, we return the index to the handleObject which in turn has their handle pointing to the index of the wanted object
            This works because we can always assume the indicies of the handleObjects wont change, while we can remove and add objects dynamically
        */
        template<typename T>
        class handleManager<T, 0>
            {
                private:
                    struct handleObject
                        {
                            Handle handle; // Index to the object
                            bool active;

                            handleObject() : handle(0), active(false) {}
                            handleObject(Handle handle) : handle(handle), active(true) {}
                        };

                    std::vector<handleObject> m_handles;

                protected:
                    std::vector<T> m_objects;
                    typename std::vector<T>::iterator removeHandle(Handle handle);

                    fe::Handle getHandle(T object);
                    
                    virtual void onAdd(T *object, fe::Handle objectHandle) {}
                    virtual void onRemove(T *object, fe::Handle objectHandle) {}

                public:
                    Handle addObject(T object);
                    Handle addObject(T object, Handle handle);
                    void removeObject(Handle handle);
                    void removeObject(Handle handle, std::function<void(T*)> onRemoveFunc);
                    bool handleActive(Handle handle);
                    void clearAllObjects();
                    void clearAllObjects(std::function<void(T*)> onRemoveFunc);

                    typename std::remove_pointer<T>::type *getObject(Handle handle) const;
                    const std::vector<T> &getObjects() const;

            };

        #include "handleManagerVector.inl"
        #include "handleManagerRaw.inl"
    }