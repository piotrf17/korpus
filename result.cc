#include "result.h"

#include "corpus.h"
#include "index.h"
#include "lexeme.h"

namespace korpus {

Result::Result(const Corpus* corpus,
               const int docid,
               const uint64_t token) :
    corpus_(corpus),
    docid_(docid),
    token_(token) {
  lexeme_ = corpus_->GetLexeme(docid, token);
}

int Result::GetContext(std::vector<Lexeme*>* context) const {
  *context = corpus_->GetSentence(docid_, token_);
  for (unsigned int i = 0; i < context->size(); ++i) {
    if ((*context)[i] == lexeme_) {
      return i;
    }
  }
  return -1;
}

};
