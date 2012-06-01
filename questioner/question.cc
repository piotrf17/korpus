#include "questioner/question.h"

#include "korpus/lexeme.h"
#include "korpus/result.h"

namespace questioner {

Question::Question() {
}

Question::~Question() {
}

void Question::Build(const korpus::Result& result) {
  int pos = result.GetContext(&words_);

  // Obscure words of the same case next to our word.
  // TODO(piotrf): make this general based on the case of our result.
  hidden_.resize(words_.size(), false);
  hidden_[pos] = true;
  for (int i = pos - 1; i >= 0; --i) {
    if (words_[i]->attributes().count("gen") &&
        !words_[i]->attributes().count("prep")) {
      hidden_[i] = true;
    } else {
      break;
    }
  }
  for (unsigned int i = pos + 1; i < words_.size(); ++i) {
    if (words_[i]->attributes().count("gen") &&
        !words_[i]->attributes().count("prep")) {
      hidden_[i] = true;
    } else {
      break;
    }
  }
}

void Question::RenderQuestion(std::string* out) const {
  out->clear();
  for (unsigned int i = 0; i < words_.size(); ++i) {
    if (hidden_[i]) {
      *out += "<span class=\"qword\">(" + words_[i]->base();
      if (words_[i]->attributes().count("pl")) {
        *out += " pl";
      }
      *out += ")</span> ";
    } else {
      *out += words_[i]->value() + " ";
    }
  }
}

void Question::RenderAnswer(std::string* out) const {
  out->clear();
  for (unsigned int i = 0; i < words_.size(); ++i) {
    if (hidden_[i]) {
      *out += "<span class=\"aword\">" +
          words_[i]->value() +
          "</span> ";
    } else {
      *out += words_[i]->value() + " ";
    }
  }

}

}  // namespace questioner
