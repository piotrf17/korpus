#include <gtest/gtest.h>
#include <fstream>
#include <memory>

#include "korpus/lexeme.h"

namespace korpus {

TEST(Lexeme, ReadWrite) {
  Lexeme l("pażdzierniku", "pażdziernik:a:b22:c3:d4");
  
  std::ofstream outfile("/tmp/lexeme_test", std::ofstream::binary);
  l.SaveToBinary(&outfile);
  outfile.close();

  std::ifstream infile("/tmp/lexeme_test", std::ifstream::binary);
  std::unique_ptr<Lexeme> l2(Lexeme::FromBinary(&infile));
  infile.close();

  ASSERT_EQ("pażdzierniku", l2->value());
  ASSERT_EQ("pażdziernik", l2->base());
}

}
