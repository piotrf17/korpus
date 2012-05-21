#ifndef QUESTIONER_HTTP_SERVER_H
#define QUESTIONER_HTTP_SERVER_H

#include <string>

struct mg_connection;
struct mg_context;
struct mg_request_info;

namespace questioner {
  
class HTTPServer {
 public:
  HTTPServer(int port);
  ~HTTPServer();

  void Start();
  void Stop();

  // public out of laziness, do not call
  bool Serve(int event,
             struct mg_connection *conn,
             const struct mg_request_info *request_info);

 protected:
  virtual void HandleRequest(const std::string& uri,
                             const std::string& query,
                             std::string* output) = 0;
  
 private:

  int port_;
  struct mg_context* ctx_;
};

}  // namespace questioner

#endif
