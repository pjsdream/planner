#ifndef SIMPLAN_NLP_PHRASE_H_
#define SIMPLAN_NLP_PHRASE_H_

#include <string>

namespace simplan
{
class Phrase
{
public:
  enum class PosType
  {
    UNDEFINED,
    VERB,
    NOUN,
    ADJECTIVE,
    ADVERB,
  };

public:
  Phrase();
  ~Phrase();

  void SetPhrase(const std::string& phrase)
  {
    phrase_ = phrase;
  }

  void SetPosFromString(std::string pos);

private:
  std::string phrase_;
  PosType pos_ = PosType::UNDEFINED;
};
}

#endif // SIMPLAN_NLP_PHRASE_H_
