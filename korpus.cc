#include <iostream>
#include <string>

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
  std::string corpus_root("/home/piotrf/data/nkjp-podkorpus/");
  std::vector<std::string> documents;
  documents.push_back("TrybunaSlaska");
  documents.push_back("Sztafeta");
  documents.push_back("WilkDom");
  documents.push_back("WilkWilczy");
  documents.push_back("ZycieINowoczesnosc");
  documents.push_back("ZycieWarszawy_Zycie");
  
  std::string error;
  korpus::Corpus corpus;
  if (!corpus.LoadFromXml(corpus_root, documents, &error)) {
    std::cout << "Error loading doc: " << error << std::endl;
    return 1;
  }
  
  korpus::Index index(corpus);
  index.Build();

  std::vector<korpus::Result> results;
  index.QueryBase("specjalny", &results);
  std::cout << results.size() << " result(s) for \"specjalny\":" << std::endl;
  for (auto it = results.begin(); it != results.end(); ++it) {
    BuildQuestion(*it);
  }

  return 0;
}
