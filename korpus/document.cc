#include "korpus/document.h"

#include <cassert>
#include <iostream>

#include "korpus/lexeme.h"
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

bool Document::LoadFromBinary(std::ifstream* infile) {
  // Load Sections.
  size_t num_sections;
  infile->read(reinterpret_cast<char*>(&num_sections), sizeof(size_t));
  while (num_sections > 0) {
    sections_.push_back(Section());
    Section& section = sections_.back();
    // Load Sentences.
    size_t num_sentences;
    infile->read(reinterpret_cast<char*>(&num_sentences), sizeof(size_t));
    while (num_sentences > 0) {
      section.push_back(Sentence());
      Sentence& sentence = section.back();
      // Load Lexemes.
      size_t num_lexemes;
      infile->read(reinterpret_cast<char*>(&num_lexemes), sizeof(size_t));
      while (num_lexemes > 0) {
        sentence.push_back(Lexeme::FromBinary(infile));
        --num_lexemes;
      }      
      --num_sentences;
    }    
    --num_sections;
  }
  return true;
}

bool Document::SaveToBinary(std::ofstream* outfile) const {
  size_t num_sections = sections_.size();
  outfile->write(reinterpret_cast<char*>(&num_sections), sizeof(size_t));
  for (auto sec_it = sections_.begin(); sec_it != sections_.end(); ++sec_it) {
    size_t num_sentences = sec_it->size();
    outfile->write(reinterpret_cast<char*>(&num_sentences), sizeof(size_t));
    for (auto sen_it = sec_it->begin(); sen_it != sec_it->end(); ++sen_it) {
      size_t num_lexemes = sen_it->size();
      outfile->write(reinterpret_cast<char*>(&num_lexemes), sizeof(size_t));
      for (auto lex_it = sen_it->begin(); lex_it != sen_it->end(); ++lex_it) {
        if (!(*lex_it)->SaveToBinary(outfile)) {
          return false;
        }
      }
    }
  }
  return true;
}

Lexeme* Document::GetLexeme(uint64_t token) const {
  int section, sentence, lexeme;
  DecodeToken(token, &section, &sentence, &lexeme);
  return sections_[section][sentence][lexeme];
}

const std::vector<Lexeme*>& Document::GetSentence(uint64_t token) const {
  int section, sentence, lexeme;
  DecodeToken(token, &section, &sentence, &lexeme);
  return sections_[section][sentence];
}


inline void Document::EncodeToken(int section,
                                  int sentence,
                                  int lexeme,
                                  uint64_t* token) {
  assert(lexeme < (1 << 16));
  assert(section < (1 << 16));
  *token =
      (static_cast<uint64_t>(section) << 48) |
      (static_cast<uint64_t>(sentence) << 16) |
      (lexeme & 0x0000ffff);
}

inline void Document::DecodeToken(uint64_t token,
                                  int* section,
                                  int* sentence,
                                  int* lexeme) {
  *section = static_cast<int>(token >> 48);
  *sentence = static_cast<int>(token >> 16);
  *lexeme = static_cast<int>(token & 0xffff);
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

uint64_t DocumentIterator::token() const {
  uint64_t token;
  Document::EncodeToken(section_, sentence_, lexeme_, &token);
  return token;
}

}
