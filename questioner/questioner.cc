#include <iostream>
#include "questioner/question_server.h"

int main(int argc, char** argv) {
  questioner::QuestionServer server(8022);

  std::string corpus_filename =
      "/home/piotrf/data/nkjp-podkorpus/test_corpus.bin";
  if (!server.LoadCorpus(corpus_filename)) {
    std::cout << "Failed to load corpus at " << corpus_filename << std::endl;
    return 1;
  }
  
  server.Start();
  std::cout << "========================================\n\n";
  std::cout << "        Question Server READY\n\n";
  std::cout << "========================================\n";
  
  getchar();
  std::cout << "Shutting down...\n";
  server.Stop();
  std::cout << "Bye!\n";
  
  return 0;
}
