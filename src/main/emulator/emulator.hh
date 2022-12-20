#include <iostream>
#include <fstream>
#include <map>
#include "../file_handler.hh"
#include "tokeniser.hh"
#define _MEM_SIZE 128
#define SUCCESS 0
#define ERROR 1

struct EmulatorFlags {
  bool StringConstant;
  bool Program;
  bool Fail;
};

class Emulator {
 public:
  Emulator() { 
    flags_.StringConstant = false;
    flags_.Program = false;
    flags_.Fail = false;
  }
  
  int EmulateLine(std::string& line) {
    int ret = SUCCESS;
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

  // Parse int from string at first index. Sets Fail flag if couldn't parse.
  size_t GetStringNumber(std::string& string) {
    int number = 0;
    try {
      number = std::stoi(string.substr(1, std::string::npos));
    } catch (std::invalid_argument const& ex) {
        std::cerr << "Couldn't parse int from \"" << string << "\"" << std::endl;
        flags_.Fail = true;
    } catch (std::out_of_range const& ex) {
        std::cerr << "Couldn't parse int from \"" << string << "\"" << std::endl;
        flags_.Fail = true;
    }
    return number;
  }

  // Prettily print memory to std out.
  void PrintMemory() {
    std::cout << "[";
    for (size_t i = 0; i < _MEM_SIZE - 1; i++) {
    std::cout << memory_[i] << ",";
    }
    std::cout << memory_[_MEM_SIZE - 1] << "]" << std::endl;
  }

  // Handle print instruction.
  int HandlePrint(std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
      std::cerr << "Invalid PRINT token" << std::endl;
      return ERROR;
    }
    int const_number = GetStringNumber(tokens[1]);
    if (flags_.Fail) {
      return ERROR;
    }
    if (!stringMap_.contains(const_number)) {
      std::cerr << "S" << const_number << " not found"
                << std::endl;
      return ERROR;
    }
    std::cout << stringMap_[const_number] << std::endl;
    return SUCCESS;
  }

  int EmulateMainProgram(std::string& line) {
    if (line.length() == 0) {
      return SUCCESS;
    }
    std::vector<std::string> tokens = Tokenise(line);
    std::string& instruction = tokens[0];
    if (instruction == "PRINT") {
      return HandlePrint(tokens);
    } else if (line == "_PMEM()") {
      PrintMemory();
    } else if (instruction == "CMOV") {
      if (tokens.size() != 3) {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return ERROR;
      }
      std::string location = tokens[2];
      if (location.at(0) != '&') {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return ERROR;
      }
      int constant;
      try {
        constant = std::stoi(tokens[1]);
      } catch(std::invalid_argument const& ex) {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return ERROR;
      } catch(std::out_of_range const& ex) {
        std::cerr << "Invalid CMOV tokens" << std::endl;
        return ERROR;
      }
      int locationNumber = GetStringNumber(location);
      if (flags_.Fail) {
        return ERROR;
      }
      memory_[locationNumber] = constant;
    } else if (instruction == "RMOV") {
      if (tokens.size() != 3) {
        std::cerr << "Invalid RMOV tokens" << std::endl;
        return ERROR;
      }
      std::string strtLoc = tokens[1];
      std::string dstLoc = tokens[2];
      if (strtLoc.at(0) != '&' || dstLoc.at(0) != '&') {
        std::cerr << "Invalid RMOV tokens" << std::endl;
        return ERROR;
      }
      memory_[GetStringNumber(dstLoc)] = memory_[GetStringNumber(strtLoc)];
    } else {
      std::cerr << "Invalid instruction " << line << std::endl;
      return ERROR;
    }
    return SUCCESS;
  }

  int HandleStringConstant(std::string& line) {
    // Setup for string identifier
    if (prevLine_.length() <= 1) {
      std::cerr << "Invalid string identifier" << std::endl;
      return ERROR;
    }
    int const_number = GetStringNumber(prevLine_);
    std::pair<int, std::string> p(const_number, line);
    stringMap_.insert(p);
    flags_.StringConstant = false;
    return SUCCESS;
  }
};
