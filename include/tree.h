// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <cstdint>
#include <vector>

struct PermNode {
  char val;
  std::vector<PermNode*> kids;

  explicit PermNode(char ch) : val(ch) {}

  ~PermNode() {
    for (auto kid : kids) {
      delete kid;
    }
  }
};

class PermutationGenerator {
 private:
  PermNode* head;

  void expand(PermNode* cur, std::vector<char>& rest);

 public:
  explicit PermutationGenerator(std::vector<char> items);
  ~PermutationGenerator();

  PermNode* getHead() const { return head; }
};

std::vector<std::vector<char>> extractAll(PermutationGenerator& gen);

std::vector<char> extractByNumLinear(PermutationGenerator& gen, int num);

std::vector<char> extractByNumDirect(PermutationGenerator& gen, int num);

#endif  // INCLUDE_TREE_H_
