// Copyright 2022 NNTU-CS
#include <cstdint>
#include <vector>

#include "../include/tree.h"

void PermutationGenerator::expand(PermNode* cur, std::vector<char>& rest) {
  if (rest.empty()) {
    return;
  }
  for (size_t pos = 0; pos < rest.size(); ++pos) {
    PermNode* newborn = new PermNode(rest[pos]);
    cur->kids.push_back(newborn);
    std::vector<char> leftover;
    for (size_t idx = 0; idx < rest.size(); ++idx) {
      if (pos != idx) {
        leftover.push_back(rest[idx]);
      }
    }
    expand(newborn, leftover);
  }
}

PermutationGenerator::PermutationGenerator(std::vector<char> items) {
  head = new PermNode('\0');
  expand(head, items);
}

PermutationGenerator::~PermutationGenerator() {
  delete head;
}

void walk(PermNode* node, std::vector<char>& acc,
          std::vector<std::vector<char>>& collector) {
  if (node->kids.empty() && node->val != '\0') {
    collector.push_back(acc);
    return;
  }
  for (auto kid : node->kids) {
    acc.push_back(kid->val);
    walk(kid, acc, collector);
    acc.pop_back();
  }
}

std::vector<std::vector<char>> extractAll(PermutationGenerator& gen) {
  std::vector<std::vector<char>> collector;
  std::vector<char> acc;
  walk(gen.getHead(), acc, collector);
  return collector;
}

std::vector<char> extractByNumLinear(PermutationGenerator& gen, int num) {
  std::vector<std::vector<char>> all = extractAll(gen);
  if (num < 1 || num > static_cast<int>(all.size())) {
    return std::vector<char>();
  }
  return all[num - 1];
}

static int64_t factorialCalc(int n) {
  if (n <= 1) return 1;
  int64_t res = 1;
  for (int i = 2; i <= n; ++i) {
    res *= i;
  }
  return res;
}

std::vector<char> extractByNumDirect(PermutationGenerator& gen, int num) {
  std::vector<std::vector<char>> all = extractAll(gen);
  if (num < 1 || num > static_cast<int>(all.size())) {
    return std::vector<char>();
  }
  std::vector<char> result;
  PermNode* cur = gen.getHead();
  int remaining = num - 1;
  while (!cur->kids.empty()) {
    int branches = static_cast<int>(cur->kids.size());
    int step = factorialCalc(branches - 1);
    int chosen = remaining / step;
    if (chosen >= branches) {
      return std::vector<char>();
    }
    cur = cur->kids[chosen];
    result.push_back(cur->val);
    remaining = remaining % step;
  }
  return result;
}
