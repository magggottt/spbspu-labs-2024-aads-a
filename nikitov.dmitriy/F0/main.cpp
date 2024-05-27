#include <iostream>
#include <fstream>
#include <string>
#include <tree.hpp>
#include <limits>
#include "dictionary.hpp"
#include "commands.hpp"
#include <functional>

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "Russian");
  using namespace nikitov;

  Tree< std::string, Dictionary > dictOfDicts;
  for (int i = 1; i != argc + 1; ++i)
  {
    std::ifstream fileInput(argv[i]);
    if (fileInput.is_open())
    {
      Dictionary dict;
      fileInput >> dict;
      dictOfDicts.insert({ argv[i], dict });
      fileInput.close();
    }
  }

  Tree< std::pair< std::string, std::string >, std::function< void(const Tree< std::string, Dictionary >&, std::istream&,
    std::ostream&) > > constCommands;
  constCommands[{ "print", "dictionary" }] = printDictCmd;
  constCommands[{ "print", "all" }] = printAllCmd;
  constCommands[{ "find", "translation" }] = findTranslationCmd;
  constCommands[{ "find", "antonym" }] = findAntonymCmd;
  constCommands[{ "translate", "sentence" }] = translateSentenceCmd;
  constCommands[{ "translate", "file" }] = translateFileCmd;
  constCommands[{ "save", "dictionary" }] = saveCmd;

  Tree< std::pair< std::string, std::string >, std::function< void(Tree< std::string, Dictionary >&, std::istream&) > > commands;
  commands[{ "create", "dictionary" }] = createCmd;
  commands[{ "add", "translation" }] = addTranslationCmd;
  commands[{ "add", "antonym" }] = addAntonymCmd;
  commands[{ "edit", "primary" }] = editPrimaryCmd;
  commands[{ "edit", "secondary" }] = editSecondaryCmd;
  commands[{ "delete", "primary" }] = deletePrimaryCmd;
  commands[{ "delete", "secondary" }] = deleteSecondaryCmd;
  commands[{ "delete", "antonym" }] = deleteAntonymCmd;
  commands[{ "merge", "dictionary" }] = mergeCmd;

  std::string command;
  std::string parameter ;
  while (std::cin >> command && std::cin >> parameter)
  {
    try
    {
      constCommands.at({ command, parameter })(dictOfDicts, std::cin, std::cout);
    }
    catch (const std::out_of_range&)
    {
      try
      {
        commands.at({ command, parameter })(dictOfDicts, std::cin);
      }
      catch (const std::out_of_range&)
      {
        std::cerr << "<INVALID COMMAND>" << '\n';
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
      catch (const std::exception& e)
      {
        std::cerr << e.what() << '\n';
        std::cin.clear();
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
