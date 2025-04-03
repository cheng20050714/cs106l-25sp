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
  // 1. Step One: Skip words that are already correctly spelled
  auto misspelled_view = std::ranges::views::filter(source, [&dictionary](const Token& token) {
    return !dictionary.contains(token.content); // 保留拼写错误的单词
  });

  // 2. Step Two: Generate suggestions for misspelled words
  std::set<Mispelling> result;
  for (const auto& token : misspelled_view) {
    std::set<std::string> suggestions;

    // 遍历字典，寻找 Damerau-Levenshtein 距离为 1 的单词
    // utils.cpp func levenshtein
    for (const auto& word : dictionary) {
      if (levenshtein(token.content, word) == 1) {
        suggestions.insert(word); // 添加拼写建议
      }
    }

    // 3. Step Three: Store the misspelled word and its suggestions
    if (!suggestions.empty()) {
      result.insert(Mispelling{token, suggestions}); // 创建 Mispelling 对象并插入结果集合
    }
  }

  return result;
}

/* Helper methods */

#include "utils.cpp"