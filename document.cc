#include <iostream>

#include "document.h"
#include "lexeme.h"
#include "pugixml/pugixml.hpp"

namespace korpus {

const char* kMorphoFileName = "ann_morphosyntax.xml";

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
  std::string filename(doc_root + "/" + kMorphoFileName);
  
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

DocumentIterator::DocumentIterator(const Document& doc) :
    doc_(doc),
    section_(0),
    sentence_(0),
    lexeme_(0) {
}

DocumentIterator::DocumentIterator(const DocumentIterator& it) :
    doc_(it.doc_),
    section_(it.section_),
    sentence_(it.sentence_),
    lexeme_(it.lexeme_) {
}

DocumentIterator::~DocumentIterator() {
}

DocumentIterator& DocumentIterator::operator++() {
  if (!end()) {
    ++lexeme_;
    if (lexeme_ >= static_cast<int>(doc_.sections_[section_][sentence_].size())) {
      ++sentence_;
      if (sentence_ >= static_cast<int>(doc_.sections_[section_].size())) {
        ++section_;
        sentence_ = 0;
      }
      lexeme_ = 0;
    }
  }
  return *this;
}

bool DocumentIterator::operator==(const DocumentIterator& it) const {
  return section_ == it.section_ && sentence_ == it.sentence_ && lexeme_ == it.lexeme_;
}

bool DocumentIterator::operator!=(const DocumentIterator& it) const {
  return !operator==(it);
}

Lexeme& DocumentIterator::operator*() {
  return *operator->();
}

Lexeme* DocumentIterator::operator->() {
  return doc_.sections_[section_][sentence_][lexeme_];
}

bool DocumentIterator::end() const {
  return section_ >= static_cast<int>(doc_.sections_.size());
}

int DocumentIterator::token() const {
  return (section_ << 24) + (sentence_ << 16) + lexeme_;
}

}
