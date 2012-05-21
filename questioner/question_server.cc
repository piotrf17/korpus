#include "questioner/question_server.h"

#include <functional>
#include <iostream>

#include "korpus/corpus.h"
#include "korpus/index.h"

namespace questioner {

QuestionServer::QuestionServer(int port) :
    HTTPServer(port),
    corpus_(NULL),
    index_(NULL) {
  handlers_["/"] =
      std::bind(&QuestionServer::ServeFrontpage, this,
                std::placeholders::_1, std::placeholders::_2);
  handlers_["/index.html"] =
      std::bind(&QuestionServer::ServeFrontpage, this,
                std::placeholders::_1, std::placeholders::_2);
  handlers_["/lookup"] =
      std::bind(&QuestionServer::ServeLookup, this,
                std::placeholders::_1, std::placeholders::_2);
  handlers_["/practice"] =
      std::bind(&QuestionServer::ServePractice, this,
                std::placeholders::_1, std::placeholders::_2);
}

QuestionServer::~QuestionServer() {
  delete corpus_;
  delete index_;
}

bool QuestionServer::LoadCorpus(const std::string& filename) {
  std::cout << "Loading corpus..." << std::endl;
  corpus_ = new korpus::Corpus();
  if (!corpus_->LoadFromBinary(filename)) {
    return false;
  }
  std::cout << "Done!" << std::endl;

  std::cout << "Building index..." << std::endl;
  index_ = new korpus::Index(*corpus_);
  index_->Build();
  std::cout << "Done!" << std::endl;
  
  return true;
}

void QuestionServer::ServeFrontpage(
    const std::string& query,
    std::string* output) {
  const std::string kFrontPageHtml =
      "<html>"
      "<head>"
      "  <title>Polish Declension Trainer</title>"
      "</head>"
      "<body>"
      "  <a href=\"practice\">Start practicing!</a>"
      "</body>"
      "</html>";
  *output = kFrontPageHtml;
}

void QuestionServer::ServeLookup(
    const std::string& query,
    std::string* output) {
  const std::string kNotImplemented =
      "<html>"
      "<head>"
      "  <title>Polish Declension Trainer</title>"
      "</head>"
      "<body>"
      "  <p>Not implemented yet!</p>"
      "</body>"
      "</html>";
  *output = kNotImplemented;
}

void QuestionServer::ServePractice(
    const std::string& query,
    std::string* output) {
  const std::string header =
      "<html>"
      "<head>"
      "  <title>Practice</title>"
      "</head>"
      "<body>";
  const std::string footer =
      "</body>"
      "</html>";

  if (question_builder_.get() == NULL) {
    question_builder_.reset(new QuestionBuilder(index_));
  }

  std::string question;
  question_builder_->GetQuestion(&question);
  
  *output = header + question + footer;
}

void QuestionServer::HandleRequest(const std::string& uri,
                                   const std::string& query,
                                   std::string* output) {
  const std::string kNotFoundHtml =
      "<html>"
      "<head>"
      "  <title>Not found!</title>"
      "</head>"
      "<body>"
      "  <p>Page not found!!</p>"
      "</body>"
      "</html>";

  std::cout << "Request " << uri << " " << query << std::endl;

  if (handlers_.find(uri) != handlers_.end()) {
    handlers_[uri](query, output);
  } else {
    *output = kNotFoundHtml;
  }
}

}  // namespace questioner
