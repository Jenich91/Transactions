#ifndef SRC_sfleta_MAP_H_
#define SRC_sfleta_MAP_H_

#include "tree.h"
#include <vector>

namespace sfleta {
template <typename K, typename T>
class Map : public Tree<K, T> {
 public:
    using key_type = K;
    using mapped_type = T;
    using value_type = std::pair<K, T>;
    using const_reference = const value_type&;
    using iterator = typename Tree<K, T>::Iterator;

    class Mapiterator : public iterator {
     public:
        Mapiterator() : iterator::Iterator() {}
        value_type operator*();
    };
    Mapiterator Begin() const;
    Mapiterator End() const;

    Map() : Tree<K, T>() {}
    Map(Map<K, T>&& t) { *this = std::move(t); }
    explicit Map(const Map<K, T>& t) : Tree<K, T>() { for (auto value : t) Insert(value); }
    explicit Map(std::initializer_list<value_type> const& items) { for (auto& value : items) Insert(value); }

    template <typename ... Args>
    std::vector<std::pair<iterator, bool>> Emplace(Args&&... args);
    std::pair<iterator, bool> Insert(const_reference value) { return Insert(value.first, value.second); }
    std::pair<iterator, bool> InsertOrAssign(const K& key, const T& obj);
    std::pair<iterator, bool> Insert(const K& key, const T& obj);
    Map<K, T>& operator=(Map<K, T>&& other);
    void Merge(Map<K, T>& other);
    T& operator[](const K& key);
    T& At(const K& key);
};
}  // namespace sfleta

#include "sfleta_map.cc"
#endif  //  SRC_sfleta_MAP_H_
