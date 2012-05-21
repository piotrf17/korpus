#include "korpus/lexeme.h"

#include <boost/algorithm/string.hpp>
#include <cstring>
#include <vector>

#include "korpus/lexeme.pb.h"
#include "pugixml/pugixml.hpp"

namespace korpus {

Lexeme::Lexeme(const std::string& value) :
    value_(value) {
}

Lexeme::Lexeme(const std::string& value, const std::string& analysis) : 
  value_(value) {
  std::vector<std::string> tokens;
  boost::split(tokens, analysis, boost::is_any_of(":"));
  if (tokens.size() > 0) {
    base_ = tokens[0];
    attributes_.insert(tokens.begin() + 1, tokens.end());
  }
}

Lexeme::~Lexeme() {
}

Lexeme* Lexeme::FromXml(const pugi::xml_node& node) {
  std::string value;
  std::string analysis;
  for (pugi::xml_node n = node.child("fs").child("f");
       n; n = n.next_sibling("f")) {
    if (!strcmp(n.attribute("name").value(), "orth")) {
      value = n.child("string").child_value();
    } else if (!strcmp(n.attribute("name").value(), "disamb")) {
      for (pugi::xml_node fs = n.child("fs").child("f");
           fs; fs = fs.next_sibling("f")) {
        if (!strcmp(fs.attribute("name").value(), "interpretation")) {
          analysis = fs.child("string").child_value();
        }
      }
    }
  }

  return new Lexeme(value, analysis);
}

Lexeme* Lexeme::FromBinary(std::ifstream* infile) {
  StoredLexeme sl;
  size_t data_size;
  infile->read(reinterpret_cast<char*>(&data_size), sizeof(size_t));
  char* data = new char[data_size];
  infile->read(data, data_size);
  if (!sl.ParseFromArray(data, data_size)) {
    delete[] data;
    return false;
  }
  delete[] data;
  Lexeme* lexeme = new Lexeme(sl.value());
  lexeme->base_ = sl.base();
  lexeme->attributes_.insert(sl.attribute().begin(),
                             sl.attribute().end());
  return lexeme;
}

bool Lexeme::SaveToBinary(std::ofstream* outfile) const {
  StoredLexeme lexeme;
  lexeme.set_value(value_);
  lexeme.set_base(base_);
  for (auto it = attributes_.begin(); it != attributes_.end(); ++it) {
    *lexeme.add_attribute() = *it;
  }
  std::string data;
  if (!lexeme.SerializeToString(&data)) {
    return false;
  }
  size_t data_size = data.size();
  outfile->write(reinterpret_cast<char*>(&data_size), sizeof(size_t));  
  outfile->write(data.c_str(), data_size);
  return true;
}

}  // namespace korpus
