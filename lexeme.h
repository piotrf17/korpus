#ifndef KORPUS_LEXEME_H
#define KORPUS_LEXEME_H

#include <string>
#include <unordered_set>

namespace pugi {
class xml_node;
}

namespace korpus {
  
class Lexeme {
 public:
  Lexeme(const std::string& value, const std::string& analysis);
  ~Lexeme();

  static Lexeme* FromXml(const pugi::xml_node& node);
  
 private:
  std::string value_;
  //  std::string base_;
  //  std::unordered_set<std::string> attributes_;
};
  
}  // namespace korpus

#endif
