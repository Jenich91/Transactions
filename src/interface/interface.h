#pragma once
#include <algorithm>
#include <climits>
#include <functional>
#include <locale>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

#include "../model/hashTable/hashTable.h"
#include "../model/tree/self_balancing_binary_search_tree.h"

namespace sfleta {
class Interface {
  enum class Command {
    SET,
    GET,
    EXISTS,
    DEL,
    UPDATE,
    KEYS,
    RENAME,
    TTL,
    FIND,
    SHOWALL,
    UPLOAD,
    EXPORT,
    HELP,
    RETURN,
    ERROR
  };

 public:
  ~Interface() {}
  Interface(const Interface &) = delete;
  Interface(const Interface &&) = delete;
  Interface &operator=(const Interface &) = delete;
  Interface &operator=(const Interface &&) = delete;

  static Interface &GetInstance() {
    static Interface instance;
    return instance;
  }

  void ShowMainMenu();

 private:
  Interface();

  void StorageStart();
  void ShowHelpMenu();
  void ShowChooseImplementationTypes();
  void ShowWaitingForInput();
  void ShowWrongInputAttention();
  bool IsValidCommand(std::string, std::string);
  Command GetCommandNum(const char *);
  std::vector<std::string> SplitBySpace(const std::string &);

  void TrySet(const std::vector<std::string> &);
  void TryGet(const std::vector<std::string> &);
  void TryExists(const std::vector<std::string> &);
  void TryDel(const std::vector<std::string> &);
  void TryUpdate(const std::vector<std::string> &);
  void TryKeys();
  void TryRename(const std::vector<std::string> &);
  void TryTtl(const std::vector<std::string> &);
  void TryFind(const std::vector<std::string> &);
  void TryShowall();
  void TryUpload(const std::vector<std::string> &);
  void TryExport(const std::vector<std::string> &);

  void SetCommandFuncDictionary();
  void SetCommandRegexDictionary();

 private:
  std::unique_ptr<sfleta::Storage> storage;
  std::map<std::string, std::regex> commandRegexDictionary;
  std::map<Command, std::function<void(const std::vector<std::string> &)> >
      commandFuncDictionary;
};
}  // namespace sfleta
