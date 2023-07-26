#pragma once

#include "../storage.h"
#include "sfleta_map.h"

#include <iostream>
#include <sstream>
 
namespace sfleta {

    class SelfBalancingBinarySearchTree : public Storage
    {
        public:
            using record_type = sfleta::Record;

            SelfBalancingBinarySearchTree() = default;
            ~SelfBalancingBinarySearchTree() = default;
            SelfBalancingBinarySearchTree(const SelfBalancingBinarySearchTree &other) = delete;
            SelfBalancingBinarySearchTree(SelfBalancingBinarySearchTree &&other) = delete;
            SelfBalancingBinarySearchTree &operator=(const SelfBalancingBinarySearchTree &other) = delete;
            SelfBalancingBinarySearchTree &operator=(SelfBalancingBinarySearchTree &&other) = delete;


            void Set(const key_type&,const data_type&, int ex = 0) override;
            std::optional<data_type> Get(const key_type&) override;
            bool Exists(const key_type&) override;
            bool Del(const key_type &) override;
            void Update(const key_type &, const data_type &) override;
            std::vector<key_type> Keys() override;
            bool Rename(const key_type & , const key_type &) override;
            std::optional<size_t> TTL(const key_type &) override;
            std::vector<key_type> Find(const data_type &) override;
            std::vector<data_type> ShowAll() override;
            size_t Upload(const std::string&) override;
            size_t Export(const std::string&) override;


        private:
            Map<std::string, Record> map_;
            size_t count_ = 0;

            bool IsUnchangedData(const data_type &);
            bool IsDeleteRecord(const record_type &);
            void CheckRecord();
            
    };
    
} // namespace sfleta
