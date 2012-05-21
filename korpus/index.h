#ifndef KORPUS_INDEX_H
#define KORPUS_INDEX_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace korpus {

class Corpus;
class Result;

class Index {
 public:
  struct Location {
    int docid;
    uint64_t token;
  };
  
  typedef std::vector<Location> PostingList;
  
  Index(const Corpus& corpus);
  ~Index();

  void Build();

  bool QueryBase(const std::string& word,
                 std::vector<Result>* results) const;
  
 private:
  Index(const Index& o);
  Index& operator=(const Index& o);
  
  const Corpus& corpus_;
  
  std::unordered_map<std::string, PostingList> base_index_;
  std::unordered_map<std::string, PostingList> index_;
};

}  // namespace korpus

#endif
