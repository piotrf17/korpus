#include "korpus/index.h"

#include <iostream>

#include "korpus/corpus.h"
#include "korpus/lexeme.h"
#include "korpus/result.h"

namespace korpus {

Index::Index(const Corpus& corpus) :
    corpus_(corpus) {
}

Index::~Index() {
}

void Index::Build() {
  int tokens = 0;
  
  for (CorpusIterator it(corpus_); !it.end(); ++it) {
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
                      std::vector<Result>* results) const {
  auto it = base_index_.find(word);
  if (it == base_index_.end()) {
    std::cout << "Token not found!";
  } else {
    const PostingList& posting_list = it->second;
    results->clear();
    for (auto loc_it = posting_list.begin(); loc_it != posting_list.end();
         ++loc_it) {
      results->push_back(Result(&corpus_, loc_it->docid, loc_it->token));
    }
  }
  return true;
}

}  // namespace korpus
