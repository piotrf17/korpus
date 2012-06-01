#ifndef QUESTIONER_QUESTION_BUILDER_H
#define QUESTIONER_QUESTION_BUILDER_H

#include <string>

#include "questioner/question.h"

namespace korpus {
class Index;
}  // namespace korpus

namespace questioner {

class QuestionBuilder {
 public:
  QuestionBuilder(const korpus::Index* index);
  ~QuestionBuilder();

  void Generate(int num_questions);

  void GetQuestion(int num, std::string* question) const;
  void GetAnswer(int num, std::string* answer) const;
  
 private:
  const korpus::Index* index_;
  std::vector<Question> questions_;
};

}  // namespace questioner

#endif
