#include "lexeme.h"

#include <boost/algorithm/string.hpp>
#include <cstring>
#include <vector>

#include "pugixml/pugixml.hpp"

namespace korpus {

Lexeme::Lexeme(const std::string& value, const std::string& analysis) : 
  value_(value) {
  /*  std::vector<std::string> tokens;
  boost::split(tokens, analysis, boost::is_any_of(":"));
  if (tokens.size() > 0) {
    base_ = tokens[0];
    attributes_.insert(tokens.begin() + 1, tokens.end());
    }*/
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

}
