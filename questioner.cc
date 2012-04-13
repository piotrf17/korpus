#include <iostream>

#include "corpus.h"
#include "index.h"
#include "lexeme.h"
#include "result.h"

void BuildQuestion(const korpus::Result& result) {
  std::vector<korpus::Lexeme*> context;
  unsigned int pos = result.GetContext(&context);

  for (unsigned int i = 0; i < context.size(); ++i) {
    if (i != pos) {
      std::cout << context[i]->value() << " ";
    } else {
      std::cout << "____________ ";
    }
  }
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  std::string corpus_path = "/home/piotrf/data/nkjp-podkorpus/test_corpus.bin";
  
  korpus::Corpus corpus;
  if (!corpus.LoadFromBinary(corpus_path)) {
    std::cout << "Error loading corpus " << corpus_path << std::endl;
    return 1;
  }

  korpus::Index index(corpus);
  index.Build();

  std::vector<korpus::Result> results;
  index.QueryBase("pan", &results);
  std::cout << results.size() << " result(s) for \"pan\":" << std::endl;
  for (auto it = results.begin(); it != results.end(); ++it) {
    BuildQuestion(*it);
  }
  
  return 0;
}
