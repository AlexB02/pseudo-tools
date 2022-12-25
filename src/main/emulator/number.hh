/**
 * @brief Returns true if the input begins with an integer.
 *        
 * @param value String to be checked.
 * @return If string begins with an integer.
 */
bool ContainsLeadingInteger(std::string const value) {
  if (value[0] == '-') {
    return isdigit(value[1]);
  }
  return isdigit(value[0]);
}
