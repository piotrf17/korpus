#include <iostream>

#include "corpus.h"
#include "document.h"

namespace korpus {

Corpus::Corpus() {
}

Corpus::~Corpus() {
  for (auto it = documents_.begin(); it != documents_.end(); ++it) {
    delete (*it);
  }
}

bool Corpus::LoadFromXml(const std::string& corpus_root,
                         std::string* error) {
  *error = "not implemented yet";
  return false;
}


bool Corpus::LoadFromXml(const std::string& corpus_root,
                         const std::vector<std::string>& document_names,
                         std::string* error) {
  for (auto it = document_names.begin(); it != document_names.end(); ++it) {
    Document* doc = new Document();
    std::string doc_root = corpus_root + (*it);
    if (!doc->LoadFromXml(doc_root, error)) {
      delete doc;
      return false;
    }
    documents_.push_back(doc);
  }

  return true;
}

}  // namespace korpus
