#include <iostream>
#include <string>

#include "corpus.h"
#include "index.h"

int main(int argc, char** argv) {
  std::string corpus_root("/home/piotrf/projects/polish/nkjp-podkorpus/");
  std::vector<std::string> documents;
  documents.push_back("Sztafeta");
  documents.push_back("WilkDom");
  documents.push_back("WilkWilczy");
  
  std::string error;
  korpus::Corpus corpus;
  if (!corpus.LoadFromXml(corpus_root, documents, &error)) {
    std::cout << "Error loading doc: " << error << std::endl;
    return 1;
  }

  korpus::Index index;
  index.Build(corpus);

  return 0;
}
