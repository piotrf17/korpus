#include "questioner/question_builder.h"

#include <iostream>

#include "korpus/index.h"
#include "korpus/result.h"

namespace questioner {

QuestionBuilder::QuestionBuilder(const korpus::Index* index) :
    index_(index) {
}

QuestionBuilder::~QuestionBuilder() {
}

void QuestionBuilder::GetQuestion(std::string* question) {
  std::vector<korpus::Result> results;
  std::vector<std::string> query;
  query.push_back(":gen");
  query.push_back(":subst");
  index_->Query(query, &results);

  std::cout << "Found " << results.size() << " results.\n" << std::endl;
  
  *question = "";
  for (const korpus::Result& result : results) {
    *question = *question + result.base() + " " + result.value() + "<br>";
  }
}

}  // namespace questioner
