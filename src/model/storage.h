#pragma once
#include <optional>
#include <vector>

#include "person.h"

namespace sfleta {
class Storage {
 public:
  using key_type = std::string;
  using data_type = Person;

 public:
  Storage(){};
  virtual ~Storage(){};

  virtual void Set(const key_type&, const data_type&, int ex = 0) = 0;
  virtual std::optional<data_type> Get(const key_type&) = 0;
  virtual bool Exists(const key_type&) = 0;
  virtual bool Del(const key_type&) = 0;
  virtual void Update(const key_type&, const data_type&) = 0;
  virtual std::vector<key_type> Keys() = 0;
  virtual bool Rename(const key_type&, const key_type&) = 0;
  virtual std::optional<size_t> TTL(const key_type&) = 0;
  virtual std::vector<key_type> Find(const data_type&) = 0;
  virtual std::vector<data_type> ShowAll() = 0;
  virtual size_t Upload(const std::string&) = 0;
  virtual size_t Export(const std::string&) = 0;
};
}  // namespace sfleta