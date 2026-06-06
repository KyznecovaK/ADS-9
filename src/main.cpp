// Copyright 2022 NNTU-CS
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "../include/tree.h"

void show(const std::vector<char>& seq) {
  for (char ch : seq) {
    std::cout << ch;
  }
  std::cout << std::endl;
}

void showAll(const std::vector<std::vector<char>>& sequences) {
  for (const auto& seq : sequences) {
    show(seq);
  }
}

int getRandomIndex(int maxVal) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(1, maxVal);
  return dist(gen);
}

int64_t factVal(int n) {
  if (n <= 1) return 1;
  int64_t out = 1;
  for (int i = 2; i <= n; ++i) {
    out *= i;
  }
  return out;
}

std::vector<char> makeAlphabet(int sz) {
  std::vector<char> out;
  for (int i = 0; i < sz; ++i) {
    out.push_back('0' + i);
  }
  return out;
}

int main() {
  std::cout << "=== Permutation Tree Demo ===\n\n";

  std::cout << "--- Task 1: Build Tree ---\n";
  std::vector<char> src = {'1', '2', '3'};
  PMTree tree(src);
  std::cout << "Tree built for: ";
  for (char c : src) std::cout << c << " ";
  std::cout << "\n\n";

  std::cout << "--- Task 2: GetAllPermutations ---\n";
  auto all = getAllPerms(tree);
  std::cout << "All permutations of {1,2,3}:\n";
  showAll(all);
  std::cout << "Count: " << all.size() << "\n\n";

  std::cout << "--- Task 3: Get permutation by number ---\n";
  std::cout << "Perm #1 (slow): ";
  show(getPerm1(tree, 1));
  std::cout << "Perm #2 (fast): ";
  show(getPerm2(tree, 2));
  std::cout << "Perm #6 (slow): ";
  show(getPerm1(tree, 6));
  std::cout << "Perm #8 (fast, invalid): ";
  std::vector<char> emptyRes = getPerm2(tree, 8);
  if (emptyRes.empty()) {
    std::cout << "(empty)\n";
  } else {
    show(emptyRes);
  }
  std::cout << "\n";

  std::cout << "--- Task 4: Performance Experiment ---\n";
  std::cout << "Measuring time vs alphabet size\n\n";

  std::ofstream outFile("result/timing_data.txt");
  outFile << "# n\t\tgetAllPerms(us)\tgetPerm1(us)\tgetPerm2(us)\n";

  for (int sz = 3; sz <= 10; ++sz) {
    std::cout << "Testing n=" << sz << "... ";
    std::cout.flush();

    std::vector<char> alphabet = makeAlphabet(sz);

    PMTree tAll(alphabet);
    auto start = std::chrono::high_resolution_clock::now();
    auto permsAll = getAllPerms(tAll);
    auto end = std::chrono::high_resolution_clock::now();
    int64_t timeAll = std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();

    PMTree tSlow(alphabet);
    int total = static_cast<int>(permsAll.size());
    int rnd = getRandomIndex(total);
    start = std::chrono::high_resolution_clock::now();
    getPerm1(tSlow, rnd);
    end = std::chrono::high_resolution_clock::now();
    int64_t timeSlow = std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();

    PMTree tFast(alphabet);
    rnd = getRandomIndex(total);
    start = std::chrono::high_resolution_clock::now();
    getPerm2(tFast, rnd);
    end = std::chrono::high_resolution_clock::now();
    int64_t timeFast = std::chrono::duration_cast<
        std::chrono::microseconds>(end - start).count();

    outFile << sz << "\t\t" << timeAll << "\t\t"
            << timeSlow << "\t\t" << timeFast << "\n";
    std::cout << "OK (getAllPerms: " << timeAll << " us)\n";
  }

  outFile.close();
  std::cout << "\nData saved to result/timing_data.txt\n";
  return 0;
}
