#ifndef QUESTIONER_QUESTION_H
#define QUESTIONER_QUESTION_H

#include <string>
#include <vector>

namespace korpus {
  class Lexeme;
  class Result;
}  // namespace korpus

namespace questioner {

class Question {
 public:
  Question();
  ~Question();

  void Build(const korpus::Result& result);

  void RenderQuestion(std::string* out) const;  
  void RenderAnswer(std::string* out) const;
  
 private:
  std::vector<korpus::Lexeme*> words_;
  std::vector<bool> hidden_;
};

} // namespace questioner

#endif
