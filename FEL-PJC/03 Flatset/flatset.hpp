#pragma once

#include <iostream>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

template <typename T, typename Comparator = std::less<T> >
class flat_set {
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using size_type = std::size_t;
    using value_type = T;
 
    // Speciální member funkce se chovají běžným stylem
    flat_set() : data(), comparator() { };
    flat_set(Comparator const& cmp) : flat_set() { comparator = cmp; };
    flat_set(flat_set const& rhs) {
        flat_set temp(rhs.begin(), rhs.end(), rhs.comparator);
        swap(temp);
    }
    flat_set(flat_set && rhs) : flat_set(rhs) { };
    flat_set& operator=(flat_set const& rhs) {
        flat_set temp(rhs.begin(), rhs.end(), rhs.comparator);
        this->swap(temp);
        return *this;
    }
    flat_set& operator=(flat_set && rhs) {
        this->swap(rhs);
        return *this;
    }
    ~flat_set() { };
 
    // Konstruktory co vytvoří flat_set z prvků definovaných jako
    // [first, last). Alokují pouze jednou pokud je to možné.
    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last): flat_set() {
        data.reserve(std::distance(first, last));
        insert(first, last);
    }
    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last, Comparator const& cmp) : flat_set() {
        comparator = cmp;
        data.reserve(std::distance(first, last));
        insert(first, last);
    };
 
    // Vloží prvek do setu, buďto pomocí kopie, nebo pomocí přesunu.
    std::pair<iterator, bool> insert(T const& item) {
        try {
            auto found = find(item);
            if (found != end()) {
                return std::pair<iterator, bool>(found, false);
            }
            auto place = std::upper_bound(begin(), end(), item, comparator);
            auto insert_iterator = data.insert(place, item);
            return std::pair<iterator, bool>(insert_iterator, true);
        } catch (std::bad_alloc e) {
            return std::pair<iterator, bool>(end(), false);
        }
    }

    std::pair<iterator, bool> insert(T&& item) {
        try {
            auto found = find(item);
            if (found != end()) {
                return std::pair<iterator, bool>(found, false);
            }
            auto place = std::upper_bound(begin(), end(), item, comparator);
            auto insert_iterator = data.insert(place, std::move(item));
            return std::pair<iterator, bool>(insert_iterator, true);
        } catch (std::bad_alloc e) {
            return std::pair<iterator, bool>(end(), false);
        }
    }
    
    // Vloží prvky z [first, last), alokuje pouze jednou pokud je to možné
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
        while (first != last) {
            insert(*first);
            first++;
        }
    }
 
    // Smaže prvek na který ukazuje i, vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator i) { return data.erase(i); };
    // Smaže všechny prvky z [from, to), vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator from, const_iterator to) { return data.erase(from, to); };
    // Iterátory předané dovnitř erase odkazují dovnitř setu.
 
    // Smaže prvek rovný klíči pokud existuje.
    // Vrátí kolik prvků bylo smazáno
    size_type erase(value_type const& key) {
        int counter = 0;
        auto it = find(key);
        while (it != end()) {
            erase(it);
            counter++;
            it = find(key);
        }

        return counter;
    }
    
    // Běžné funkce k vytvoření iterátorů
    iterator begin() noexcept { return data.begin(); };
    iterator end() noexcept { return data.end(); };
    const_iterator begin() const noexcept { return data.begin(); };
    const_iterator end() const noexcept { return data.end(); };
    const_iterator cbegin() const noexcept { return data.cbegin(); };
    const_iterator cend() const noexcept { return data.cend(); };
 
    bool empty() const { return data.empty(); }
    
    size_type size() const { return data.size(); }
    
    size_type capacity() const { return data.capacity(); }
 
    // Zajistí, aby se do instance flat_setu dalo vložit c prvků
    // aniž by byla potřeba realokace
    void reserve(size_type c) { data.reserve(c); }
 
    // Vymaže všechny prvky ze setu
    void clear() { data.clear(); }

    // Vrátí iterátor ukazující na prvek ekvivalentní s v, nebo end(),
    // pokud takový prvek není uvnitř setu
    iterator find(T const& v) {
        auto it = data.begin();
        while (it != data.end()) {
            if (comparator(*it, v) == false && comparator(v, *it) == false) {
                return it;
            }
            it++;
        }

        return it;
    }
    const_iterator find(T const& v) const { return find(v); }
 
    // Vrátí iterátor k prvnímu prvku, který není menší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator lower_bound(T const& t) { return std::lower_bound(begin(), end(), t, comparator); }
    const_iterator lower_bound(T const& t) const { return lower_bound(t); }
 
    // Vrátí iterátor k prvnímu prvku, který je větší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator upper_bound(T const& t) { return std::upper_bound(begin(), end(), t, comparator); }
    const_iterator upper_bound(T const& t) const { return upper_bound(t); }
 
    // Prohodí obsah dvou setů (včetně komparátoru)
    void swap(flat_set& o) {
        std::swap(data, o.data);
        std::swap(comparator, o.comparator);
    }

    std::vector<T> getData() const { return data; };
private:
    std::vector<T> data;
    Comparator comparator;
};
 
// porovnání probíhá lexikograficky

template <typename T>
bool operator==(flat_set<T> const& a, flat_set<T> const& b) {
    // if (a.comparator != b.comparator) return false;
    return distance(a.begin(), a.end()) == distance(b.begin(), b.end()) && equal(a.begin(), a.end(), b.begin());
}

template <typename T>
bool operator!=(flat_set<T> const& a, flat_set<T> const& b) {
    return !(a == b);
}

template <typename T>
bool operator< (flat_set<T> const& a, flat_set<T> const& b) {
    return a.getData() < b.getData();
}

template <typename T>
bool operator<=(flat_set<T> const& a, flat_set<T> const& b) {
    return a.getData() <= b.getData();
}

template <typename T>
bool operator>=(flat_set<T> const& a, flat_set<T> const& b) {
    return a.getData() >= b.getData();
}

template <typename T>
bool operator> (flat_set<T> const& a, flat_set<T> const& b) {
    return a.getData() > b.getData();
}
 
// Prohodí obsah dvou setů (včetně komparátoru)
template <typename T>
void swap(flat_set<T> const& a, flat_set<T> const& b) {
    a.swap(b);
}
