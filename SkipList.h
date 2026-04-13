#pragma once
#include "coin_flip.h"
#include <iostream>

template<class T>

class SkipList {
    class Node {
        friend SkipList;
        int sentinel = 0;//not a sentinel, overridden in constructor if Node is sentinel
        Node* prev = nullptr;
        Node* next = nullptr;
        Node* below = nullptr;
        Node* above = nullptr;
        T value;

        Node(T v) : value(v) {}//item constructor

        Node(bool const  i) {//sentinel constructor
            if (i == false) {
                sentinel = -1;
            } else {
                sentinel = 1;
            }
        }

        bool is_sentinel() const {
            return sentinel != 0;
        }

        bool operator<=(T other) {
            if (sentinel == -1) {
                return true;
            }
            if (sentinel == 1) {
                return false;
            }
            return value <= other;
        }

        bool operator==(T other) {
            if (sentinel != 0) {
                return false;
            }
            return value == other;
        }

        bool operator==(Node other) {
            if (is_sentinel()) {
                return sentinel == other.sentinel;
            }
            if (other.is_sentinel()) {
                return false;
            }
            return value == other.value;
        }


    };
    class Iterator {
        friend SkipList;
        Node* curr;

        Iterator(Node* n) : curr(n) {}
    public:
        T& operator*() {
            return curr->value;
        }

        Iterator& operator++() {
            curr = curr->next;
            return *this;
        }

        bool operator==(Iterator other) {
            return *curr == *other.curr;
        }

        bool operator!=(Iterator other) {
            return !(*curr == *other.curr);
        }
    };
    Node* start;
    size_t num_items;

    Node* base() {
        Node* b = start;
        while (b->below != nullptr) {
            b = b->below;
        }
        return b;
    }

    void add_row() {
        start->above = new Node(false);//add left sentinel above
        start->above->below = start;
        Node* end = start->next;
        while (!end->is_sentinel()) {
            end = end->next;
        }
        end->above = new Node(true);//add right sentinel above
        end->above->below = end;
        start->above->next = end->above;//connect them
        end->above->prev = start->above;

        start = start->above;
    }

    Node* find(T item) {
        Node* curr = start;
        while (true) {
            while (*curr->next <= item) {//must dereference to use overloaded comparison with T item
                curr = curr->next;
            }
            if (curr->below == nullptr) {
                break;
            }
            curr = curr->below;
        }
        return curr;
    }

    Node* attach_after(Node* predecessor, T item) {//create a new Node(item), attach it after predecessor, return its pointer
        Node* new_node = new Node(item);
        new_node->prev = predecessor;
        new_node->next = predecessor->next;
        new_node->next->prev = new_node;
        new_node->prev->next = new_node;
        return new_node;
    }

    void destroy(Node* target) {//delete the node and all nodes above
        target->prev->next = target->next;
        target->next->prev = target->prev;//disconnected horizontally
        Node* next = target->above;
        delete target;
        if (next != nullptr) {
            destroy(next);
        }
    }
public:
    SkipList() : num_items(0) {
        start = new Node(false);//left sentinel
        start->next = new Node(true);//right sentinel
    }

    ~SkipList() {
        clear();//removes middle nodes
        Node* nxt;
        while (start != nullptr) {
            nxt = start->below;
            delete start->next;
            delete start;
            start = nxt;
        }
    }

    Iterator begin() {
        return Iterator(base()->next);
    }

    Iterator end() {
        Node* b = base()->next;
        while (!b->is_sentinel()) {
            b = b->next;
        }
        return Iterator(b);
    }

    bool insert(T item) {
        Node* target = find(item);
        if (*target == item) {
            return false;
        }
        Node* new_node = attach_after(target, item);
        while (coin()) {
            Node* ladder = new_node->prev;
            while (ladder->above == nullptr) {
                if (ladder->is_sentinel()) {//hit left wall with no ladder up, must be at top of list
                    add_row();
                    break;
                }
                ladder = ladder->prev;
            }
            new_node->above = attach_after(ladder->above, item);
            new_node->above->below = new_node;
            new_node = new_node->above;
        }
        num_items++;
        return true;
    }

    bool remove(T item) {
        Node* target = find(item);
        if (!(*target == item)) {
            return false;
        }
        destroy(target);
        num_items--;
        return true;
    }

    bool contains(T item) {
        Node* target = find(item);
        return *target == item;
    }

    size_t size() const {
        return  num_items;
    }

    void clear() {
        Node* b = base();
        while (!b->next->is_sentinel()) {
            destroy(b->next);
        }
        num_items = 0;
    }
};