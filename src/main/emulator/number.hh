
bool IsInt(std::string const value) {
  try {
      std::stoi(value);
      return true;
  } 
  catch (std::invalid_argument const& ex) {} 
  catch (std::out_of_range const& ex) {}
  return false;
}
