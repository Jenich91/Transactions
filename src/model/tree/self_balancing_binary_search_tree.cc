#include "self_balancing_binary_search_tree.h"

namespace sfleta {

    void SelfBalancingBinarySearchTree::Set(const key_type &key, const data_type &person, int ex)
    {
        CheckRecord();
        Record record(person,ex);
        auto pair = map_.Insert(key,record);
        if(pair.second) { ++count_; }
    }

    std::optional<sfleta::Storage::data_type> SelfBalancingBinarySearchTree::Get(const key_type &key)
    {
        std::optional<sfleta::Storage::data_type> person = std::nullopt;
        CheckRecord();
        if (count_ > 0)
        {
            if(map_.Contains(key)){
                auto node = map_.Find(key).node_;
                sfleta::Record record = node->data_->second;
                person = record.values;    
            }
        }
        return person;
    }

    bool SelfBalancingBinarySearchTree::Exists(const key_type &key)
    {
        bool res = false;
        CheckRecord();
        if(count_ > 0) {
            res = map_.Contains(key);
        }
        return res;
    }

    bool SelfBalancingBinarySearchTree::Del(const key_type &key)
    {
        bool res = false;
        CheckRecord();
        if(count_ > 0) {
            if(map_.Contains(key)) {
                map_.Erase(map_.Find(key));
                res = true;
                --count_;
            }
        }
        return res;
    }

    void SelfBalancingBinarySearchTree::Update(const key_type &key, const data_type &person_update)
    {
        CheckRecord();
        if(count_ > 0) {
            if(map_.Contains(key)){
                if (IsUnchangedData(person_update)) {
                    Record& record = map_.Find(key).node_->data_->second;
                    if(person_update.last_name != "-"){
                        record.values.last_name = person_update.last_name;
                    }
                    if(person_update.first_name != "-"){
                        record.values.first_name = person_update.first_name;
                    }
                    if(person_update.year_of_birth != -1) {
                        record.values.year_of_birth = person_update.year_of_birth;
                    }
                    if (person_update.city != "-"){
                        record.values.city = person_update.city;
                    }
                    if (person_update.coins != -1) {
                        record.values.coins = person_update.coins;
                    }
                } else {
                    auto node = map_.Find(key).node_;
                    node->data_->second.values = person_update;
                }
            }
        }
    }

    std::vector<sfleta::Storage::key_type> SelfBalancingBinarySearchTree::Keys()
    {
        std::vector<key_type> res;
        CheckRecord();
        if(count_ > 0) {
            for (sfleta::Map<key_type, record_type>::Mapiterator it = map_.Begin(); it != map_.End(); ++it) {
                res.emplace_back((*it).first);
            }
        }
        return res;
    }

    bool SelfBalancingBinarySearchTree::Rename(const key_type &key1, const key_type &key2)
    {
        bool res = false;
        CheckRecord();
        if(count_ > 0) {
            if(map_.Contains(key1)) {
                Record record = map_.Find(key1).node_->data_->second;
                map_.Erase(map_.Find(key1));
                auto pair = map_.Insert(key2,record);
                res = pair.second;
            }
        }
        return res;
    }

    std::optional<size_t> SelfBalancingBinarySearchTree::TTL(const key_type &key)
    {
        std::optional<size_t> res = std::nullopt;
        CheckRecord();
        if(count_ > 0) {
            if(map_.Contains(key)) {
                Record& record = map_.Find(key).node_->data_->second;
                if(record.life_time > 0) {
                    res = static_cast<size_t>(record.life_time - (std::time(nullptr) - record.create_time));
                } else {
                    res = record.life_time;
                }
            }
        }
        return res;
    }

    std::vector<sfleta::Storage::key_type> SelfBalancingBinarySearchTree::Find(const data_type &person)
    {
        CheckRecord();
        std::vector<key_type> res;
        if(count_ > 0) {
            for (auto it = map_.Begin(); it != map_.End(); ++it) {
                record_type record = (*it).second;
                data_type tmp = record.values;
                bool isEqual = true;

                if(person.last_name != "-"){
                    if(tmp.last_name != person.last_name) isEqual = false;
                }
                if(person.first_name != "-"){
                    if(tmp.first_name != person.first_name) isEqual = false;
                }
                if(person.year_of_birth != -1) {
                    if(tmp.year_of_birth != person.year_of_birth) isEqual = false;
                }
                if (person.city != "-"){
                     if(tmp.city != person.city) isEqual = false;
                }
                if (person.coins != -1) {
                     if(tmp.coins != person.coins) isEqual = false;
                }

                if(isEqual){
                    key_type key = (*it).first; 
                    res.emplace_back(key);
                }
            }
        }
        return res;
    }

    std::vector<sfleta::Storage::data_type> SelfBalancingBinarySearchTree::ShowAll()
    {
        CheckRecord();
        std::vector<data_type> res;
        if(count_ > 0) {
            for (auto it = map_.Begin(); it != map_.End(); ++it) {
                record_type record = (*it).second;
                data_type person = record.values;
                res.emplace_back(person);
            }
        }
        return res;
    }

    size_t SelfBalancingBinarySearchTree::Upload(const std::string &path)
    {
        size_t count = 0;
        std::ifstream input(path);
        if(input.fail()) throw std::invalid_argument("Ошибка открытия файла по пути - " + path);
        
        for(std::string line; getline(input, line);) {
            key_type key;
            data_type person;

            std::stringstream iss(line);
            iss >> key;
            if(Exists(key)) continue;

            iss >> person.last_name;
            iss >> person.first_name;
            iss >> person.year_of_birth;
            iss >> person.city;
            iss >> person.coins;

            if(iss.fail()) throw std::invalid_argument("Ошибка считывания файла");

            person.RemoveSurroundingSymbols('"');
            
            Set(key, person);
            if(Exists(key)) count++;
        }
        return count;
    }

    size_t SelfBalancingBinarySearchTree::Export(const std::string &path)
    {
        size_t count = 0;
        std::ofstream output(path);
        if(output.fail()) throw std::invalid_argument("Ошибка открытия файла по пути - " + path);

        if (output.is_open()) {
            if(count_ > 0) {
                for (auto it = map_.Begin(); it != map_.End(); ++it) {
                    record_type record = (*it).second;
                    data_type person = record.values;
                    key_type key = (*it).first;

                    output << key << " ";
                    output << "\"" << person.last_name << "\"" << " ";
                    output << "\"" << person.first_name << "\"" << " ";
                    output << person.year_of_birth << " ";
                    output << "\"" << person.city << "\"" << " ";
                    output << person.coins << "\n";

                    ++count;
                }
            }
            output.close();
        }
        return count;
    }

    /*---------private----------*/

    bool SelfBalancingBinarySearchTree::IsUnchangedData(const data_type &person)
    {
        bool res = false;
        if( person.last_name == "-" ||
            person.first_name == "-" ||
            person.year_of_birth == -1 ||
            person.city == "-" ||
            person.coins == -1) { res = true; }
        return res;
    }

    bool SelfBalancingBinarySearchTree::IsDeleteRecord(const record_type &record)
    {
        bool res = false;
        if(record.life_time > 0) {
            long time = static_cast<long>(record.life_time - (std::time(nullptr) - record.create_time));
            if (time <= 0) { res = true; }
        }
        return res;
    }

    void SelfBalancingBinarySearchTree::CheckRecord()
    {
        if(count_ > 0) {
            std::vector<key_type> keys;
            for (auto it = map_.Begin(); it != map_.End(); ++it) {
                record_type record = it.node_->data_->second;
                if (IsDeleteRecord(record)) {
                    key_type key = it.node_->data_->first;
                    keys.emplace_back(key);
                }
            }

            for(key_type key : keys){
                sfleta::Tree<key_type, record_type>::Iterator it = map_.Find(key);
                map_.Erase(it);
                --count_;
            }
        }
    }

} // namespace sfleta
