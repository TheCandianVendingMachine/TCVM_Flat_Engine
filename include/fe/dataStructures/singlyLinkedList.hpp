// singlyLinkedList.hpp
// A singly linked list data structure
#pragma once
#include "../flatEngineExport.hpp"

namespace fe
    {
        template<typename T>
        class singlyLinkedList
            {
                public:
                    struct node
                        {
                            T m_data = T();
                            node *m_next = nullptr;
                        };

                private:
                    node *m_head;

                public:
                    singlyLinkedList() : m_head(nullptr) {}
                    // Allocates a new node and inserts into the linked-list
                    void insert(T data);
                    // Inserts the node into the linked list
                    void insert(node *node);
                    // Inserts the node into the linked list at location
                    void insert(node *previous, node *node);
                    
                    // Replaces node base with node
                    void replace(node *base, node *replacement);

                    // Removes the node from the linked list and frees the memory
                    void remove(node *node);

                    // Clears the linked list of all nodes
                    void clear(bool dealloc = true);

                    node *head() const;

                    // Returns true if there is a loop in the linked list
                    bool loop() const;

            };

        template<typename T>
        void singlyLinkedList<T>::insert(T data)
            {
                node *newNode = new node;
                newNode->m_data = data;
                newNode->m_next = nullptr;
                insert(newNode);
            }

        template<typename T>
        void singlyLinkedList<T>::insert(node *node)
            {
                if (!m_head)
                    {
                        m_head = node;
                    }
                else
                    {
                        singlyLinkedList::node *temp = m_head;
                        while (temp->m_next)
                            {
                                temp = temp->m_next;
                            }
                        temp->m_next = node;
                    }
            }

        template<typename T>
        void singlyLinkedList<T>::insert(node *previous, node *node)
            {
                if (!previous)
                    {
                        node->m_next = m_head;
                        m_head = node;
                    }
                else
                    {
                        node->m_next = previous->m_next;
                        previous->m_next = node;
                    }
            }

        template<typename T>
        void singlyLinkedList<T>::replace(node *base, node *replacement)
            {
                node *it = head();
                node *itPrev = nullptr;
                while (it)
                    {
                        if (it == base)
                            {
                                break;
                            }
                        itPrev = it;
                        it = it->m_next;
                    }

                if (!itPrev)
                    {
                        // head
                        m_head = replacement;
                        m_head->m_next = base->m_next;
                    }
                else
                    {
                        itPrev->m_next = replacement;
                        replacement->m_next = base->m_next;
                    }
            }

        template<typename T>
        void singlyLinkedList<T>::remove(node *node)
            {
                singlyLinkedList::node *prev = m_head;
                if (prev->m_next)
                    {
                        while (prev->m_next != node)
                            {
                                prev = prev->m_next;
                            }
                    }

                if (prev)
                    {
                        prev->m_next = node->m_next;
                    }

                delete node;
                node = nullptr;
            }

        template<typename T>
        void singlyLinkedList<T>::clear(bool dealloc)
            {
                if (dealloc)
                    {
                        while (m_head)
                            {
                                node *next = m_head->m_next;
                                delete m_head;
                                m_head = next;
                            }
                    }
                m_head = nullptr;
            }

        template<typename T>
        typename fe::singlyLinkedList<T>::node *singlyLinkedList<T>::head() const
            {
                return m_head;
            }

        template<typename T>
        bool singlyLinkedList<T>::loop() const
            {
                node *hare = m_head;
                node *tortise = m_head;
                do
                    {
                        if (!hare)
                            {
                                return false;
                            }
                        hare = hare->m_next;
                        if (!hare)
                            {
                                return false;
                            }
                        hare = hare->m_next;
                        tortise = tortise->m_next;

                        if (hare == tortise)
                            {
                                return true;
                            }
                } while (hare);

                // should never reach this
                return false;
            }
    }