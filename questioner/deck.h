// A Deck is a collection of question/answers and statistics about
// when they were asked, the current difficulty level, etc.
// A Deck mantains synchronization with a disk serialization.

#ifndef QUESTIONER_DECK_H
#define QUESTIONER_DECK_H

namespace questioner {

class Deck {
 public:
  Deck();
  ~Deck();

  bool LoadDeck(const std::string& filename);
  
  void LogQuestion(const Question& question,
                   int difficulty);
  
 private:
};

}  // namespace questioner

#endif
