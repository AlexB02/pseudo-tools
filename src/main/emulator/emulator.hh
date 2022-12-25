#include <iostream>
#include <fstream>
#include <map>
#include "error.hh"
#include "../file_handler.hh"
#include "tokeniser.hh"
#include "number.hh"
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
  Emulator(std::string fileName) : exceptionHandler_(fileName) { 
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
  int32_t memory_[_MEM_SIZE] = {};
  ExceptionHandler exceptionHandler_;
  std::string const fileName_;

  // Parse int from string at first index.
  size_t GetStringNumber(std::string const string, int from) {
    assert(from < string.size());
    std::string substring = string.substr(from, std::string::npos);
    if (!ContainsLeadingInteger(substring)) {
      exceptionHandler_.ThrowException(ERROR, "Couldn't parse int from \"" + string + "\"");
    }
    return std::stoi(substring);
  }

  // Prettily print memory to std out.
  void PrintMemory() {
    std::cout << "[";
    for (size_t i = 0; i < _MEM_SIZE - 1; i++) {
    std::cout << memory_[i] << ",";
    }
    std::cout << memory_[_MEM_SIZE - 1] << "]" << std::endl;
  }

  bool IsAddress(std::string const address) {
    if (address.at(0) != '&' || address.size() == 1) {
      return false;
    }
    size_t number = GetStringNumber(address, 1);
    return true;
  }

  void HandlePrintAddress(std::string const address) {
    int loc = GetStringNumber(address, 1);
    if (loc >= _MEM_SIZE) {
      exceptionHandler_.ThrowException(2, "Out of memory access");
    }
    std::cout << memory_[loc] << std::endl;
  }

  // Handle print instruction.
  int HandlePrint(std::vector<std::string>& tokens) {
    if (tokens.size() != 2) {
      std::cerr << "Invalid PRINT token" << std::endl;
      return ERROR;
    }
    std::string& printArg = tokens[1];
    exceptionHandler_.Assert(printArg.size() > 1,
                             "Invalid PRINT argument: " + printArg);
    if (IsAddress(printArg)) {
      HandlePrintAddress(printArg);
      return SUCCESS;
    }
    int const_number = GetStringNumber(printArg, 1);
    if (!stringMap_.contains(const_number)) {
      std::cerr << "S" << const_number << " not found"
                << std::endl;
      return ERROR;
    }
    std::cout << stringMap_[const_number] << std::endl;
    return SUCCESS;
  }

  int EmulateMainProgram(std::string const line) {
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
      int locationNumber = GetStringNumber(location, 1);
      memory_[locationNumber] = constant;
    } else if (instruction == "RMOV") {
      exceptionHandler_.Assert(tokens.size() == 3, "Invalid RMOV tokens");
      std::string strtLoc = tokens[1];
      std::string dstLoc = tokens[2];
      exceptionHandler_.Assert(IsAddress(strtLoc) && IsAddress(dstLoc),
                               "Invalid RMOV tokens");
      memory_[GetStringNumber(dstLoc, 1)] =
          memory_[GetStringNumber(strtLoc, 1)];
    } else if (instruction == "CADD") {
      exceptionHandler_.Assert(tokens.size() == 3, "Invalid CADD");
      std::string const constant = tokens[1];
      std::string const address = tokens[2];
      exceptionHandler_.Assert(ContainsLeadingInteger(constant), "Invalid constant in CADD");
      exceptionHandler_.Assert(IsAddress(address), "Invalid address in CADD");
      int add_value = std::stoi(constant);
      size_t loc = GetStringNumber(address, 1);
      memory_[loc] = memory_[loc] + add_value;
    } else if (instruction == "RADD") {
      exceptionHandler_.Assert(tokens.size() == 3, "Invalid RADD tokens");
      std::string strtLoc = tokens[1];
      std::string dstLoc = tokens[2];
      exceptionHandler_.Assert(IsAddress(strtLoc) && IsAddress(dstLoc),
                               "Invalid RADD tokens");
      memory_[GetStringNumber(dstLoc, 1)] =
          memory_[GetStringNumber(dstLoc, 1)] + memory_[GetStringNumber(strtLoc, 1)];
    }
    else {
      exceptionHandler_.ThrowException(1, "Invalid instruction " + line);
    }
    return SUCCESS;
  }

  int HandleStringConstant(std::string& line) {
    // Setup for string identifier
    if (prevLine_.length() <= 1) {
      std::cerr << "Invalid string identifier" << std::endl;
      return ERROR;
    }
    int const_number = GetStringNumber(prevLine_, 1);
    std::pair<int, std::string> p(const_number, line);
    stringMap_.insert(p);
    flags_.StringConstant = false;
    return SUCCESS;
  }
};
