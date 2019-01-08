#include "fe/subsystems/serializer/feDataWriter.hpp"
#include "fe/objectManagement/str.hpp"
#include "fe/debug/logger.hpp"
#include <algorithm>
#include <ostream>
#include <istream>
#include <array>

void fe::feDataWriter::ignoreWhiteSpace(std::istream &in)
    {
        while (in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\n')
            {
                in.get();
            }
    }

fe::feDataWriter::feDataWriter()
    {
        m_baseItem = fe::imp::dataWriter::object("serialized_items", "");
        m_itemStack.push(&m_baseItem);
        m_writingObject = false;
    }

fe::feDataWriter::feDataWriter(const char *initialName)
    {
        m_baseItem = fe::imp::dataWriter::object(initialName, "");
        m_itemStack.push(&m_baseItem);
        m_writingObject = false;
    }

void fe::feDataWriter::setTitle(const std::string &title)
    {
        m_currentTitle = title;
        // We can assume that the top item is an object since you can only (theoretically) set titles in the object
        ObjectType *top = static_cast<ObjectType*>(m_itemStack.top());
        if (top->m_title != title)
            {
                ObjectType *newTopObject = nullptr;
                m_itemStack.pop();
                switch (m_itemStack.top()->m_type)
                    {
                        case ItemEnum::LIST:
                            newTopObject = static_cast<ObjectType*>(static_cast<ListType*>(m_itemStack.top())->getSubObject(title.c_str()));
                            if (newTopObject) 
                                {
                                    static_cast<ListType*>(m_itemStack.top())->m_storedItems.emplace(top);
                                }
                            else
                                {
                                    top->m_title = title;
                                    newTopObject = top;
                                }
                            break;
                        case ItemEnum::OBJECT:
                            newTopObject = static_cast<ObjectType*>(static_cast<ObjectType*>(m_itemStack.top())->getSubObject(title.c_str()));
                            break;
                        default:
                            break;
                    }
                FE_ASSERT(newTopObject, "Object with title does not exist in data");

                m_itemStack.push(newTopObject);

            }
    }

void fe::feDataWriter::startObject(const std::string &id)
    {
        startItem<ObjectType>(id.c_str(), m_currentTitle.c_str());
    }

void fe::feDataWriter::startObjectList(const std::string &id)
    {
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        ListType *list = static_cast<ListType*>(object->getSubItemType(id.c_str(), ItemEnum::LIST));
        if (!list)
            {
                startItem<ListType>(id.c_str());
            }
        else
            {
                m_itemStack.push(list);
            }
        startObject(id);
    }

void fe::feDataWriter::startObjectRead(const std::string &id)
    {
        FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT, "Current stack top is not an object - cannot start new object");
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        ObjectType *objectMember = static_cast<ObjectType*>(object->getSubItemType(id.c_str(), ItemEnum::OBJECT));
        m_itemStack.push(objectMember);
    }

void fe::feDataWriter::startObjectListRead(const std::string &id)
    {
        FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT, "Current stack top is not an object - cannot start new object");
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        ListType *list = static_cast<ListType*>(object->getSubItemType(id.c_str(), ItemEnum::LIST));
        m_itemStack.push(list);
        m_itemStack.push(list->getSubItem());
    }

void fe::feDataWriter::endObject()
    {
        if (m_itemStack.top()->m_cleanup)
            {
                delete m_itemStack.top();
            }
        m_itemStack.pop();
        m_currentTitle = "";
    }

void fe::feDataWriter::endObjectList()
    {
        endObject();
        endObject();
        m_currentTitle = "";
    }

void fe::feDataWriter::write(const std::string &id, const std::string &value)
    {
        FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT, "Current stack top is not an object - cannot start new object");
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        object->m_memberItems.emplace_back(new DataType(id.c_str(), value.c_str()));
    }

void fe::feDataWriter::writeList(const std::string &id, const std::string &value)
    {
        FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT, "Current stack top is not an object - cannot start new object");
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        ListType *list = static_cast<ListType*>(object->getSubItemType(id.c_str(), ItemEnum::LIST));
        if (!list)
            {
                object->m_memberItems.emplace_back(new ListType(id.c_str()));
                list = static_cast<ListType*>(object->m_memberItems.back().get());
            }

        list->m_storedItems.emplace(new DataType(id.c_str(), value.c_str()));
    }

std::string fe::feDataWriter::read(const std::string &id)
    {
        FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT, "Current stack top is not an object - cannot read data");
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        DataType *data = static_cast<DataType*>(object->getSubItemType(id.c_str(), ItemEnum::DATA));
        if (!data)
            {
                return "";
            }
        return data->m_data;
    }

std::string fe::feDataWriter::readList(const std::string &id)
    {
        FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT, "Current stack top is not an object - cannot read data");
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        DataType *data = static_cast<DataType*>(static_cast<ListType*>(object->getSubItemType(id.c_str(), ItemEnum::LIST))->getSubItem());
        if (!data)
            {
                return "";
            }
        return data->m_data;
    }

void fe::feDataWriter::outData(std::ostream &out)
    {
        m_baseItem.output(out);
    }

void fe::feDataWriter::readData(std::istream &in)
    {
        constexpr unsigned int bufferSize = 512;
        std::array<char, bufferSize> line;

        clearData();
        if (!(in.get() == 'o' && in.get() == 'b' && in.get() == 'j'))
            {
                FE_LOG_ERROR("File has invalid format");
                return;
            }
        in.get();

        in.getline(&line[0], bufferSize, '{');
        m_baseItem.m_name = line.data();

        fe::nonFixedStack<ItemEnum> itemStack;
        itemStack.push(ItemEnum::OBJECT);

        // Backwards Compatability
        std::string currentListName = "";

        while (!itemStack.empty())
            {
                ignoreWhiteSpace(in);

                char nextChar = in.peek();
                if (nextChar == '}' || nextChar == ']')
                    {
                        in.get();
                        itemStack.pop();
                        endObject();
                    }
                else
                    {
                        bool oldListVersion = false;
                        in.getline(&line[0], bufferSize, ' ');
                        switch (FE_STR_CONST(std::string(line.data()).c_str()))
                            {
                                case FE_STR_CONST("itm"):
                                    oldListVersion = true;
                                case FE_STR_CONST("dat"):
                                    itemStack.push(ItemEnum::DATA);
                                    break;
                                case FE_STR_CONST("obj"):
                                    itemStack.push(ItemEnum::OBJECT);
                                    break;
                                case FE_STR_CONST("ols"):
                                case FE_STR_CONST("ils"):
                                case FE_STR_CONST("list"):
                                    itemStack.push(ItemEnum::LIST);
                                    break;
                                default:
                                    break;
                            }

                        switch (itemStack.top())
                            {
                                case ItemEnum::DATA:
                                    {
                                        std::string name = currentListName;
                                        if (!oldListVersion)
                                            {
                                                in.getline(&line[0], bufferSize, ':');
                                                name = line.data();
                                            }

                                        in.getline(&line[0], bufferSize, ';');
                                        std::string data = line.data();

                                        startItem<DataType>(name.c_str(), data.c_str());
                                        endObject();
                                        itemStack.pop();
                                    }
                                    break;
                                case ItemEnum::OBJECT: 
                                    {
                                        in.getline(&line[0], bufferSize, '{');
                                        std::string name = "";
                                        std::string title = "";

                                        bool gettingTitle = false;
                                        for (auto &c : line)
                                            {
                                                if (c == ':')
                                                    {
                                                        gettingTitle = true;
                                                    }
                                                else if (c == '\0')
                                                    {
                                                        break;
                                                    }
                                                else
                                                    {
                                                        if (gettingTitle)
                                                            {
                                                                title += c;
                                                            }
                                                        else
                                                            {
                                                                name += c;
                                                            }
                                                    }
                                            }
                                        
                                        startItem<ObjectType>(name.c_str(), title.c_str());
                                    }
                                    break;
                                case ItemEnum::LIST:
                                    in.getline(&line[0], bufferSize, '[');
                                    startItem<ListType>(std::string(line.data()).c_str());
                                    currentListName = line.data();
                                    break;
                                default:
                                    break;
                        }
                    }
            }
        m_itemStack.push(&m_baseItem);
    }

bool fe::feDataWriter::listHasItems(const std::string &list)
    {
        FE_ASSERT(m_itemStack.top()->m_type == ItemEnum::OBJECT, "Current stack top is not an object - cannot read data");
        ObjectType *object = static_cast<ObjectType*>(m_itemStack.top());
        ListType *listObject = static_cast<ListType*>(object->getSubItemType(list.c_str(), ItemEnum::LIST));
        return listObject && listObject->hasItems();
    }

void fe::feDataWriter::clearData()
    {
        while (m_itemStack.top() != &m_baseItem)
            {
                m_itemStack.pop();
            }
        m_baseItem.m_memberItems.clear();
    }

bool fe::imp::dataWriter::list::hasItems() const
    {
        return !m_storedItems.empty();
    }

fe::imp::dataWriter::item *fe::imp::dataWriter::list::getSubItem()
    {
        fe::imp::dataWriter::item *item = m_storedItems.top();
        m_storedItems.pop();
        item->m_cleanup = true;
        return item;
    }

fe::imp::dataWriter::object *fe::imp::dataWriter::list::getSubObject(const char *title)
    {
        fe::imp::dataWriter::object *returnObj = nullptr;
        
        fe::str titleStr = FE_STR_CONST(title);
        fe::nonFixedStack<fe::imp::dataWriter::item*> items;
        while (!m_storedItems.empty())
            {
                if (m_storedItems.top()->m_type == fe::imp::dataWriter::itemType::OBJECT &&
                    FE_STR_CONST(static_cast<fe::imp::dataWriter::object*>(m_storedItems.top())->m_title.c_str()) == titleStr)
                    {
                        returnObj = static_cast<fe::imp::dataWriter::object*>(m_storedItems.top());
                        break;
                    }
                else
                    {
                        items.push(m_storedItems.top());
                    }
                m_storedItems.pop();
            }

        while (!items.empty())
            {
                m_storedItems.emplace(items.top());
                items.pop();
            }

        return returnObj;
    }

fe::imp::dataWriter::list::~list()
    {
        while (!m_storedItems.empty())
            {
                delete m_storedItems.top();
                m_storedItems.pop();
            }
    }

fe::imp::dataWriter::item *fe::imp::dataWriter::object::getSubItem(const char *name) const
    {
        fe::str nameStr = FE_STR_CONST(name);
        for (unsigned int i = 0; i < m_memberItems.size(); i++)
            {
                if (FE_STR_CONST(m_memberItems[i]->m_name.c_str()) == nameStr)
                    {
                        return m_memberItems[i].get();
                    }
            }

        return nullptr;
    }

fe::imp::dataWriter::item *fe::imp::dataWriter::object::getSubItemType(const char *name, fe::imp::dataWriter::itemType type) const
    {
        fe::str nameStr = FE_STR_CONST(name);
        for (unsigned int i = 0; i < m_memberItems.size(); i++)
            {
                if (m_memberItems[i]->m_type == type && FE_STR_CONST(m_memberItems[i]->m_name.c_str()) == nameStr)
                    {
                        return m_memberItems[i].get();
                    }
            }

        return nullptr;
    }

fe::imp::dataWriter::object *fe::imp::dataWriter::object::getSubObject(const char *title) const
    {
        fe::str titleStr = FE_STR_CONST(title);
        for (unsigned int i = 0; i < m_memberItems.size(); i++)
            {
                // We can't assume that this sub-item is an object as objects can store any type
                if (m_memberItems[i]->m_type == fe::imp::dataWriter::itemType::OBJECT &&
                    FE_STR_CONST(static_cast<fe::imp::dataWriter::object*>(m_memberItems[i].get())->m_title.c_str()) == titleStr)
                    {
                        return static_cast<fe::imp::dataWriter::object*>(m_memberItems[i].get());
                    }
            }

        return nullptr;
    }

fe::imp::dataWriter::item *fe::imp::dataWriter::object::getSubItemTypeFromHeiarchy(const char *name, fe::imp::dataWriter::itemType type)
    {
        fe::str nameStr = FE_STR_CONST(name);
        for (unsigned int i = 0; i < m_memberItems.size(); i++)
            {
                if (m_memberItems[i]->m_type == type && FE_STR_CONST(m_memberItems[i]->m_name.c_str()) == nameStr)
                    {
                        return m_memberItems[i].get();
                    }
                else if (m_memberItems[i]->m_type == fe::imp::dataWriter::itemType::OBJECT)
                    {
                        fe::imp::dataWriter::item *item = static_cast<fe::imp::dataWriter::object*>(m_memberItems[i].get())->getSubItemTypeFromHeiarchy(name, type);
                        if (item)
                            {
                                return item;
                            }
                    }
            }

        return nullptr;
    }

void fe::imp::dataWriter::list::output(std::ostream &out, const std::string &indent)
    {
        out << indent << "list " << m_name << "[\n";
        fe::nonFixedStack<fe::imp::dataWriter::item*> tempStack;
        while (!m_storedItems.empty())
            {
                tempStack.push(m_storedItems.top());
                m_storedItems.pop();
                tempStack.top()->output(out, indent + "    ");
            }

        while (!tempStack.empty())
            {
                m_storedItems.emplace(tempStack.top());
                tempStack.pop();
            }

        out << indent + "]\n";
    }

void fe::imp::dataWriter::object::output(std::ostream &out, const std::string &indent)
    {
        out << indent << "obj " << m_name;
        if (m_title != "")
            {
                out << ":" << m_title;
            }
        out << "{\n";

        for (auto &child : m_memberItems)
            {
                child->output(out, indent + "    ");
            }

        out << indent << "}\n";
    }

void fe::imp::dataWriter::data::output(std::ostream &out, const std::string &indent)
    {
        out << indent << "dat " << m_name << ":" << m_data << ";\n";
    }
