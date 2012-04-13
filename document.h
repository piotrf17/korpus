#ifndef KORPUS_DOCUMENT_H
#define KORPUS_DOCUMENT_H

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#include <gtest/gtest_prod.h>

namespace korpus {

class Lexeme;
  
class Document {
 public:
  typedef std::vector<Lexeme*> Sentence;
  typedef std::vector<Sentence> Section;
  
  Document();
  ~Document();

  bool LoadFromXml(const std::string& doc_root, std::string* error);

  bool LoadFromBinary(std::ifstream* infile);
  bool SaveToBinary(std::ofstream* outfile) const;
  
  Lexeme* GetLexeme(uint64_t token) const;
  const std::vector<Lexeme*>& GetSentence(uint64_t token) const;

  // Exposed for testing.
  static void EncodeToken(int section,
                          int sentence,
                          int lexeme,
                          uint64_t* token);
  static  void DecodeToken(uint64_t token,
                           int* section,
                           int* sentence,
                           int* lexeme);
  
 private:  
  friend class DocumentIterator;
  
  std::vector<Section> sections_;
};

class DocumentIterator {
 public:
  DocumentIterator(const Document& doc);
  DocumentIterator(const DocumentIterator& it);
  ~DocumentIterator();

  DocumentIterator& operator++();

  bool operator==(const DocumentIterator& it) const;
  bool operator!=(const DocumentIterator& it) const;

  Lexeme& operator*();
  Lexeme* operator->();

  bool end() const;

  uint64_t token() const;
  
 private:
  const Document& doc_;
  int section_, sentence_, lexeme_;
};

}  // namespace korpus

#endif
