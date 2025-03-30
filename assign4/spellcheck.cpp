#include "spellcheck.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

Corpus tokenize(const std::string& source) {
  //1.Step One: Identify all iterators to space characters
  auto whitespace_iters = find_all(source.begin(), source.end(), isspace);
  
  Corpus tokens;
  
  //2.Step Two: Generate tokens between consecutive space characters
  std::transform(
      whitespace_iters.begin(), whitespace_iters.end() - 1,
      whitespace_iters.begin() + 1,
      std::inserter(tokens, tokens.end()),
      [&source](auto it1, auto it2)->Token {
          return Token(source, it1, it2);
      }
  );
  
  //3.Step Three: Get rid of empty tokens
  std::erase_if(tokens, [](const Token& t) { return t.content.empty(); });
  return tokens;
}

std::set<Mispelling> spellcheck(const Corpus& source, const Dictionary& dictionary) {
  /* TODO: Implement this method */
  return std::set<Mispelling>();
};

/* Helper methods */

#include "utils.cpp"