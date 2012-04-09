#include <iostream>
#include <string>

#include "corpus.h"
#include "index.h"

int main(int argc, char** argv) {
  std::string corpus_root("/home/piotrf/projects/polish/nkjp-podkorpus/");
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

  korpus::Index index;
  index.Build(corpus);

  index.QueryBase("być", NULL);
  index.QueryBase("ten", NULL);
  index.QueryBase("czerwony", NULL);

  return 0;
}
