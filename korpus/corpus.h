#ifndef KORPUS_CORPUS_H
#define KORPUS_CORPUS_H

#include <boost/scoped_ptr.hpp>
#include <string>
#include <vector>

namespace korpus {

class Lexeme;
class Document;
class DocumentIterator;
  
class Corpus {
 public:
  Corpus();
  ~Corpus();

  bool LoadFromXml(const std::string& corpus_root,
                   std::string* error);
  bool LoadFromXml(const std::string& corpus_root,
                   const std::vector<std::string>& document_names,
                   std::string* error);
  
  bool LoadFromBinary(const std::string& binary_file);
  bool SaveToBinary(const std::string& binary_file) const;

  Lexeme* GetLexeme(int docid, uint64_t token) const;
  const std::vector<Lexeme*>& GetSentence(int docid, uint64_t token) const;
  
 private:
  friend class CorpusIterator;
  
  std::vector<Document*> documents_;
};

class CorpusIterator {
 public:
  CorpusIterator(const Corpus& corpus);
  CorpusIterator(const CorpusIterator& it);
  ~CorpusIterator();

  CorpusIterator& operator++();

  bool operator==(const CorpusIterator& it) const;
  bool operator!=(const CorpusIterator& it) const;

  Lexeme& operator*();
  Lexeme* operator->();

  bool end() const;

  int docid() const;
  uint64_t token() const;
  
 private:
  const Corpus& corpus_;
  std::vector<Document*>::const_iterator it_;
  boost::scoped_ptr<DocumentIterator> doc_it_;
};

}  // namespace korpus

#endif
