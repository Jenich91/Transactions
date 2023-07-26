#pragma once
#include <vector>
#include <utility>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <regex>
#include <locale>

#include "../storage.h"

namespace sfleta {
class HashTable : public Storage {
public:
    using keys_arr =  std::vector<key_type>;
    using values_arr = std::vector<data_type>;
private:
    using hash_type = size_t;

public:
    HashTable();
    ~HashTable() {};

    void Set(const key_type&, const data_type&, int ex = 0) override;
    std::optional<data_type> Get(const key_type&) override;
    bool Exists(const key_type&) override;
    bool Del(const key_type&) override;
    void Update(const key_type&, const data_type&) override;
    std::vector<key_type> Keys() override;
    bool Rename(const key_type& , const key_type&) override;
    std::optional<size_t> TTL(const key_type&) override;
    std::vector<key_type> Find(const data_type&) override;
    std::vector<data_type> ShowAll() override;
    size_t Upload(const std::string&) override;
    size_t Export(const std::string&) override;

private:
    size_t GetHashCode(const key_type&);
   
private:
    std::vector<std::pair<key_type, Record> > table;
};
}