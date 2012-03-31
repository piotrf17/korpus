#ifndef KORPUS_INDEX_H
#define KORPUS_INDEX_H

#include <unordered_map>
#include <vector>

namespace korpus {

class Corpus;
class ResultSet;

class Index {
 public:
  struct Location {
    int docid;
    int token;
  };
  
  typedef std::vector<Location> PostingList;
  
  Index();
  ~Index();

  void Build(const Corpus& corpus);

  bool QueryBase(const std::string& word,
                 ResultSet* result);
  
 private:
  std::unordered_map<std::string, PostingList> base_index_;
  std::unordered_map<std::string, PostingList> index_;
};

}  // namespace korpus

#endif
