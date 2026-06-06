// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <cstdint>
#include <vector>

struct PMNode {
  char val;
  std::vector<PMNode*> kids;

  explicit PMNode(char ch) : val(ch) {}

  ~PMNode() {
    for (auto kid : kids) {
      delete kid;
    }
  }
};

class PMTree {
 private:
  PMNode* root;

  void expand(PMNode* cur, std::vector<char>& rest);

 public:
  explicit PMTree(std::vector<char> items);
  ~PMTree();

  PMNode* getRoot() const { return root; }
};

std::vector<std::vector<char>> getAllPerms(PMTree& tree);

std::vector<char> getPerm1(PMTree& tree, int num);

std::vector<char> getPerm2(PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
