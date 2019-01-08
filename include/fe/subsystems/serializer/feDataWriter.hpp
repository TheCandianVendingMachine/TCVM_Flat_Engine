// feDataWriter.hpp
// fe custom serialized data format using streaming instead of copying into RAM
#pragma once
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/serializer/dataWriter.hpp"
#include "fe/dataStructures/stack.hpp"
#include "fe/typeDefines.hpp"
#include "fe/feAssert.hpp"

#include <vector>
#include <memory>
#include <string>
#include <type_traits>

namespace fe
    {
        namespace imp
            {
                namespace dataWriter
                    {
                        enum class itemType : fe::uInt8
                            {
                                DATA,
                                OBJECT,
                                LIST
                            };

                        // Base class for all items possible
                        struct item
                            {
                                std::string m_name;
                                itemType m_type;
                                bool m_cleanup;
                                item() = delete;
                                item(itemType type) : m_type(type), m_cleanup(false) {}
                                item(const char *name, itemType type) : m_name(name), m_type(type), m_cleanup(false) {}

                                virtual void output(std::ostream &out, const std::string &indent = "") = 0;
                            };
                        
                        // Basic data. Char, Bool, Int, Float, etc.
                        struct data : item
                            {
                                std::string m_data;
                                data() : item(itemType::DATA), m_data(nullptr) {}
                                data(const char *name, const char *data) : item(name, itemType::DATA), m_data(data) {}

                                void output(std::ostream &out, const std::string &indent = "") override final;
                            };

                        struct object : item
                            {
                                std::string m_title;
                                std::vector<std::unique_ptr<item>> m_memberItems;

                                object() : item(itemType::OBJECT) {}
                                object(const char *name, const char *title) : item(name, itemType::OBJECT), m_title(title) {}

                                // Returns the first sub-item from the object
                                fe::imp::dataWriter::item *getSubItem(const char *name) const;
                                // Returns the first sub-item with the type from the object
                                fe::imp::dataWriter::item *getSubItemType(const char *name, fe::imp::dataWriter::itemType type) const;

                                // Returns the first sub-object from the object with the appropiate title
                                fe::imp::dataWriter::object *getSubObject(const char *title) const;

                                // Returns the first sub-object of the sub-type from the entire object heiarchy - searches all member items as well
                                fe::imp::dataWriter::item *getSubItemTypeFromHeiarchy(const char *name, fe::imp::dataWriter::itemType type);

                                void output(std::ostream &out, const std::string &indent = "") override final;
                            };

                        // List of items of any type. Items are constructed and stored in the object
                        struct list : item
                            {
                                fe::nonFixedStack<item*> m_storedItems;
                                list(): item(itemType::LIST) {}
                                list(const char *name) : item(name, itemType::LIST) {}
                                ~list();

                                bool hasItems() const;
                                // Returns the top sub-item from the stack.
                                fe::imp::dataWriter::item *getSubItem();
                                // Returns the first object from the stack with the appropiate title
                                fe::imp::dataWriter::object *getSubObject(const char *title);

                                void output(std::ostream &out, const std::string &indent = "") override final;
                            };
                    }
            }

        class feDataWriter : public fe::dataWriter
            {
                private:
                    using ItemEnum = fe::imp::dataWriter::itemType;
                    using ItemType = fe::imp::dataWriter::item;
                    using DataType = fe::imp::dataWriter::data;
                    using ObjectType = fe::imp::dataWriter::object;
                    using ListType = fe::imp::dataWriter::list;

                    // obj serialized_items
                    imp::dataWriter::object m_baseItem;
                    bool m_writingObject;

                    std::string m_currentTitle;
                    fe::nonFixedStack<fe::imp::dataWriter::item*> m_itemStack;

                    template<typename TItemType, typename ...Args>
                    void startItem(Args &&...args);

                    // Extracts all whitespace until next character is alphanumeric
                    FLAT_ENGINE_API void ignoreWhiteSpace(std::istream &in);

            public:
                    FLAT_ENGINE_API feDataWriter();
                    FLAT_ENGINE_API feDataWriter(const char *initialName);

                    FLAT_ENGINE_API void setTitle(const std::string &title) override final;

                    FLAT_ENGINE_API void startObject(const std::string &id) override final;
                    FLAT_ENGINE_API void startObjectList(const std::string &id) override final;
                    FLAT_ENGINE_API void startObjectRead(const std::string &id) override final;
                    FLAT_ENGINE_API void startObjectListRead(const std::string &id) override final;
                    FLAT_ENGINE_API void endObject() override final;
                    FLAT_ENGINE_API void endObjectList() override final;

                    FLAT_ENGINE_API void write(const std::string &id, const std::string &value) override final;
                    FLAT_ENGINE_API void writeList(const std::string &id, const std::string &value) override final;

                    FLAT_ENGINE_API std::string read(const std::string &id) override final;
                    FLAT_ENGINE_API std::string readList(const std::string &id) override final;

                    FLAT_ENGINE_API void outData(std::ostream &out) override final;
                    FLAT_ENGINE_API void readData(std::istream &in) override final;

                    FLAT_ENGINE_API bool listHasItems(const std::string &list) override final;

                    FLAT_ENGINE_API void clearData() override final;
            };

        template<typename TItemType, typename ...Args>
        inline void feDataWriter::startItem(Args &&...args)
            {
                static_assert(std::is_base_of<ItemType, TItemType>::value, "Cannot add non-subclass item to object");
                FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT || m_itemStack.top()->m_type == ItemEnum::LIST, "Current stack top is not an object - cannot start new object");
                ItemType *newItem = new TItemType(std::forward<Args>(args)...);

                switch (m_itemStack.top()->m_type)
                    {
                        case ItemEnum::LIST:
                            static_cast<ListType*>(m_itemStack.top())->m_storedItems.emplace(newItem);
                            break;
                        case ItemEnum::OBJECT:
                            static_cast<ObjectType*>(m_itemStack.top())->m_memberItems.emplace_back(newItem);
                            break;
                        default:
                            break;
                    }

                m_itemStack.push(newItem);
            }
    }

