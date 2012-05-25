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

// Build the index.  By construction, the index will have sorted
// posting lists.
void Index::Build() {
  int tokens = 0;
  
  for (CorpusIterator it(corpus_); !it.end(); ++it) {
    Location loc;
    loc.docid = it.docid();
    loc.token = it.token();

    index_["^" + it->base()].push_back(loc);
    index_[it->value()].push_back(loc);

    for (const std::string& attribute : it->attributes()) {
      index_[":"+attribute].push_back(loc);
    }
    
    ++tokens;
  }

  std::cout << "Tokens: " << tokens << std::endl;
  std::cout << "Index Size: " << index_.size() << std::endl;
}

// Query the index.  The query is an AND across all the query_terms.
// Currently there is no way to phrase an OR in a query.
bool Index::Query(const std::vector<std::string> query_terms,
                  std::vector<Result>* results) const {
  results->clear();

  // Find positions that match all query terms.
  PostingList final;
  for (const std::string& term : query_terms) {
    auto it = index_.find(term);
    if (it == index_.end()) {
      return true;
    } else {
      const PostingList& matched = it->second;
      if (final.size() == 0) {
        final.assign(matched.begin(), matched.end());
      } else {
        PostingList temp;
        ListIntersect(matched, final, &temp);
        final.swap(temp);
      }
    }
  }

  // Create result set from these positions.
  for (const Location& loc : final) {
    results->push_back(Result(&corpus_, loc.docid, loc.token));
  }

  return true;
}

bool operator==(const Index::Location& a, const Index::Location& b) {
  return (a.docid == b.docid) && (a.token == b.token);
}

bool operator<(const Index::Location& a, const Index::Location& b) {
  if (a.docid < b.docid) {
    return true;
  } else if (a.docid == b.docid) {
    return a.token < b.token;
  } else {
    return false;
  }
}

bool operator>(const Index::Location& a, const Index::Location& b) {
  if (a.docid > b.docid) {
    return true;
  } else if (a.docid == b.docid) {
    return a.token > b.token;
  } else {
    return false;
  }
}

// Intersect lists a and b, storing the output in c.
// It is assumed that the lists are in sorted order, or at least that
// they have the same relative ordering of common elements.
void Index::ListIntersect(const PostingList& a,
                          const PostingList& b,
                          PostingList* c) const {
  size_t ai = 0, bi = 0;
  c->clear();
  while (ai < a.size() && bi < b.size()) {
    if (a[ai] == b[bi]) {
      c->push_back(a[ai]);
      ++ai;
      ++bi;
    } else if (a[ai] < b[bi]) {
      ++ai;
    } else {
      ++bi;
    }
  }
}

}  // namespace korpus
