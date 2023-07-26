#include <chrono>
#include <thread>
#include "gtest/gtest.h"
#include "model/hashTable/hashTable.h"
#include "model/tree/self_balancing_binary_search_tree.h"

using namespace sfleta;

TEST(hashTable_set_and_exist, test1) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};

    storage->Set(k1, {"Васильев", "Иван", 2000, "Москва", 55} );
    storage->Set(k2, {"Сидоров", "Сергей", 1847, "Суздаль", 123} );

    ASSERT_TRUE(storage->Exists(k1));
    ASSERT_TRUE(storage->Exists(k2));
    ASSERT_FALSE(storage->Exists("key3"));
}

TEST(hashTable_set_and_exist, test2) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};

    storage->Set(k1, {"Васильев", "Иван", 2000, "Москва", 55} );
    storage->Set(k1, {"Сидоров", "Сергей", 1847, "Суздаль", 123} );

    ASSERT_TRUE(storage->Exists(k1));
    ASSERT_FALSE(storage->Exists(k2));
    ASSERT_FALSE(storage->Exists("key3"));
}

TEST(hashTable_get, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::data_type values_ref1 {"Васильев", "Иван", 2000, "Москва", 55};
    storage->Set(k1, values_ref1);
    ASSERT_TRUE(values_ref1 == storage->Get(k1).value());

    Storage::key_type k2 = {"key2"};
    Storage::data_type values_ref2 {"Сидоров", "Сергей", 1847, "Суздаль", 123};
    storage->Set(k2, values_ref2);
    ASSERT_TRUE(values_ref2 == storage->Get(k2).value());
}

TEST(hashTable_ttl, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::data_type values_ref1 {"Васильев", "Иван", 2000, "Москва", 55};
    storage->Set(k1, values_ref1, 10);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ASSERT_TRUE(storage->Exists(k1));

    Storage::key_type k2 = {"key2"};
    Storage::data_type values_ref2 {"Сидоров", "Сергей", 1847, "Суздаль", 123};
    storage->Set(k2, values_ref2, 2);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ASSERT_FALSE(storage->Exists(k2));
}

TEST(hashTable_ttl2, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::data_type values_ref1 = {"Васильев", "Иван", 2000, "Москва", 105};
    storage->Set(k1, values_ref1, 15);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    ASSERT_TRUE(storage->Exists(k1));
    ASSERT_TRUE(storage->TTL(k1).value() == 10);
}

TEST(hashTable_del, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    storage->Set(k1, {"Васильев", "Иван", 2000, "Москва", 55});
    
    ASSERT_TRUE(storage->Del(k1));
    ASSERT_FALSE(storage->Exists(k1));
}

TEST(hashTable_update, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    storage->Set(k1, {"Васильев", "Иван", 2000, "Москва", 55});
    storage->Update(k1, Storage::data_type("-", "-", -1, "-", 105) );

    Storage::data_type values_ref1 {"Васильев", "Иван", 2000, "Москва", 105};
    ASSERT_TRUE(values_ref1 == storage->Get(k1).value());
}

TEST(hashTable_keys, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};
    storage->Set(k1, {"Васильев", "Иван", 2000, "Москва", 55});
    storage->Set(k2, {"Сидоров", "Сергей", 1847, "Суздаль", 123});

    auto keys = storage->Keys();
    ASSERT_TRUE(keys.at(0) == k1);
    ASSERT_TRUE(keys.at(1) == k2);
}

TEST(hashTable_rename, test1) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};
    Storage::data_type values_ref1 = {"Васильев", "Иван", 2000, "Москва", 105};
    storage->Set(k1, values_ref1);

    ASSERT_TRUE(storage->Rename(k1, k2));
    ASSERT_FALSE(storage->Exists(k1));
    ASSERT_TRUE(storage->Exists(k2));
    ASSERT_TRUE(values_ref1 == storage->Get(k2).value());
}

TEST(hashTable_rename, test2) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};
    Storage::data_type values_ref1 = {"Васильев", "Иван", 2000, "Москва", 105};
    storage->Set(k1, values_ref1);
    storage->Set(k2, {"Сидоров", "Сергей", 1847, "Суздаль", 123});

    ASSERT_FALSE(storage->Rename(k1, k2));
}

TEST(hashTable_find, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();
    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};
    Storage::data_type values_ref1 = {"Васильев", "Иван", 2000, "Москва", 55};
    Storage::data_type values_ref2 = {"Васильев", "Антон", 1997, "Тверь", 55};
    storage->Set(k1, values_ref1);
    storage->Set(k2, values_ref2);

    HashTable::keys_arr k_arr{ k1, k2 };

    ASSERT_TRUE(k_arr == storage->Find(Storage::data_type("Васильев", "-", -1, "-", 55)) );
}

TEST(hashTable_showall, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();

    Storage::data_type values_ref1 = {"Васильев", "Иван", 2000, "Москва", 55};
    Storage::data_type values_ref2 = {"Васильев", "Антон", 1997, "Тверь", 55};
    Storage::data_type values_ref3 = {"Сидоров", "Сергей", 1847, "Суздаль", 12312313};

    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};
    Storage::key_type k3 = {"key3"};

    storage->Set(k1, values_ref1);
    storage->Set(k2, values_ref2);
    storage->Set(k3, values_ref3);

    HashTable::values_arr values_ref {values_ref1, values_ref2, values_ref3};

    ASSERT_TRUE(values_ref == storage->ShowAll());
}

TEST(hashTable_upload, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();

    Storage::data_type values_ref1 = {"Васильев", "Иван", 2000, "Москва", 55};
    Storage::data_type values_ref2 = {"Васильев", "Антон", 1997, "Тверь", 55};
    Storage::data_type values_ref3 = {"Сидоров", "Сергей", 1847, "Суздаль", 12312313};

    HashTable::values_arr values_refs {values_ref1, values_ref2, values_ref3};

    ASSERT_TRUE(values_refs.size() == storage->Upload("../data-samples/upload.txt"));
    ASSERT_TRUE(values_refs == storage->ShowAll());
}

TEST(hashTable_export, test) {
    std::unique_ptr<sfleta::Storage> storage = std::make_unique<sfleta::HashTable>();

    Storage::data_type values_ref1 = {"Васильев", "Иван", 2000, "Москва", 55};
    Storage::data_type values_ref2 = {"Васильев", "Антон", 1997, "Тверь", 55};
    Storage::data_type values_ref3 = {"Сидоров", "Сергей", 1847, "Суздаль", 12312313};

    Storage::key_type k1 = {"key1"};
    Storage::key_type k2 = {"key2"};
    Storage::key_type k3 = {"key3"};

    storage->Set(k1, values_ref1);
    storage->Set(k2, values_ref2);
    storage->Set(k3, values_ref3);

    HashTable::values_arr values_refs {values_ref1, values_ref2, values_ref3};

    ASSERT_TRUE(values_refs.size() == storage->Export("../data-samples/export.txt"));
    storage->Del(k1);
    storage->Del(k2);
    storage->Del(k3);

    ASSERT_TRUE(values_refs.size() == storage->Upload("../data-samples/export.txt"));
    ASSERT_TRUE(values_refs == storage->ShowAll());
}

TEST(tree_Set, test_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    ASSERT_TRUE(store.Exists(key));
}

TEST(tree_Set, test_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Storage::key_type key = "ivan";
    ASSERT_FALSE(store.Exists(key));
}

TEST(tree, Set_3) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("Леонидов","Леонид",1981,"Владивосток",500);
    store.Set(key,p2);
    ASSERT_TRUE(store.Exists(key));
    std::vector<sfleta::Storage::key_type> arr = store.Find(p);
    ASSERT_FALSE(arr.empty()); 
    ASSERT_TRUE(arr.front() == "ivan");
}

TEST(tree, Get_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Storage::key_type key = "ivan";
    ASSERT_FALSE(store.Get(key).has_value());
}

TEST(tree, Get_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    std::optional<sfleta::Storage::data_type> person = store.Get(key);
    ASSERT_TRUE((*person).last_name == "Иванов");
    ASSERT_TRUE((*person).first_name == "Иван");
    ASSERT_TRUE((*person).year_of_birth == 1991);
    ASSERT_TRUE((*person).city == "Якутск");
    ASSERT_TRUE((*person).coins == 1000);
}

TEST(tree, Get_3) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("Леонидов","Леонид",1981,"Владивосток",500);
    store.Set(key,p2);

    std::optional<sfleta::Storage::data_type> person = store.Get(key);
    ASSERT_TRUE((*person).last_name == "Иванов");
    ASSERT_TRUE((*person).first_name == "Иван");
    ASSERT_TRUE((*person).year_of_birth == 1991);
    ASSERT_TRUE((*person).city == "Якутск");
    ASSERT_TRUE((*person).coins == 1000);
}

TEST(tree, Get_4) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    sfleta::Storage::key_type no_key = "vlad";
    ASSERT_FALSE(store.Get(no_key).has_value());
}

TEST(tree, Exists_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    ASSERT_TRUE(store.Exists(key));
}

TEST(tree, Exists_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key1 = "ivan";
    sfleta::Storage::key_type key2 = "mila";
    store.Set(key1,p);
    ASSERT_FALSE(store.Exists(key2));
}

TEST(tree, Del_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    ASSERT_TRUE(store.Del(key));
}

TEST(tree, Del_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key1 = "ivan";
    sfleta::Storage::key_type key2 = "mila";
    store.Set(key1,p);
    ASSERT_FALSE(store.Del(key2));
}

TEST(tree, Update_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("Иванов","Иван",1991,"Владивосток",1000);
    store.Update(key,p2);

    std::optional<sfleta::Storage::data_type> person = store.Get(key);
    ASSERT_TRUE((*person).last_name == "Иванов");
    ASSERT_TRUE((*person).first_name == "Иван");
    ASSERT_TRUE((*person).year_of_birth == 1991);
    ASSERT_TRUE((*person).city == "Владивосток");
    ASSERT_TRUE((*person).coins == 1000);
}

TEST(tree, Update_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("Леонидов","Леонид",1981,"Владивосток",500);
    store.Update(key,p2);

    std::optional<sfleta::Storage::data_type> person = store.Get(key);
    ASSERT_TRUE((*person).last_name == "Леонидов");
    ASSERT_TRUE((*person).first_name == "Леонид");
    ASSERT_TRUE((*person).year_of_birth == 1981);
    ASSERT_TRUE((*person).city == "Владивосток");
    ASSERT_TRUE((*person).coins == 500);
}

TEST(tree, Update_3) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("-","Леонид",1981,"-",500);
    store.Update(key,p2);

    std::optional<sfleta::Storage::data_type> person = store.Get(key);
    ASSERT_TRUE((*person).last_name == "Иванов");
    ASSERT_TRUE((*person).first_name == "Леонид");
    ASSERT_TRUE((*person).year_of_birth == 1981);
    ASSERT_TRUE((*person).city == "Якутск");
    ASSERT_TRUE((*person).coins == 500);
}

TEST(tree, Update_4) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("-","Леонид",-1,"Владивосток",-1);
    store.Update(key,p2);

    std::optional<sfleta::Storage::data_type> person = store.Get(key);
    ASSERT_TRUE((*person).last_name == "Иванов");
    ASSERT_TRUE((*person).first_name == "Леонид");
    ASSERT_TRUE((*person).year_of_birth == 1991);
    ASSERT_TRUE((*person).city == "Владивосток");
    ASSERT_TRUE((*person).coins == 1000);
}

TEST(tree, Key_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    std::vector<sfleta::Storage::key_type> arr = store.Keys();
    ASSERT_TRUE(arr.empty());   
}

TEST(tree, Key_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    std::vector<sfleta::Storage::key_type> arr = store.Keys();
    ASSERT_TRUE(arr.front() == "ivan");
}

TEST(tree, Key_3) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("Леонидов","Леонид",1981,"Владивосток",500);
    sfleta::Storage::key_type key2 = "leonid";
    store.Set(key2,p2);
    
    ASSERT_TRUE(store.Del(key2));

    std::vector<sfleta::Storage::key_type> arr = store.Keys();
    ASSERT_TRUE(arr.front() == "ivan");
}

TEST(tree, Key_4) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);

    sfleta::Person p2("Леонидов","Леонид",1981,"Владивосток",500);
    sfleta::Storage::key_type key2 = "leonid";
    store.Set(key2,p2);

    sfleta::Person p3("Петров","Петр",1985,"Нижний Новгород",750);
    sfleta::Storage::key_type key3 = "petr";
    store.Set(key3,p3);


    std::vector<sfleta::Storage::key_type> arr = store.Keys();
    ASSERT_TRUE(arr.at(0) == "ivan");
    ASSERT_TRUE(arr.at(1) == "leonid");
    ASSERT_TRUE(arr.at(2) == "petr");
}

TEST(tree, Rename_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key1 = "ivan";
    sfleta::Storage::key_type key2 = "leroy";

    store.Set(key1,p);
    ASSERT_TRUE(store.Rename(key1,key2));
}

TEST(tree, Rename_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Storage::key_type key1 = "ivan";
    sfleta::Storage::key_type key2 = "leroy";
    
    ASSERT_FALSE(store.Rename(key1,key2));
}

TEST(tree, TTL_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    ASSERT_FALSE(store.TTL(key).has_value());
}

TEST(tree, TTL_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p,10);
    std::chrono::seconds dura(5);
    std::this_thread::sleep_for(dura);
    ASSERT_EQ((*store.TTL(key)),5);
}

TEST(tree, TTL_3) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p,10);
    std::chrono::seconds dura(11);
    std::this_thread::sleep_for(dura);
    ASSERT_FALSE(store.TTL(key).has_value());
}

TEST(tree, Find_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    std::vector<sfleta::Storage::key_type> arr = store.Find(p);
    ASSERT_TRUE(arr.empty()); 
}

TEST(tree, Find_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    std::vector<sfleta::Storage::key_type> arr = store.Find(p);
    ASSERT_FALSE(arr.empty()); 
    ASSERT_TRUE(arr.front() == "ivan"); 
}

TEST(tree, Find_3) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p,10);
    sfleta::Person p2("Иванов","Леонид",1981,"Владивосток",500);
    sfleta::Storage::key_type key2 = "leond";
    store.Set(key2,p2);
    sfleta::Person sample("Иванов","-",-1,"-",-1);
    std::vector<sfleta::Storage::key_type> arr = store.Find(sample);
    ASSERT_FALSE(arr.empty());
    ASSERT_TRUE(arr.front() == "ivan"); 
    ASSERT_TRUE(arr.back() == "leond"); 
}

TEST(tree, Find_4) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",500);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    sfleta::Person p2("Иванов","Леонид",1981,"Владивосток",500);
    sfleta::Storage::key_type key2 = "leond";
    store.Set(key2,p2);
    sfleta::Person sample("-","-",-1,"-",500);
    std::vector<sfleta::Storage::key_type> arr = store.Find(sample);
    ASSERT_FALSE(arr.empty());
    ASSERT_TRUE(arr.front() == "ivan"); 
    ASSERT_TRUE(arr.back() == "leond"); 
}

TEST(tree, Find_5) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    sfleta::Person p2("Иванов","Леонид",1981,"Якутск",500);
    sfleta::Storage::key_type key2 = "leond";
    store.Set(key2,p2);
    sfleta::Person sample("Иванов","-",-1,"Якутск",500);
    std::vector<sfleta::Storage::key_type> arr = store.Find(sample);
    ASSERT_FALSE(arr.empty());
    ASSERT_TRUE(arr.front() == "leond"); 
}

TEST(tree, Find_6) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    sfleta::Person p2("Иванов","Леонид",1981,"Якутск",500);
    sfleta::Storage::key_type key2 = "leond";
    store.Set(key2,p2);
    sfleta::Person sample("Иванов","-",-1,"Якутск",-1);
    std::vector<sfleta::Storage::key_type> arr = store.Find(sample);
    ASSERT_FALSE(arr.empty());
    ASSERT_TRUE(arr.front() == "ivan"); 
    ASSERT_TRUE(arr.back() == "leond"); 
}

TEST(tree, Find_7) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    sfleta::Person p2("Иванов","Леонид",1981,"Якутск",500);
    sfleta::Storage::key_type key2 = "leond";
    store.Set(key2,p2);
    sfleta::Person sample("Иванов","-",1991,"-",-1);
    std::vector<sfleta::Storage::key_type> arr = store.Find(sample);
    ASSERT_FALSE(arr.empty());
    ASSERT_TRUE(arr.front() == "ivan"); 
}

TEST(tree, ShowAll_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    std::vector<sfleta::Storage::data_type> arr = store.ShowAll();
    ASSERT_TRUE(arr.empty());
}

TEST(tree, ShowAll_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    sfleta::Person p2("Иванов","Леонид",1981,"Владивосток",500);
    sfleta::Storage::key_type key2 = "leond";
    store.Set(key2,p2);
    std::vector<sfleta::Storage::data_type> arr = store.ShowAll();
    ASSERT_FALSE(arr.empty());
    ASSERT_TRUE(arr.front().last_name == "Иванов");
    ASSERT_TRUE(arr.front().first_name == "Иван");
    ASSERT_TRUE(arr.front().year_of_birth == 1991);
    ASSERT_TRUE(arr.front().city == "Якутск");
    ASSERT_TRUE(arr.front().coins == 1000);
    ASSERT_TRUE(arr.back().last_name == "Иванов");
    ASSERT_TRUE(arr.back().first_name == "Леонид");
    ASSERT_TRUE(arr.back().year_of_birth == 1981);
    ASSERT_TRUE(arr.back().city == "Владивосток");
    ASSERT_TRUE(arr.back().coins == 500);
}

TEST(tree, ShowAll_3) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p("Иванов","Иван",1991,"Якутск",1000);
    sfleta::Storage::key_type key = "ivan";
    store.Set(key,p);
    sfleta::Person p2("Иванов","Леонид",1981,"Владивосток",500);
    sfleta::Storage::key_type key2 = "leond";
    store.Set(key2,p2,5);
    std::chrono::seconds dura(5);
    std::this_thread::sleep_for(dura);
    std::vector<sfleta::Storage::data_type> arr = store.ShowAll();
    ASSERT_FALSE(arr.empty());
    ASSERT_TRUE(arr.front().last_name == "Иванов");
    ASSERT_TRUE(arr.front().first_name == "Иван");
    ASSERT_TRUE(arr.front().year_of_birth == 1991);
    ASSERT_TRUE(arr.front().city == "Якутск");
    ASSERT_TRUE(arr.front().coins == 1000);
}

TEST(tree, Upload_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    ASSERT_TRUE(0 == store.Upload("../data-samples/upload_empty.txt"));
}

TEST(tree, Upload_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p1("Васильев", "Иван", 2000, "Москва", 55);
    sfleta::Person p2("Васильев", "Антон", 1997, "Тверь", 55);
    sfleta::Person p3("Сидоров", "Сергей", 1847, "Суздаль", 12312313);
    std::vector<sfleta::Storage::data_type> arr;
    arr.emplace_back(p1);
    arr.emplace_back(p2);
    arr.emplace_back(p3);
    ASSERT_TRUE(arr.size() == store.Upload("../data-samples/upload.txt"));
}

TEST(tree, Export_1) {
    sfleta::SelfBalancingBinarySearchTree store;
    ASSERT_TRUE(0 == store.Export("../data-samples/export.txt"));
}

TEST(tree, Export_2) {
    sfleta::SelfBalancingBinarySearchTree store;
    sfleta::Person p1("Васильев", "Иван", 2000, "Москва", 55);
    sfleta::Storage::key_type key1 = "ivan";
    sfleta::Person p2("Васильев", "Антон", 1997, "Тверь", 55);
    sfleta::Storage::key_type key2 = "anton";
    sfleta::Person p3("Сидоров", "Сергей", 1847, "Суздаль", 12312313);
    sfleta::Storage::key_type key3 = "sergey";
    store.Set(key1,p1);
    store.Set(key2,p2);
    store.Set(key3,p3);
    ASSERT_TRUE(3 == store.Export("../data-samples/export.txt"));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
