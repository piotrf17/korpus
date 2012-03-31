#include "document.h"
#include "lexeme.h"
#include "pugixml/pugixml.hpp"

namespace korpus {

Document::Document() {
}

Document::~Document() {
  // Iterate over sections.
  for (auto i = sections_.begin(); i != sections_.end(); ++i) {
    // Iterate over sentences.
    for (auto j = i->begin(); j != i->end(); ++j) {
      // Iterate over words.
      for (auto k = j->begin(); k != j->end(); ++k) {
        delete *k;
      }
    }
  }
}

bool Document::LoadFromXml(const std::string& doc_root, std::string* error) {
  std::string filename(doc_root + "/ann_morphosyntax.xml");
  
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(filename.c_str());
  if (!result) {
    *error = "failed to XML parse " + filename;
    return false;
  }

  pugi::xml_node body = doc.child("teiCorpus").child("TEI").
      child("text").child("body");
  if (body.empty()) {
    *error = "failed to find body node";
    return false;
  }

  for (pugi::xml_node section_node = body.child("p");
       section_node; section_node = section_node.next_sibling("p")) {
    sections_.push_back(Section());
    Section& section = sections_.back();
    
    for (pugi::xml_node sentence_node = section_node.child("s");
         sentence_node; sentence_node = sentence_node.next_sibling("s")) {
      section.push_back(Sentence());
      Sentence& sentence = section.back();
      
      for (pugi::xml_node word_node = sentence_node.child("seg");
           word_node; word_node = word_node.next_sibling("seg")) {
        sentence.push_back(Lexeme::FromXml(word_node));
      }
    }
  }

  return true;
}

}
