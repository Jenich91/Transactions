#include "interface.h"

namespace sfleta {
Interface::Interface() {
  SetCommandFuncDictionary();
  SetCommandRegexDictionary();
}

void Interface::SetCommandFuncDictionary() {
  commandFuncDictionary[Command::SET] =
      std::bind(&Interface::TrySet, this, std::placeholders::_1);
  commandFuncDictionary[Command::GET] =
      std::bind(&Interface::TryGet, this, std::placeholders::_1);
  commandFuncDictionary[Command::EXISTS] =
      std::bind(&Interface::TryExists, this, std::placeholders::_1);
  commandFuncDictionary[Command::DEL] =
      std::bind(&Interface::TryDel, this, std::placeholders::_1);
  commandFuncDictionary[Command::UPDATE] =
      std::bind(&Interface::TryUpdate, this, std::placeholders::_1);
  commandFuncDictionary[Command::KEYS] = std::bind(&Interface::TryKeys, this);
  commandFuncDictionary[Command::RENAME] =
      std::bind(&Interface::TryRename, this, std::placeholders::_1);
  commandFuncDictionary[Command::TTL] =
      std::bind(&Interface::TryTtl, this, std::placeholders::_1);
  commandFuncDictionary[Command::FIND] =
      std::bind(&Interface::TryFind, this, std::placeholders::_1);
  commandFuncDictionary[Command::SHOWALL] =
      std::bind(&Interface::TryShowall, this);
  commandFuncDictionary[Command::UPLOAD] =
      std::bind(&Interface::TryUpload, this, std::placeholders::_1);
  commandFuncDictionary[Command::EXPORT] =
      std::bind(&Interface::TryExport, this, std::placeholders::_1);
  commandFuncDictionary[Command::HELP] =
      std::bind(&Interface::ShowHelpMenu, this);
}

void Interface::SetCommandRegexDictionary() {
  std::string key = (R"(\s\w+)");
  std::string values = (R"(\s[\S]+\s[\S]+\s\d+\s[\S]+\s\d+)");
  std::string valuesMaybeDash =
      (R"(\s[\S|-]+\s[\S|-]+\s[\S|-]+\s[\S|-]+\s[\S|-]+)");
  std::string exMaybe = (R"((\sEX\s\d+)?)");
  std::string end = (R"(\s*?$)");

  commandRegexDictionary["SET"] =
      std::regex(R"(^SET)" + key + values + exMaybe + end, std::regex::icase);
  commandRegexDictionary["GET"] =
      std::regex(R"(^GET)" + key + end, std::regex::icase);
  commandRegexDictionary["EXISTS"] =
      std::regex(R"(^EXISTS)" + key + end, std::regex::icase);
  commandRegexDictionary["DEL"] =
      std::regex(R"(^DEL)" + key + end, std::regex::icase);
  commandRegexDictionary["UPDATE"] =
      std::regex(R"(^UPDATE)" + key + valuesMaybeDash + end, std::regex::icase);
  commandRegexDictionary["KEYS"] =
      std::regex(R"(^KEYS)" + end, std::regex::icase);
  commandRegexDictionary["RENAME"] =
      std::regex(R"(^RENAME)" + key + key + end, std::regex::icase);
  commandRegexDictionary["TTL"] =
      std::regex(R"(^TTL)" + key + end, std::regex::icase);
  commandRegexDictionary["FIND"] =
      std::regex(R"(^FIND)" + valuesMaybeDash + end, std::regex::icase);
  commandRegexDictionary["SHOWALL"] =
      std::regex(R"(^SHOWALL)" + end, std::regex::icase);
  commandRegexDictionary["UPLOAD"] =
      std::regex(R"(^UPLOAD\s.*$)", std::regex::icase);
  commandRegexDictionary["EXPORT"] =
      std::regex(R"(^EXPORT\s.*)", std::regex::icase);
  commandRegexDictionary["HELP"] =
      std::regex(R"(^HELP)" + end, std::regex::icase);
  commandRegexDictionary["RETURN"] =
      std::regex(R"(^RETURN)" + end, std::regex::icase);
}

bool Interface::IsValidCommand(std::string fullyCommand,
                               std::string commandName) {
  std::transform(commandName.begin(), commandName.end(), commandName.begin(),
                 ::toupper);
  return regex_match(fullyCommand, commandRegexDictionary[commandName]);
}

void Interface::ShowMainMenu() {
  while (true) {
    ShowChooseImplementationTypes();

    int input = -1;
    std::cin >> input;

    if (std::cin.fail()) {
      ShowWrongInputAttention();
      continue;
    }

    switch (input) {
      case 1:
        storage = std::make_unique<sfleta::HashTable>();
        StorageStart();
        break;
      case 2:
        storage = std::make_unique<sfleta::SelfBalancingBinarySearchTree>();
        StorageStart();
        break;
      case 0:
        std::cout << "bye-bye";
        return;
      default:
        ShowWrongInputAttention();
    }
  }
}

void Interface::ShowChooseImplementationTypes() {
  system("clear");
  std::cout << "~~Main menu~~\n"
            << "Выберите тип хранилища:\n"
            << "\t1 - Хеш-таблица\n"
            << "\t2 - Самобалансирующееся бинарное дерево поиска\n"
            << "\t0 - Выход\n";
}

void Interface::ShowWrongInputAttention() {
  std::cout << "Некорректный ввод\n";
  std::cin.clear();
  ShowWaitingForInput();
}

void Interface::ShowWaitingForInput() {
  std::cout << "Для продолжения нажмите Enter\n";
  std::cin.sync();
  std::cin.ignore(INT_MAX, '\n');
  system("clear");
}

void Interface::StorageStart() {
  while (true) {
    ShowWaitingForInput();
    std::cout
        << "~~Storage~~\n"
        << "Введите команду:\n"
        << "\tЗапрос к хранилищу - <Команда запроса> <Аргумент1> "
           "<Аргумент2>...\n"
        << "\tHELP - для вывода справочной информации о командах запросов\n"
        << "\tRETURN - для возврата в главное меню\n";

    std::string fullyCommand = "";
    std::getline(std::cin, fullyCommand);
    if (std::cin.fail() or fullyCommand.empty()) {
      ShowWrongInputAttention();
      continue;
    }

    std::vector<std::string> commandArgs = SplitBySpace(fullyCommand);
    std::string commandName = commandArgs.front();

    if (!IsValidCommand(fullyCommand, commandName)) {
      std::cout << "Введенна невалидная команда!\n" << std::endl;
      continue;
    }

    Command commandNum = GetCommandNum(commandName.c_str());
    switch (commandNum) {
      case Command::SET... Command::HELP:
        commandFuncDictionary[commandNum](commandArgs);
        break;
      case Command::RETURN:
        return;
      default:
        ShowWrongInputAttention();
        continue;
    }
  }
}

std::vector<std::string> Interface::SplitBySpace(const std::string& str) {
  std::string tmp;
  std::stringstream ss(str);
  std::vector<std::string> v;
  while (getline(ss, tmp, ' ')) v.push_back(tmp);
  return v;
}

Interface::Command Interface::GetCommandNum(const char* commandName) {
  if (strcasecmp(commandName, "SET") == 0) return Command::SET;
  if (strcasecmp(commandName, "GET") == 0) return Command::GET;
  if (strcasecmp(commandName, "EXISTS") == 0) return Command::EXISTS;
  if (strcasecmp(commandName, "DEL") == 0) return Command::DEL;
  if (strcasecmp(commandName, "UPDATE") == 0) return Command::UPDATE;
  if (strcasecmp(commandName, "KEYS") == 0) return Command::KEYS;
  if (strcasecmp(commandName, "RENAME") == 0) return Command::RENAME;
  if (strcasecmp(commandName, "TTL") == 0) return Command::TTL;
  if (strcasecmp(commandName, "FIND") == 0) return Command::FIND;
  if (strcasecmp(commandName, "SHOWALL") == 0) return Command::SHOWALL;
  if (strcasecmp(commandName, "UPLOAD") == 0) return Command::UPLOAD;
  if (strcasecmp(commandName, "EXPORT") == 0) return Command::EXPORT;
  if (strcasecmp(commandName, "HELP") == 0) return Command::HELP;
  if (strcasecmp(commandName, "RETURN") == 0) return Command::RETURN;
  return Command::ERROR;
}

void Interface::TrySet(const std::vector<std::string>& commandArgs) {
  Storage::key_type key;
  Storage::data_type values;
  int ex = 0;

  key = commandArgs.at(1);
  values.last_name = commandArgs.at(2);
  values.first_name = commandArgs.at(3);
  values.year_of_birth = std::stoi(commandArgs.at(4));
  values.city = commandArgs.at(5);
  values.coins = std::stoi(commandArgs.at(6));
  if (commandArgs.size() > 8) ex = std::stoi(commandArgs.at(8));

  storage->Set(key, values, ex);

  auto findedValues = storage->Get(key);
  if (findedValues.has_value() and findedValues == values) {
    std::cout << "OK" << std::endl;
    values.Print();
  } else
    std::cout << "(null)" << std::endl;
}

void Interface::TryGet(const std::vector<std::string>& commandArgs) {
  Storage::key_type key = commandArgs.at(1);

  auto findedValues = storage->Get(key);
  if (findedValues.has_value()) {
    std::cout << "OK" << std::endl;
    findedValues.value().Print();
  } else
    std::cout << "(null)" << std::endl;
}

void Interface::TryExists(const std::vector<std::string>& commandArgs) {
  Storage::key_type key = commandArgs.at(1);

  if (storage->Exists(key)) {
    std::cout << "true" << std::endl;
  } else
    std::cout << "false" << std::endl;
}

void Interface::TryDel(const std::vector<std::string>& commandArgs) {
  Storage::key_type key = commandArgs.at(1);

  if (storage->Exists(key)) {
    storage->Del(key);
    std::cout << "true" << std::endl;
  } else
    std::cout << "false" << std::endl;
}

void Interface::TryUpdate(const std::vector<std::string>& commandArgs) {
  Storage::key_type key = commandArgs.at(1);
  Storage::data_type values;

  storage->Update(
      key, {commandArgs.at(2), commandArgs.at(3),
            commandArgs.at(4) == "-" ? -1 : std::stoi(commandArgs.at(4)),
            commandArgs.at(5),
            commandArgs.at(6) == "-" ? -1 : std::stoi(commandArgs.at(6))});

  auto findedValues = storage->Get(key);
  if (findedValues.has_value()) {
    std::cout << "OK" << std::endl;
    findedValues.value().Print();
  } else
    std::cout << "(null)" << std::endl;
}

void Interface::TryKeys() {
  auto findedValues = storage->Keys();
  if (!findedValues.empty()) {
    for (size_t i = 0; i < findedValues.size(); i++) {
      std::cout << i + 1 << ')' << findedValues.at(i) << std::endl;
    }
  } else {
    std::cout << "(null)" << std::endl;
  }
}

void Interface::TryRename(const std::vector<std::string>& commandArgs) {
  Storage::key_type key1 = commandArgs.at(1);
  Storage::key_type key2 = commandArgs.at(2);

  if (storage->Rename(key1, key2)) {
    std::cout << "true" << std::endl;
  } else {
    std::cout << "false" << std::endl;
  }
}

void Interface::TryShowall() {
  auto findedValues = storage->ShowAll();
  if (!findedValues.empty()) {
    printf("%-2s|%-s|%-15s|%-15s|%-15s|%-15s|\n", "№", "Last name",
           "First name", "Year", "City", "Number of coins");
    for (size_t i = 0; i < findedValues.size(); i++) {
      Storage::data_type i_values = findedValues.at(i);

      printf("%-2zu|%-15s|%-15s|%-15d|%-15s|%-15d\n", i + 1,
             i_values.last_name.c_str(), i_values.first_name.c_str(),
             i_values.year_of_birth, i_values.city.c_str(), i_values.coins);
    }
  } else
    std::cout << "(null)" << std::endl;
}

void Interface::TryTtl(const std::vector<std::string>& commandArgs) {
  Storage::key_type key = commandArgs.at(1);
  auto findedValues = storage->TTL(key);
  if (findedValues.has_value() and findedValues.value() != 0) {
    std::cout << findedValues.value() << std::endl;
  } else
    std::cout << "(null)" << std::endl;
}

void Interface::TryFind(const std::vector<std::string>& commandArgs) {
  Storage::data_type values;

  auto findedValues = storage->Find({
      commandArgs.at(1),
      commandArgs.at(2),
      commandArgs.at(3) != "-" ? std::stoi(commandArgs.at(3)) : -1,
      commandArgs.at(4),
      commandArgs.at(5) != "-" ? std::stoi(commandArgs.at(5)) : -1,
  });

  if (!findedValues.empty()) {
    for (size_t i = 0; i < findedValues.size(); i++) {
      std::cout << i + 1 << ')' << findedValues.at(i) << std::endl;
    }
  } else {
    std::cout << "(null)" << std::endl;
  }
}

void Interface::TryUpload(const std::vector<std::string>& commandArgs) {
  try {
    int rowCount = storage->Upload(commandArgs.at(1));
    std::cout << "OK " << rowCount << "\n";
  } catch (std::invalid_argument const& e) {
    std::cout << e.what() << "\n";
  }
}

void Interface::TryExport(const std::vector<std::string>& commandArgs) {
  try {
    int rowCount = storage->Export(commandArgs.at(1));
    std::cout << "OK " << rowCount << "\n";
  } catch (std::invalid_argument const& e) {
    std::cout << e.what() << "\n";
  }
}

void Interface::ShowHelpMenu() {
  std::cout << "Команды необходимо вводить в представленном формате:\n\n"

            << "\t<Ключ> - должен состоять из букв латинского алфавита, может "
               "содержать цифры\n"
            << "\t<Фамилия>, <Имя>, <Город> - должны состоять из любых "
               "непробельных символов\n"
            << "\t<Число текущих коинов>, <Время в секундах>, <Город> - должны "
               "состоять из цифр\n"
            << "\tЕсли в первом или последнем символе входной строки "
               "содержится символ \",\n"
            << "\tто он символ будет удален, и в хранилище будет обрезанная "
               "строка\n\n"

            << "\tSET <ключ> <Фамилия> <Имя> <Год рождения> <Город> <Число "
               "текущих коинов> EX <время в \n"
            << "\tсекундах>(необязательное поле)\n"
            << "\tКоманда используется для установки ключа и его значения.\n\n"

            << "\tGET <ключ>\n"
            << "\tКоманда используется для получения значения, связанного с "
               "ключом. Если такой записи нет, \n"
            << "\tто будет возвращён (null)\n\n"

            << "\tEXISTS <ключ>\n"
            << "\tЭта команда проверяет, существует ли запись с данным ключом. "
               "Она возвращает true если \n"
            << "\tобъект существует или false если нет\n\n"

            << "\tDEL <ключ>\n"
            << "\tКоманда удаляет ключ и соответствующее значение, после чего "
               "возвращает true, если запись\n"
            << "\tуспешно удалена, в противном случае - false\n\n"

            << "\tUPDATE <ключ> <Фамилия> <Имя> <Год рождения> <Город> <Число "
               "текущих коинов>\n"
            << "\tКоманда обновляет значение по соответствующему ключу, если "
               "такой ключ существует\n"
            << "\tЕсли же какое-то поле менять не планируется, то на его месте "
               "ставится прочерк '-'\n\n"

            << "\tKEYS\n"
            << "\tВозвращает все ключи, которые есть в хранилище\n\n"

            << "\tRENAME <ключ> <ключ>\n"
            << "\tКоманда используется для переименования ключей\n\n"

            << "\tTTL <ключ>\n"
            << "\tКогда ключ установлен с истечением срока действия, эту "
               "команду можно использовать для\n"
            << "\tпросмотра оставшегося времени.\n"
            << "\tЕсли записи с заданным ключом не существует, то возвращается "
               "(null)\n\n"

            << "\tFIND <ключ> <Фамилия> <Имя> <Год рождения> <Город> <Число "
               "текущих коинов>\n"
            << "\tЭта команда используется для восстановления ключа (или "
               "ключей) по заданному значению.\n"
            << "\tЕсли же по каким-то полям не будет выполняться поиск, то на "
               "их месте ставится прочерк -\n\n"

            << "\tSHOWALL\n"
            << "\tКоманда для получения всех записей, которые содержатся в "
               "key-value хранилище на текущий "
            << "момент\n\n"

            << "\tUPLOAD\n"
            << "\tДанная команда используется для загрузки данных из файла. "
               "Файл содержит список \n"
            << "\tзагружаемых данных в формате\n\n"

            << "\tEXPORT\n"
            << "\tДанная команда используется для выгрузки данныхв файл\n\n";
}

}  // namespace sfleta
