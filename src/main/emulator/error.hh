#include <stdlib.h>
#include <string>
#include <iostream>

#define TODO ThrowException(1, "Reached TODO\nExiting with 1")

class ExceptionHandler {
  public:
    ExceptionHandler(std::string filename) : fileName_(filename) {}

    void ThrowException(int exitCode, std::string const message) {
      std::cerr << message << std::endl;
      std::cerr << "Exception code: " 
                << exitCode 
                << " in " 
                << fileName_ 
                << std::endl;
      exit(exitCode);
    }

    void Assert(bool cond, std::string const errMsg) {
      if (!cond) {
        ThrowException(3, errMsg);
      }
    }
  private:
    std::string const fileName_;
};
