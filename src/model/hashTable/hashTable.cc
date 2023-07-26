#include "hashTable.h"

namespace sfleta {
HashTable::HashTable() {
    table.resize(1021);
}

unsigned long HashTable::GetHashCode(const key_type& key) {
    unsigned long i = 0;
    for (int j = 0; key[j]; j++)
        i += key[j];
    return i;
}

void HashTable::Set(const key_type& key, const data_type& values, int ex) {
    if(Exists(key)) return;

    Record newRecord (values, ex);

    hash_type hashIndex = GetHashCode(key);
    if(hashIndex >= table.capacity()) { table.resize(hashIndex*2); }

    while(hashIndex < table.capacity()-1 and table.at(hashIndex).second.IsALive()) {
        hashIndex++;
    }
   
    table.at(hashIndex) = {key, newRecord};
}

std::optional<Storage::data_type> HashTable::Get(const key_type& key) {
    hash_type hashIndex = GetHashCode(key);

    while(hashIndex < table.capacity()-1) {
        if(table.at(hashIndex).first==key and table.at(hashIndex).second.IsALive()) {
            return table.at(hashIndex).second.values;
        }
        hashIndex++;
    }

    return std::nullopt;
}

bool HashTable::Exists(const key_type& key) {
    hash_type hashIndex = GetHashCode(key);
    while(hashIndex < table.capacity()-1) {
        if(table.at(hashIndex).first==key 
            and table.at(hashIndex).second.IsALive()
        ) return true;
        hashIndex++;
    }
    return false;
};

bool HashTable::Del(const key_type& key) {
    hash_type hashIndex = GetHashCode(key);

    while(hashIndex < table.capacity()-1 and
          table.at(hashIndex).first!=key and 
          table.at(hashIndex).second.IsALive() ) {
        hashIndex++;
    }
    if(hashIndex+1 == table.capacity()) return false;

    table.at(hashIndex).second.is_alive = false;
    return true;
}

void HashTable::Update(const key_type& key, const data_type& values) {
    auto hash = GetHashCode(key);
    if(table.at(hash).second.IsALive() ) {
        if(values.last_name != "-")
            table.at(hash).second.values.last_name = values.last_name;
        if(values.first_name != "-")
            table.at(hash).second.values.first_name = values.last_name;
        if(values.year_of_birth != -1)
            table.at(hash).second.values.year_of_birth = values.year_of_birth;
        if(values.city != "-")
            table.at(hash).second.values.city = values.city;
        if(values.coins != -1)
            table.at(hash).second.values.coins = values.coins;
    }
}

HashTable::keys_arr HashTable::Keys() {
    keys_arr keys;
    for_each(table.begin(), table.end(), 
        [&keys](std::pair<key_type, Record>& row)  { 
            if(row.second.IsALive()) keys.push_back(row.first);
        } );
    return keys;
}

bool HashTable::Rename(const key_type& oldKey, const key_type& newKey) {
    auto oldHash = GetHashCode(oldKey);
    auto newHash = GetHashCode(newKey);

    if(!Exists(newKey) and 
        table.at(oldHash).second.IsALive() and
        !table.at(newHash).second.IsALive()) {

        table.at(oldHash).first = newKey;
        table.at(newHash) = std::move(table.at(oldHash));

        table.at(oldHash).first.clear();
        table.at(oldHash).second.is_alive = false;

        return true;
    }
    return false;
}

std::optional<size_t> HashTable::TTL(const key_type& key) {
    std::optional<size_t> tll;
    if(Exists(key))
        tll = table.at(GetHashCode(key)).second.GetRemainingLifeTime();
    return tll;
}

HashTable::keys_arr HashTable::Find(const data_type& values) {
    keys_arr matchKeys;

    for (auto& key : Keys()) {
        auto hash = GetHashCode(key);

        bool hasMatch = false;
        
        if( (values.last_name == "-" or table.at(hash).second.values.last_name == values.last_name)
        and (values.first_name == "-" or table.at(hash).second.values.first_name == values.first_name)
        and (values.year_of_birth == -1 or table.at(hash).second.values.year_of_birth == values.year_of_birth)
        and (values.city == "-" or table.at(hash).second.values.city == values.city)
        and (values.coins == -1 or table.at(hash).second.values.coins == values.coins) ) 
            hasMatch = true;

        if(hasMatch == true) matchKeys.push_back(key);
    }
    
    return matchKeys;
}

HashTable::values_arr HashTable::ShowAll() {
    values_arr v_arr;
    
    auto keys = Keys();
    std::sort(keys.begin(), keys.end(),[](const key_type& a, const key_type& b) -> bool {
            return (a.size() < b.size()) || (a.size() == b.size() && a < b);
        });

    std::for_each(keys.begin(), keys.end(), [&](const auto& key) { v_arr.push_back(table.at(GetHashCode(key)).second.values); });

    return v_arr;
}

size_t HashTable::Upload(const std::string& path) {
    size_t count = 0;
    std::ifstream input(path);
    if(input.fail()) throw std::invalid_argument("Ошибка открытия файла по пути - " + path);
    
    if (input.is_open()) {
        for(std::string line; getline(input, line);) {
            key_type key;
            Person values;

            std::stringstream iss(line);
            iss >> key;
            if(Exists(key)) continue;

            iss >> values.last_name;
            iss >> values.first_name;
            iss >> values.year_of_birth;
            iss >> values.city;
            iss >> values.coins;

            if(iss.fail()) throw std::invalid_argument("Ошибка считывания файла");

            values.RemoveSurroundingSymbols('"');
            
            Set(key, values);
            if(Exists(key)) count++;
        }
        input.close();
    }
    
    return count;
}

size_t HashTable::Export(const std::string& path) {
    size_t count = 0;
    std::ofstream output(path);
    if(output.fail()) throw std::invalid_argument("Ошибка открытия файла по пути - " + path);

    if (output.is_open()) {
        for (auto key : Keys()) {
            auto values = table.at(GetHashCode(key)).second.values;
            output << key << " ";
            output << "\"" << values.last_name << "\" ";
            output << "\"" << values.first_name << "\" ";
            output << values.year_of_birth << " ";
            output << "\"" << values.city << "\" ";
            output << values.coins << " "
            << "\n";

            count++;
        }
        output.close();
    }

    return count;
}

}
