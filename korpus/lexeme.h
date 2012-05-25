#ifndef KORPUS_LEXEME_H
#define KORPUS_LEXEME_H

#include <fstream>
#include <string>
#include <unordered_set>

namespace pugi {
class xml_node;
}

namespace korpus {
  
class Lexeme {
 public:
  explicit Lexeme(const std::string& value);
  Lexeme(const std::string& value, const std::string& analysis);
  ~Lexeme();

  static Lexeme* FromXml(const pugi::xml_node& node);
  static Lexeme* FromBinary(std::ifstream* infile);

  bool SaveToBinary(std::ofstream* outfile) const;

  const std::string& value() const {
    return value_;
  }

  const std::string& base() const {
    return base_;
  }

  const std::unordered_set<std::string>& attributes() const {
    return attributes_;
  }
  
 private:
  std::string value_;
  std::string base_;
  std::unordered_set<std::string> attributes_;
};
  
}  // namespace korpus

#endif
