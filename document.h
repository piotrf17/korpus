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
  std::vector<Section> sections_;
};

}  // namespace korpus

#endif
