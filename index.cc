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
  int tokens = 0;
  
  for (CorpusIterator it(corpus); !it.end(); ++it) {
    Location loc;
    loc.docid = it.docid();
    loc.token = it.token();

    base_index_[it->base()].push_back(loc);
    index_[it->value()].push_back(loc);

    ++tokens;
  }

  std::cout << "Tokens: " << tokens << std::endl;
  std::cout << "Base Index Size: " << base_index_.size() << std::endl;
  std::cout << "Index Size: " << index_.size() << std::endl;
}

bool Index::QueryBase(const std::string& word,
                      ResultSet* result) const {
  auto it = base_index_.find(word);
  if (it == base_index_.end()) {
    std::cout << "Token not found!";
  } else {
    std::cout << "Posting list size: " << it->second.size() << std::endl;;
  }
  return true;
}

}  // namespace korpus
