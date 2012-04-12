#include <gtest/gtest.h>
#include "document.h"

namespace korpus {

void EncodeDecodeTestCase(int a, int b, int c) {
  int section, sentence, lexeme;
  uint64_t token;

  section = a; sentence = b; lexeme = c;
  Document::EncodeToken(a, b, c, &token);
  Document::DecodeToken(token, &section, &sentence, &lexeme);
  ASSERT_EQ(a, section);
  ASSERT_EQ(b, sentence);
  ASSERT_EQ(c, lexeme);
}

TEST(DocumentTest, EncodeDecode) {
  EncodeDecodeTestCase(0, 0, 0);
  EncodeDecodeTestCase(0, 0, 1);
  EncodeDecodeTestCase(0, 0, 1000);
  EncodeDecodeTestCase(0, 1, 1);
  EncodeDecodeTestCase(0, 22, 1000);
  EncodeDecodeTestCase(1, 1, 1);
  EncodeDecodeTestCase(22, 22, 1000);
  EncodeDecodeTestCase(22, 1<22, 1000);
}

}  // namespace korpus
