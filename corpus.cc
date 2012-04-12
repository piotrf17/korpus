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

Lexeme* Corpus::GetLexeme(int docid, uint64_t token) const {
  return documents_[docid]->GetLexeme(token);
}

const std::vector<Lexeme*>& Corpus::GetSentence(int docid,
                                                uint64_t token) const {
  return documents_[docid]->GetSentence(token);
}

CorpusIterator::CorpusIterator(const Corpus& corpus) :
    corpus_(corpus),
    it_(corpus.documents_.begin()) {
  if (!end()) {
    doc_it_.reset(new DocumentIterator(**it_));
  }
}

CorpusIterator::CorpusIterator(const CorpusIterator& it) :
    corpus_(it.corpus_),
    it_(it.it_) {
  doc_it_.reset(new DocumentIterator(*it.doc_it_));
}

CorpusIterator::~CorpusIterator() {
}

CorpusIterator& CorpusIterator::operator++() {
  doc_it_->operator++();
  if (doc_it_->end()) {
    ++it_;
    doc_it_.reset(new DocumentIterator(**it_));
  }
  return *this;
}

bool CorpusIterator::operator==(const CorpusIterator& it) const {
  return it_ == it.it_ && doc_it_->operator==(*it.doc_it_);
}

bool CorpusIterator::operator!=(const CorpusIterator& it) const {
  return !operator==(it);
}

Lexeme& CorpusIterator::operator*() {
  return *operator->();
}

Lexeme* CorpusIterator::operator->() {
  return doc_it_->operator->();
}

bool CorpusIterator::end() const {
  return it_ == corpus_.documents_.end();
}

int CorpusIterator::docid() const {
  return it_ - corpus_.documents_.begin();
}

uint64_t CorpusIterator::token() const {
  return doc_it_->token();
}

}  // namespace korpus
