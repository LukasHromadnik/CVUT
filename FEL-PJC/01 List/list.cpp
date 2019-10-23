#include "list.hpp"

#include <utility>
#include <algorithm>
#include <iostream>

using std::cout;
using std::endl;

using std::size_t;

namespace pjc {

    list::list(const list& rhs) : list() {
        for (auto it : rhs) {
            push_back(it);
        }
    }
    
    list& list::operator=(const list& rhs) {
        list temp(rhs);
        swap(temp);

        return *this;
    }
 
    list::list(list&& rhs) : list() {
        for (auto it : rhs) {
            push_back(it);
        }
    }
   
    list& list::operator=(list&& rhs) {
        list temp(rhs);
        swap(temp);

        return *this;
    }
   
    list::~list() {
        node *current = this->head;
        while (current) {
            node *next = current->next;
            delete current;
            current = next;
        }
    }

    list::list(const std::vector<double>& vec) {
        for (double value : vec) {
            push_back(value);
        }
    }

    void list::push_back(double elem) {
        node *newNode = new list::node();
        newNode->val = elem;

        if (this->head == nullptr) {
            this->head = newNode;
        }

        if (this->tail == nullptr) {
            this->tail = newNode;
            node *end = new list::node();
            end->prev = this->tail;
            this->tail->next = end;
            end->val = -1; // debug
        } else {
            node *end = this->tail->next;
            node *current_end = this->tail;

            current_end->next = newNode;
            newNode->prev = current_end;
            newNode->next = end;
            end->prev = newNode;
            
            this->tail = newNode;
        }
        
        this->num_elements++;
    }

    void list::pop_back() {
        if (this->tail == nullptr) return;

        if (this->head == this->tail) {
            delete this->tail->next;
            delete this->tail;
            this->head = nullptr;
            this->tail = nullptr;
        } else {
            node *tail = this->tail;
            this->tail = tail->prev;
            this->tail->next = tail->next;
            tail->next->prev = this->tail;
            delete tail;
        }

        this->num_elements--;
    }

    double const& list::back() const {
        return this->tail->val;
    }

    double& list::back() {
        return this->tail->val;
    }

    void list::push_front(double elem) {
        node *newNode = new list::node();
        newNode->val = elem;

        if (this->tail == nullptr) {
            this->tail = newNode;
            node *end = new list::node();
            end->val = -1;
            this->tail->next = end;
            end->prev = this->tail;
        }

        if (this->head == nullptr) {
            this->head = newNode;
        } else {
            node *current = this->head;
            newNode->next = current;
            current->prev = newNode;
            this->head = newNode;
        }

        this->num_elements++;
    }
    
    void list::pop_front() {
        if (this->head == nullptr) return;

        if (size() == 1) {
            delete this->tail->next;
            delete this->tail;
            this->head = nullptr;
            this->tail = nullptr;
        } else {
            node *head = this->head;
            this->head = head->next;
            head->prev = nullptr;
            delete head;
        }

        this->num_elements--;
    }

    double const& list::front() const {
        return this->head->val;
    }

    double& list::front() {
        return this->head->val;
    }
    
    void list::reverse() {
        if (empty()) return;

        node *current = this->head;
        node *last = nullptr;

        node *end = this->tail->next;
        this->tail->next = nullptr;

        while (current != nullptr) {
            node *next = current->next;
            node *prev = current->prev;
            current->next = prev;
            current->prev = next;

            last = current;
            current = next;
        }

        this->tail = this->head;
        this->head = last;
        this->tail->next = end;
        end->prev = this->tail;
    }

    size_t list::size() const {
        return num_elements;
    }
    
    bool list::empty() const {
        return num_elements == 0;
    }
    
    void list::remove(double value) {
        if (empty()) return;

        node *current = this->head;
        while (current != this->tail->next) {
            if (current->val != value) {
                current = current->next;
                continue;
            }

            node *next = current->next;
            
            if (current == this->tail) {
                pop_back();
                break;
            } else if (current == this->head) {
                this->head = next;
                next->prev = nullptr;
            } else {
                current->prev->next = next;
                next->prev = current->prev;
            }

            delete current;
            current = next;
            num_elements--;
        }
    }
    
    void list::swap(list& rhs) {
        node *head = this->head;
        node *tail = this->tail;

        this->head = rhs.head;
        this->tail = rhs.tail;

        rhs.head = head;
        rhs.tail = tail;

        size_t num_elements = this->num_elements;
        this->num_elements = rhs.num_elements;
        rhs.num_elements = num_elements;
    }

    list::iterator list::begin() {
        return list::iterator(this->head, this);
    }

    list::iterator list::end() {
        if (this->tail != nullptr) {
            return list::iterator(this->tail->next, this);
        } else {
            return list::iterator(nullptr, this);
        }
    }       

    list::const_iterator list::begin() const {
        return list::const_iterator(this->head, this);
    }

    list::const_iterator list::end() const {
        if (this->tail != nullptr)
            return list::const_iterator(this->tail->next, this);
        else 
            return list::const_iterator(nullptr, this);
    }

    list::const_iterator list::cbegin() const {
        return list::const_iterator(this->head, this);
    }

    list::const_iterator list::cend() const {
         if (this->tail != nullptr)
            return list::const_iterator(this->tail->next, this);
        else 
            return list::const_iterator(nullptr, this);
    }

    bool list::operator==(const list& rhs) const {
        if (this->size() != rhs.size()) return false;

        node *currentLeft = this->head;
        node *currentRight = rhs.head;

        for (size_t i = 1; i <= rhs.size(); i++) {
            if (currentLeft->val != currentRight->val) return false;
            currentLeft = currentLeft->next;
            currentRight = currentRight->next;
        }

        return true;
    }
    
    bool list::operator<(const list& rhs) const {
        node *currentLeft = this->head;
        node *currentRight = rhs.head;

        if (this->empty() && rhs.empty()) return false;
        if (this->empty() && !rhs.empty()) return true;

        for (int i = 0; i < (int) std::min(this->size(), rhs.size()); i++) {
            if (currentLeft->val < currentRight->val) return true;
            if (currentLeft->val > currentRight->val) return false;
            currentLeft = currentLeft->next;
            currentRight = currentRight->next;
        }

        return (this->size() < rhs.size());
    }

    std::pair<list, list> list::split(const_iterator place) {
        list lhs;
        list rhs;

        auto it = cbegin();
        while (it != place) {
            lhs.push_back(*it);
            it++;
            pop_front();
        }

        rhs.swap(*this);

        this->head = nullptr;
        this->tail = nullptr;

        return std::pair<list, list>(lhs, rhs);
    }

    void list::merge(list& rhs) {
        std::vector<double> values;
        
        for (auto it = begin(); it != end(); it++) {
            values.push_back(*it);
        }

        for (auto it = rhs.begin(); it != rhs.end(); it++) {
            values.push_back(*it);
        }

        rhs.~list();
        rhs.head = nullptr;
        rhs.tail = nullptr;
        rhs.num_elements = 0;

        std::sort(values.begin(), values.end());

        this->~list();
        this->head = nullptr;
        this->tail = nullptr;
        num_elements = 0;

        for (double value : values) {
            push_back(value);
        }
    }

    void list::sort() {
        std::vector<double> values;
        
        for (auto it = begin(); it != end(); it++) {
            values.push_back(*it);
        }

        std::sort(values.begin(), values.end());

        this->~list();
        this->head = nullptr;
        this->tail = nullptr;
        num_elements = 0;

        for (double value : values) {
            push_back(value);
        }
    }

    // Const Iterator
    list::const_iterator::const_iterator(node* ptr, const list* gen) {
        current_ptr = ptr;
        o_list = gen;
    }
     
    list::const_iterator& list::const_iterator::operator++() {
        if (current_ptr != nullptr)
            current_ptr = current_ptr->next;
        return *this;
    }

    list::const_iterator list::const_iterator::operator++(int) {
        auto result = *this;
        if (current_ptr != nullptr)
            current_ptr = current_ptr->next;
        return result;
    }

    list::const_iterator& list::const_iterator::operator--() {
        if (current_ptr != nullptr)
            current_ptr = current_ptr->prev;
        return *this;
    }

    list::const_iterator list::const_iterator::operator--(int) {
        auto result = *this;
        if (current_ptr != nullptr)
            current_ptr = current_ptr->prev;
        return result;
    }

    list::const_iterator::reference list::const_iterator::operator*() const {
        return current_ptr->val;
    }

    list::const_iterator::pointer list::const_iterator::operator->() const {
        return &current_ptr->val;
    }

    bool list::const_iterator::operator==(const list::const_iterator& rhs) const {
        return this->current_ptr == rhs.current_ptr && this->o_list == rhs.o_list;
    }

    bool list::const_iterator::operator!=(const list::const_iterator& rhs) const {
        return !(*this == rhs);
    }

    // Iterator
    list::iterator::iterator(node* ptr, const list* gen) {
        current_ptr = ptr;
        o_list = gen;
    }

    list::iterator& list::iterator::operator++() {
        if (current_ptr != nullptr)
            current_ptr = current_ptr->next;
        return *this;
    }

    list::iterator list::iterator::operator++(int) {
        auto result = *this;
        if (current_ptr != nullptr)
            current_ptr = current_ptr->next;
        return result;
    }

    list::iterator& list::iterator::operator--() {
        if (current_ptr != nullptr)
            current_ptr = current_ptr->prev;
        
        return *this;
    }

    list::iterator list::iterator::operator--(int) {
        auto result = *this;
        if (current_ptr != nullptr)
            current_ptr = current_ptr->prev;

        return result;
    }

    list::iterator::reference list::iterator::operator*() const {
        return current_ptr->val;
    }

    list::iterator::pointer list::iterator::operator->() const {
        return &current_ptr->val;
    }

    list::iterator::operator list::const_iterator() const {
        return list::const_iterator(this->current_ptr, this->o_list);
    }

    bool list::iterator::operator==(const list::iterator& rhs) const {
        return this->current_ptr == rhs.current_ptr && this->o_list == rhs.o_list;
    }

    bool list::iterator::operator!=(const list::iterator& rhs) const {
        return !(*this == rhs);
    }

    // Operatory
    bool operator!=(const list& lhs, const list& rhs) {
        return !(lhs == rhs);
    }

    bool operator>(const list& lhs, const list& rhs) {
        return !(lhs <= rhs);
    }
    
    bool operator<=(const list& lhs, const list& rhs) {
        auto leftIt = lhs.begin();
        auto rightIt = rhs.begin();

        for (int i = 0; i < (int) std::min(lhs.size(), rhs.size()); i++) {
            if (*leftIt < *rightIt) return true;
            if (*leftIt > *rightIt) return false;
            leftIt++;
            rightIt++;
        }

        return true;
    }

    bool operator>=(const list& lhs, const list& rhs) {
        auto leftIt = lhs.begin();
        auto rightIt = rhs.begin();

        if (rhs.empty()) return true;
        if (!rhs.empty() && lhs.empty()) return false;

        while (leftIt != lhs.end() || rightIt != rhs.end()) {
            if (*leftIt < *rightIt) return false;
            leftIt++;
            rightIt++;
        }

        return true;
    }

}
