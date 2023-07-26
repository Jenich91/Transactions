#pragma once
#include <ctime>
#include <iostream>
#include <string>

namespace sfleta {
struct Person {
  Person() {}
  Person(const std::string& last_name, const std::string& first_name,
         int year_of_birth, const std::string& city, int coins) {
    this->last_name = last_name;
    this->first_name = first_name;
    this->year_of_birth = year_of_birth;
    this->city = city;
    this->coins = coins;
  }
  ~Person(){};

  void Print() {
    std::cout << last_name << " " << first_name << " " << year_of_birth << " "
              << city << " " << coins << std::endl;
  }

  bool operator==(const Person& rhs) const {
    return last_name == rhs.last_name and first_name == rhs.first_name and
           year_of_birth == rhs.year_of_birth and city == rhs.city and
           coins == rhs.coins;
  }

  void RemoveSurroundingSymbols(char garbageSymbol) {
    TrimField(last_name, garbageSymbol);
    TrimField(first_name, garbageSymbol);
    TrimField(city, garbageSymbol);
  }

  void TrimField(std::string& str, char garbageSymbol) {
    if (str.size() >= 2 and str.front() == garbageSymbol) {
      str.erase(0, 1);
    }
    if (str.size() >= 2 and str.back() == garbageSymbol) {
      str.erase(str.size() - 1);
    }
  }

  std::string last_name;
  std::string first_name;
  int year_of_birth;
  std::string city;
  int coins;
};

struct Record {
  Record() {}
  explicit Record(const Person& person, int _life_time = 0)
      : values{person}, life_time{(size_t)_life_time}, is_alive{true} {}

  ~Record(){};

  int GetPassedTime() { return std::time(nullptr) - create_time; }

  int GetRemainingLifeTime() {
    return (life_time > 0) ? life_time - GetPassedTime() : 0;
  }

  bool IsALive() {
    return is_alive and
           (life_time == 0 || (life_time > 0 and GetRemainingLifeTime() > 0));
  }

  void Print() {
    values.Print();
    std::cout << "create_time - " << create_time << " "
              << "life_time - " << life_time << " "
              << "is_alive - " << is_alive << std::endl;
  }

  Person values;
  time_t create_time = std::time(nullptr);
  size_t life_time = 0;   // 0 - хранение бесконечность
  bool is_alive = false;  // по-умолчанию записили помечены как удаленные
};
}  // namespace sfleta
