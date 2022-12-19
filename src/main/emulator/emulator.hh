#include <iostream>
#include <fstream>
#include <map>
#include "../file_handler.hh"
#include "tokeniser.hh"
#define _MEM_SIZE 128

struct EmulatorFlags {
  bool StringConstant;
  bool Program;
};

class Emulator {
 public:
  Emulator() { 
    flags_.StringConstant = false;
    flags_.Program = false;
  }
  
  int EmulateLine(std::string& line) {
    int ret = 0;
    if (!flags_.Program) {  // Program setup
      if (flags_.StringConstant) {
        ret = HandleStringConstant(line);
      }
      if (line[0] == 'S') {
        flags_.StringConstant = true;
      } else if (line == "P:") {
        flags_.Program = true;
      }
    } else {  // Main program
      ret = EmulateMainProgram(line);
    }
    prevLine_ = line;
    return ret;
  }

 private:
  std::string prevLine_;
  EmulatorFlags flags_;
  std::map<size_t, std::string> stringMap_;
  uint32_t memory_[_MEM_SIZE] = {};

  size_t GetStringNumber(std::string& string) {
    return std::stoi(string.substr(1, std::string::npos));
  }

  void PrintMemory() {
    std::cout << "[";
    for (size_t i = 0; i < _MEM_SIZE - 1; i++) {
      std::cout << memory_[i] << ",";
    }
    std::cout << memory_[_MEM_SIZE - 1] << "]" << std::endl;
  }

  int EmulateMainProgram(std::string& line) {
    std::vector<std::string> tokens = Tokenise(line);
    if (tokens[0] == "PRINT") {
      if (tokens.size() != 2) {
        std::cerr << "Invalid PRINT token" << std::endl;
        return 1;
      }
      int const_number = GetStringNumber(tokens[1]);
      std::cout << stringMap_[const_number] << std::endl;
    } else if (line == "_PMEM()") {
      PrintMemory();
    } else if (tokens[0] == "CMOV") {
      if (tokens.size() != 3) {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return 1;
      }
      std::string location = tokens[2];
      if (location.at(0) != '&') {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return 1;
      }
      int constant;
      try {
        constant = std::stoi(tokens[1]);
      } catch(std::invalid_argument const& ex) {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return 1;
      } catch(std::out_of_range const& ex) {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return 1;
      }
      memory_[GetStringNumber(location)] = constant;
    } else if (tokens[0] == "RMOV") {
      if (tokens.size() != 3) {
        std::cerr << "Invalid RMOV tokens" << std::endl;
        return 1;
      }
      std::string strtLoc = tokens[1];
      std::string dstLoc = tokens[2];
      if (strtLoc.at(0) != '&' || dstLoc.at(0) != '&') {
        std::cerr << "Invalid RMOV tokens" << std::endl;
        return 1;
      }
      memory_[GetStringNumber(dstLoc)] = memory_[GetStringNumber(strtLoc)];
    }
    return 0;
  }

  int HandleStringConstant(std::string& line) {
    // Setup for string identifier
    if (prevLine_.length() <= 1) {
      std::cerr << "Invalid string identifier" << std::endl;
      return 1;
    }
    int const_number = GetStringNumber(prevLine_);
    std::pair<int, std::string> p(const_number, line);
    stringMap_.insert(p);
    flags_.StringConstant = false;
    return 0;
  }
};
