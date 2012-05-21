#include "questioner/question_builder.h"

#include "korpus/index.h"

namespace questioner {

QuestionBuilder::QuestionBuilder(const korpus::Index* index) :
    index_(index) {
}

QuestionBuilder::~QuestionBuilder() {
}

void QuestionBuilder::GetQuestion(std::string* question) {
  *question = "doof";
}

}  // namespace questioner
