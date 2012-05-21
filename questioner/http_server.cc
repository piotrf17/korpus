#include "http_server.h"

#include <cstring>
#include <cstdio>

#include "mongoose/mongoose.h"

namespace questioner {

// TERRIBLE HACK FOLLOWS
// Mongoose expects a C callback.  Out of laziness, I have the instance
// object address stored and this function simply hops there.  This means
// you can only have 1 HTTPServer, sorry!  A real solution would require
// thunking or some such magic.
static HTTPServer* hacky_singleton_server = NULL;
static void *callback(enum mg_event event,
                      struct mg_connection *conn,
                      const struct mg_request_info *request_info) {
  static char* a_ok = strdup("");
  if (hacky_singleton_server->Serve(event, conn, request_info)) {
    return a_ok;
  } else {
    return NULL;
  }
}

HTTPServer::HTTPServer(int port) :
    port_(port),
    ctx_(NULL) {
  hacky_singleton_server = this;
}

HTTPServer::~HTTPServer() {
}

void HTTPServer::Start() {
  char buf[22];
  snprintf(buf, 22, "%20d", port_);
  const char *options[] = {"listening_ports", buf, NULL};
  ctx_ = reinterpret_cast<mg_context*>(mg_start(&callback, NULL, options));
}

void HTTPServer::Stop() {
  if (ctx_) {
    mg_stop(ctx_);
    ctx_ = NULL;
  }
}

// Returns true if we finished processing the request, else false.
bool HTTPServer::Serve(int event,
                       struct mg_connection *conn,
                       const struct mg_request_info *request_info) {
  if (event == MG_NEW_REQUEST) {
    std::string uri;
    if (request_info->uri) {
      uri.assign(request_info->uri);
    }
    std::string query;
    if (request_info->query_string) {
      query.assign(request_info->query_string);
    }
    std::string output;    
    HandleRequest(uri,
                  query,
                  &output);
    mg_printf(conn, "HTTP/1.1 200 OK\r\n"
              "Content-Type: text/html\r\n\r\n"
              "%s", output.c_str());
    return true;
  } else {
   return false;
  }  
}

}  // namespace questioner
