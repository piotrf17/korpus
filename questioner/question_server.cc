#include "questioner/question_server.h"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <ctemplate/template.h>

#include "korpus/corpus.h"
#include "korpus/index.h"

namespace questioner {

QuestionServer::QuestionServer(int port) :
    HTTPServer(port),
    corpus_(NULL),
    index_(NULL),
    template_root_("../questioner/html_templates/") {
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
  ctemplate::TemplateDictionary dict("index");
  ctemplate::ExpandTemplate(template_root_ + "index.tpl",
                            ctemplate::DO_NOT_STRIP,
                            &dict,
                            output);

}

void QuestionServer::ServeLookup(
    const std::string& query,
    std::string* output) {
  const std::string kNotImplemented =
      "<html>"
      "<head>"
      "  <title>Polish Declension Trainer</title>"
      "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
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
  if (question_builder_.get() == NULL) {
    question_builder_.reset(new QuestionBuilder(index_));
    question_builder_->Generate(20);
  }

  std::unordered_map<std::string,std::string> keys;
  ParseQuery(query, &keys);
  int question_num = 0;
  if (keys.count("q")) {
    question_num = atoi(keys["q"].c_str());
  }

  ctemplate::TemplateDictionary dict("practice");

  std::string question;
  question_builder_->GetQuestion(question_num, &question);
  dict.SetValue("QUESTION", question);

  std::string answer;
  question_builder_->GetAnswer(question_num, &answer);
  dict.SetValue("ANSWER", answer);

  dict.SetIntValue("NEXTQ", question_num + 2);
  dict.SetIntValue("CURQ", question_num + 1);
  dict.SetIntValue("TOTALQ", 20);

  ctemplate::ExpandTemplate(template_root_ + "practice.tpl",
                            ctemplate::DO_NOT_STRIP,
                            &dict,
                            output);
}

void QuestionServer::HandleRequest(const std::string& uri,
                                   const std::string& query,
                                   std::string* output) {
  const std::string kNotFoundHtml =
      "<html>"
      "<head>"
      "  <title>Not found!</title>"
      "  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"
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
