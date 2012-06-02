#include "questioner/question_builder.h"

#include <cassert>
#include <iostream>
#include <time.h>
#include <unordered_set>

#include "korpus/index.h"
#include "korpus/lexeme.h"
#include "korpus/result.h"

namespace questioner {

QuestionBuilder::QuestionBuilder(const korpus::Index* index) :
    index_(index) {
  srand(time(NULL));
}

QuestionBuilder::~QuestionBuilder() {
}

void QuestionBuilder::Generate(int num_questions) {
  // Query korpus for nouns in the genitive case.
  // TODO(piotrf): make this query parameter controlled.
  std::vector<korpus::Result> results;
  std::vector<std::string> query;
  query.push_back(":gen");
  query.push_back(":subst");
  index_->Query(query, &results);

  num_questions = std::min(num_questions, static_cast<int>(results.size()));

  questions_.resize(num_questions);
  std::unordered_set<unsigned int> current_questions;
  for (int i = 0; i < num_questions; ++i) {
    // Repeat until we get a unique question.
    do {    
      int result_idx = rand() % results.size();
      questions_[i].Build(results[result_idx]);
    } while (current_questions.count(questions_[i].Id()));
    current_questions.insert(questions_[i].Id());
  }
}

void QuestionBuilder::GetQuestion(int num, std::string* question) const {
  if (num >= 0 && num < static_cast<int>(questions_.size())) {
    questions_[num].RenderQuestion(question);
  } else {
    *question = "INVALID QUESTION INDEX";
  }
}

void QuestionBuilder::GetAnswer(int num, std::string* answer) const {
  if (num >= 0 && num < static_cast<int>(questions_.size())) {
    questions_[num].RenderAnswer(answer);
  } else {
    *answer = "INVALID QUESTION INDEX";
  }
}  

}  // namespace questioner
