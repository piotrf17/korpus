#include <iostream>

#include "corpus.h"
#include "index.h"
#include "lexeme.h"

namespace korpus {

Index::Index() {
}

Index::~Index() {
}

void Index::Build(const Corpus& corpus) {
  for (CorpusIterator it(corpus); !it.end(); ++it) {
    Location loc;
    loc.docid = it.docid();
    loc.token = it.token();

    base_index_[it->base()].push_back(loc);
    index_[it->value()].push_back(loc);
  }

  std::cout << "Base Index Size: " << base_index_.size() << std::endl;
  std::cout << "Index Size: " << index_.size() << std::endl;
}

bool Index::QueryBase(const std::string& word,
                      ResultSet* result) {
  return false;
}

}  // namespace korpus
