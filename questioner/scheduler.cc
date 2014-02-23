#include "questioner/question_builder.h"

#include <cassert>
#include <ctime>
#include <iostream>
#include <unordered_set>

#include "korpus/index.h"
#include "korpus/lexeme.h"
#include "korpus/result.h"

namespace questioner {

Scheduler::Scheduler(const korpus::Index* index) :
    index_(index) {
  srand(time(NULL));
}

Scheduler::~Scheduler() {
}

void GenerateNewQuestions(
    int num_questions,
    const std::string& grammatical_case,
    const std::unordered_set<unsigned int>& current_questions,
    std::vector<Question>* new_questions) {
  // Query korpus for nouns in our given case.
  std::vector<korpus::Result> results;
  std::vector<std::string> query;
  query.push_back(grammatical_case);
  query.push_back(":subst");  // select nouns
  index_->Query(query, &results);
  num_questions = std::min(num_questions, static_cast<int>(results.size()));

  new_questions->resize(num_questions);
  for (int i = 0; i < num_questions; ++i) {
    // Repeat until we get a unique question.
    do {    
      int result_idx = rand() % results.size();
      (*new_questions)[i].Build(results[result_idx]);
    } while (current_questions.count((*new_questions)[i].Id()));
    current_questions.insert((*new_questions)[i].Id());
  }  
}

void Scheduler::Generate(int num_new_questions,
                         const std::string& grammatical_case) {
  std::unordered_set<unsigned int> current_questions;
  
  // Ask the deck for all questions scheduled today.
  std::vector<Question> scheduled_questions;
  time_t rawtime;
  time(&rawtime);
  deck_.GetScheduledQuestions(rawtime, &scheduled_questions, current_questions);
  for (const Question& q : scheduled_questions) {
    current_questions.insert(q.Id());
  }
  
  // Query the corpus for a set of new questions.
  std::vector<Question> new_questions;
  GenerateNewQuestions(num_new_questions, grammatical_case, &new_questions);

  // Merge the two sets.
  const int num_new = new_questions.size();
  const int num_scheduled = scheduled_questions.size();
  const int num_total = num_new + num_scheduled;
  questions_.reserve(num_total);
  if (num_scheduled == 0) {
    std::copy(questions_.begin(),
              new_questions.begin(),
              new_questions.end());
  } else if (num_new == 0) {
    std::copy(questions_.begin(),
              scheduled_questions.begin(),
              scheduled_questions.end());
  } else {
    const float new_interval = static_cast<float>(num_total) / num_new;
    auto it_new = new_questions.begin();
    auto it_scheduled = scheduled_questions.begin();
    int counter = 0;
    float next_new = 0;
    while (it_new != new_questions.end() &&
           it_scheduled != scheduled_questions.end()) {
      if (counter > next_new && it_new != new_questions.end()) {
        questions_.push_back(*it_new);
        ++it_new;
        next_new += new_interval;
      } else if (it_scheduled != scheduled_questions.end()) {
        questions_.push_back(*it_scheduled);
        ++it_scheduled;
      }
      ++counter;
    }
  }
}

void Scheduler::GetQuestion(int num, std::string* question) const {
  if (num >= 0 && num < static_cast<int>(questions_.size())) {
    questions_[num].RenderQuestion(question);
  } else {
    *question = "INVALID QUESTION INDEX";
  }
}

void Scheduler::GetAnswer(int num, std::string* answer) const {
  if (num >= 0 && num < static_cast<int>(questions_.size())) {
    questions_[num].RenderAnswer(answer);
  } else {
    *answer = "INVALID QUESTION INDEX";
  }
}  

}  // namespace questioner
