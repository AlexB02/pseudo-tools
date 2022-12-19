#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <map>
#include <iostream>
#include <string>
#include <fstream>

std::map<size_t, std::string> OpenFile(std::string filename) {
  std::map<size_t, std::string> lines;
  std::ifstream file(filename);
  std::string line;
  if (!file.is_open()) {
    std::cerr << "Couldn't open \"" << filename  << "\"" << std::endl;
    return lines;
  }
  size_t lineNumber = 1;
  while (std::getline(file, line)) {
    lines.emplace(lineNumber++, line);
  }
  file.close();
  return lines;
}

#endif