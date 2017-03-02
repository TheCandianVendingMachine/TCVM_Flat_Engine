#include "resourceManager.hpp"
#include "../memory/memoryManager.hpp"

void fe::resourceManager::startUp(size_t amountOfResources)
    {
        m_resourceAllocater.startUp(amountOfResources, static_cast<char*>(fe::memoryManager::get().alloc(amountOfResources * sizeof(resourceObject))));
    }

void fe::resourceManager::add(const std::string &id, const std::string &filepath, fe::resourceObjectType::resourceObjectType type)
    {
        resourceObject *allocated = get(id);
        if (allocated) return;

        switch (type)
            {
                case fe::resourceObjectType::RESOURCE_NULL:
                    {
                        resourceObject *obj = m_resourceAllocater.alloc();
                        obj->load(filepath.c_str());
                        allocated = obj;
                    }
                    break;
                case fe::resourceObjectType::RESOURCE_TEXTURE:
                    {
                        imageObject *obj = m_resourceAllocater.alloc<imageObject>();
                        obj->load(filepath.c_str());
                        allocated = obj;
                    }
                    break;
                default:
                    break;
            }

        if (allocated)
            {
                m_resources[id] = allocated;
            }
    }

void fe::resourceManager::remove(const std::string &id)
    {
        m_resourceAllocater.free(m_resources[id]);
        m_resources.erase(id);
    }

fe::resourceObject *fe::resourceManager::get(const std::string &id)
    {
        return m_resources[id];
    }

void fe::resourceManager::clear()
    {
        m_resourceAllocater.clear();
        m_resources.clear();
    }
