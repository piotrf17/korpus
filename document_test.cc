#include <fstream>
#include <gtest/gtest.h>

#include "document.h"
#include "lexeme.h"

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

TEST(StaticDocument, EncodeDecode) {
  EncodeDecodeTestCase(0, 0, 0);
  EncodeDecodeTestCase(0, 0, 1);
  EncodeDecodeTestCase(0, 0, 1000);
  EncodeDecodeTestCase(0, 1, 1);
  EncodeDecodeTestCase(0, 22, 1000);
  EncodeDecodeTestCase(1, 1, 1);
  EncodeDecodeTestCase(22, 22, 1000);
  EncodeDecodeTestCase(22, 1<22, 1000);
}

class DocumentTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    std::string path("../test_data");
    std::string error;
    doc_.LoadFromXml(path, &error);
  }

  Document doc_;
};

TEST_F(DocumentTest, ReadWrite) {
  std::ofstream outfile("/tmp/doc_test", std::ofstream::binary);
  doc_.SaveToBinary(&outfile);
  outfile.close();

  Document doc2;
  std::ifstream infile("/tmp/doc_test", std::ifstream::binary);
  doc2.LoadFromBinary(&infile);
  infile.close();

  DocumentIterator it1(doc_);
  DocumentIterator it2(doc2);
  for (; !it1.end(); ++it1, ++it2) {
    ASSERT_FALSE(it2.end());
    ASSERT_EQ(it1->value(), it2->value());
    ASSERT_EQ(it1->base(), it2->base());
    ASSERT_EQ(it1.token(), it2.token());
  }
}

}  // namespace korpus
