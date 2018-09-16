// doublyLinkedList.hpp
// A doubly linked list data structure
#pragma once
#include "../flatEngineExport.hpp"

namespace fe
    {
        template<typename T>
        class doublyLinkedList
            {
                public:
                    struct node
                        {
                            T m_data = T();
                            node *m_next = nullptr;
                            node *m_prev = nullptr;
                            // Internal flag only
                            bool m_alloc = false;
                        };

                private:
                    node *m_head;

                    unsigned int m_size;

                public:
                    doublyLinkedList() : m_head(nullptr), m_size(0) {}
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
                    void clear();

                    node *head() const;

                    // Returns node at index
                    node *at(unsigned int index) const;

                    unsigned int size() const;

                    // Returns true if there is a loop in the linked list
                    bool loop() const;

                    // returns true if there is no head node
                    bool empty() const;

            };

        template<typename T>
        void doublyLinkedList<T>::insert(T data)
            {
                node *newNode = new node;
                newNode->m_alloc = true;
                newNode->m_data = data;
                newNode->m_next = nullptr;
                newNode->m_prev = nullptr;
                insert(newNode);
            }

        template<typename T>
        void doublyLinkedList<T>::insert(node *node)
            {
                if (!m_head)
                    {
                        m_head = node;
                    }
                else
                    {
                        doublyLinkedList::node *temp = m_head;
                        while (temp->m_next)
                            {
                                temp = temp->m_next;
                            }
                        temp->m_next = node;
                        node->m_prev = temp;
                    }
                m_size++;
            }

        template<typename T>
        void doublyLinkedList<T>::insert(node *previous, node *node)
            {
                if (!previous)
                    {
                        node->m_next = m_head;
                        m_head = node;
                        m_head->m_previous = node;
                    }
                else
                    {
                        node->m_next = previous->m_next;
                        previous->m_next = node;

                        node->m_next->m_previous = node;
                        node->m_previous = previous;
                    }
            }

        template<typename T>
        void doublyLinkedList<T>::replace(node *base, node *replacement)
            {
                if (!base->m_prev)
                    {
                        // head
                        m_head = replacement;
                        m_head->m_next = base->m_next;
                        base->m_next = m_head;
                    }
                else
                    {
                        base->m_prev->m_next = replacement;
                        replacement->m_next = base->m_next;
                        base->m_next->m_previous = replacement;
                    }
            }

        template<typename T>
        void doublyLinkedList<T>::remove(node *node)
            {
                if (node->m_prev)
                    {
                        node->m_prev->m_next = node->m_next;
                        if (node->m_next) 
                            {
                                node->m_next->m_prev = node->m_prev;
                            }
                    }
                else
                    {
                        m_head = node->m_next;
                        if (node->m_next) 
                            {
                                node->m_next->m_prev = nullptr;
                            }
                    }

                delete node;
                node = nullptr;
                m_size--;
            }

        template<typename T>
        void doublyLinkedList<T>::clear()
            {
                while (m_head)
                    {
                        node *next = m_head->m_next;
                        if (m_head->m_alloc)
                            {
                                delete m_head;
                            }
                        m_head = next;
                    }
                m_head = nullptr;
                m_size = 0;
            }

        template<typename T>
        typename fe::doublyLinkedList<T>::node *doublyLinkedList<T>::head() const
            {
                return m_head;
            }

        template<typename T>
        typename fe::doublyLinkedList<T>::node *doublyLinkedList<T>::at(unsigned int index) const
            {
                if (index >= m_size) return nullptr;
                doublyLinkedList::node *node = m_head;
                while (index-- > 0)
                    {
                        node = node->m_next;
                    }
                return node;
            }

        template<typename T>
        unsigned int fe::doublyLinkedList<T>::size() const
            {
                return m_size;
            }

        template<typename T>
        bool doublyLinkedList<T>::loop() const
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

        template<typename T>
        bool doublyLinkedList<T>::empty() const
            {
                return m_head == nullptr;
            }
    }