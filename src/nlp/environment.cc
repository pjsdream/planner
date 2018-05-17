#include "nlp/phrase.h"

namespace simplan
{
namespace
{
char ToLower(char x)
{
  if ('A' <= x && x <= 'Z')
    return x - 'A' + 'a';
  return x;
}
}

Phrase::Phrase()
{
}

Phrase::~Phrase()
{
}

void Phrase::SetPosFromString(std::string pos)
{
  // To lowercase
  for (int i = 0; i < pos.length(); i++)
    pos[i] = ToLower(pos[i]);

  if (pos == "noun" || pos == "nn")
    pos_ = PosType::NOUN;
  else if (pos == "verb" || pos == "vb")
    pos_ = PosType::VERB;
  else if (pos == "adjective" || pos == "jj")
    pos_ = PosType::ADJECTIVE;
  else if (pos == "adverb" || pos == "rb")
    pos_ = PosType::ADVERB;
  else
    pos_ = PosType::UNDEFINED;
}
}
