#ifndef QUESTIONER_QUESTION_BUILDER_H
#define QUESTIONER_QUESTION_BUILDER_H

#include <string>

namespace korpus {
class Index;
}  // namespace korpus

namespace questioner {

class QuestionBuilder {
 public:
  QuestionBuilder(const korpus::Index* index);
  ~QuestionBuilder();

  void GetQuestion(std::string* question);
  
 private:
  const korpus::Index* index_;
};

}  // namespace questioner

#endif
