#include "korpus/result.h"

#include "korpus/corpus.h"
#include "korpus/index.h"
#include "korpus/lexeme.h"

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

const std::string& Result::base() const {
  return lexeme_->base();
}

const std::string& Result::value() const {
  return lexeme_->value();
}

};
