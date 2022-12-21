#include <iostream>
#include <fstream>
#include <map>
#include "../file_handler.hh"
#include "emulator.hh"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: psemulate <filepath>" << std::endl;
    return -1;
  }
  const std::string filename = argv[1];
  std::map<size_t, std::string> lines = OpenFile(filename);
  Emulator emulator(filename);
  for (size_t lineNumber = 1; lineNumber <= lines.size(); lineNumber++) {
    std::string line = lines[lineNumber];
    emulator.EmulateLine(line);
  }
  return 0;
}
