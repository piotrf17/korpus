#ifndef KORPUS_CORPUS_H
#define KORPUS_CORPUS_H

#include <string>
#include <vector>

namespace korpus {

class Document;
  
class Corpus {
 public:
  Corpus();
  ~Corpus();

  bool LoadFromXml(const std::string& corpus_root,
                   std::string* error);
  bool LoadFromXml(const std::string& corpus_root,
                   const std::vector<std::string>& document_names,
                   std::string* error);
  
 private:
  std::vector<Document*> documents_;
};

}  // namespace korpus

#endif
