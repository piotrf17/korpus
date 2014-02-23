#ifndef QUESTIONER_SCHEDULER_H
#define QUESTIONER_SCHEDULER_H

#include <deque>
#include <string>

#include "questioner/question.h"

namespace korpus {
class Index;
}  // namespace korpus

namespace questioner {

class Scheduler {
 public:
  Scheduler(const korpus::Index* index);
  ~Scheduler();

  void Generate(int num_new_questions,
                const std::string& grammatical_case);

  void GetQuestion(int num, std::string* question) const;
  void GetAnswer(int num, std::string* answer) const;

  int num_questions() const {
    return questions_.size();
  }
  
 private:
  void GenerateNewQuestions(
      int num_questions,
      const std::string& grammatical_case,
      const std::unordered_set<unsigned int>& current_questions,
      std::vector<Question>* new_questions);
  
  const korpus::Index* index_;
  std::deque<Question> questions_;
};

}  // namespace questioner

#endif
