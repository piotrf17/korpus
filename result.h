#ifndef KORPUS_RESULT_H
#define KORPUS_RESULT_H

#include <cstdint>
#include <string>
#include <vector>

namespace korpus {

class Corpus;
class Lexeme;  
  
class Result {
 public:
  Result(const Corpus* corpus,
         const int docid,
         const uint64_t token);

  int GetContext(std::vector<Lexeme*>* context) const;

  const std::string& base() const;
  const std::string& value() const;
  
 private:
  const Corpus* corpus_;
  int docid_;
  uint64_t token_;
  const Lexeme* lexeme_;
};

}  // namespace korpus

#endif
