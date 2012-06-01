#include "questioner/question_builder.h"

#include <cassert>
#include <iostream>

#include "korpus/index.h"
#include "korpus/lexeme.h"
#include "korpus/result.h"

namespace questioner {

QuestionBuilder::QuestionBuilder(const korpus::Index* index) :
    index_(index) {
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

  // TODO(piotrf): check that we actually have num_questions
  // results!
  
  questions_.resize(num_questions);
  for (int i = 0; i < num_questions; ++i) {
    // Choose a random result to make a question.
    // TODO(piotrf): dedup between questions.
    int result_idx = rand() % results.size();
    questions_[i].Build(results[result_idx]);
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
