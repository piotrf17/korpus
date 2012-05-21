#ifndef QUESTIONER_QUESTION_SERVER_H
#define QUESTIONER_QUESTION_SERVER_H

#include <map>
#include <memory>

#include <boost/function.hpp>

#include "http_server.h"
#include "question_builder.h"

namespace korpus {
class Corpus;
class Index;
}

namespace questioner {

class QuestionServer : public HTTPServer{
 public:
  QuestionServer(int port);
  ~QuestionServer();

  bool LoadCorpus(const std::string& filename);

 protected:
  virtual void HandleRequest(const std::string& uri,
                             const std::string& query,
                             std::string* output);
  
 private:

  void ServeFrontpage(const std::string& query,
                      std::string* output);
  void ServeLookup(const std::string& query,
                   std::string* output);
  void ServePractice(const std::string& query,
                     std::string* output);

  typedef boost::function<
      void (const std::string&, std::string*)> RequestHandler;
  std::map<std::string, RequestHandler> handlers_;

  std::unique_ptr<QuestionBuilder> question_builder_;

  korpus::Corpus* corpus_;
  korpus::Index* index_;
};
  
}  // namespace questioner

#endif
