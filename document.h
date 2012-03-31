#ifndef KORPUS_DOCUMENT_H
#define KORPUS_DOCUMENT_H

#include <string>
#include <vector>

namespace korpus {

class Lexeme;
  
class Document {
 public:
  typedef std::vector<Lexeme*> Sentence;
  typedef std::vector<Sentence> Section;
  
  Document();
  ~Document();

  bool LoadFromXml(const std::string& doc_root, std::string* error);
  
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

  int token() const;
  
 private:
  const Document& doc_;
  int section_, sentence_, lexeme_;
};

}  // namespace korpus

#endif
