// Copyright 2022 NNTU-CS
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

#include "../include/tree.h"

void display(const std::vector<char>& seq) {
  for (char ch : seq) {
    std::cout << ch;
  }
  std::cout << std::endl;
}

void displayAll(const std::vector<std::vector<char>>& sequences) {
  for (const auto& seq : sequences) {
    display(seq);
  }
}

int getRandomIndex(int maxVal) {
  static std::random_device rd;
  static std::mt19937 generator(rd());
  std::uniform_int_distribution<> dist(1, maxVal);
  return dist(generator);
}

int64_t factValue(int n) {
  if (n <= 1) return 1;
  int64_t out = 1;
  for (int i = 2; i <= n; ++i) {
    out *= i;
  }
  return out;
}

std::vector<char> buildAlphabet(int size) {
  std::vector<char> out;
  for (int i = 0; i < size; ++i) {
    out.push_back('0' + i);
  }
  return out;
}

int main() {
  std::cout << "=== Permutation Tree Demo ===\n\n";

  std::cout << "--- Task 1: Build Tree ---\n";
  std::vector<char> source = {'1', '2', '3'};
  PermutationGenerator generator(source);
  std::cout << "Tree constructed for: ";
  for (char c : source) std::cout << c << " ";
  std::cout << "\n\n";

  std::cout << "--- Task 2: GetAllPermutations ---\n";
  auto allPerms = extractAll(generator);
  std::cout << "All permutations of {1,2,3}:\n";
  displayAll(allPerms);
  std::cout << "Total count: " << allPerms.size() << "\n\n";

  std::cout << "--- Task 3: Get permutation by number ---\n";
  std::cout << "Perm #1 (linear): ";
  display(extractByNumLinear(generator, 1));
  std::cout << "Perm #2 (direct): ";
  display(extractByNumDirect(generator, 2));
  std::cout << "Perm #6 (linear): ";
  display(extractByNumLinear(generator, 6));
  std::cout << "Perm #8 (direct, invalid): ";
  std::vector<char> emptyResult = extractByNumDirect(generator, 8);
  if (emptyResult.empty()) {
    std::cout << "(empty vector)" << std::endl;
  } else {
    display(emptyResult);
  }
  std::cout << "\n";

  std::cout << "--- Task 4: Performance Experiment ---\n";
  std::cout << "Measuring execution time vs alphabet size\n\n";

  std::ofstream timingOutput("result/timing_data.txt");
  timingOutput << "# n\t\tgetAllPerms(us)\tgetPerm1(us)\tgetPerm2(us)\n";

  for (int sz = 3; sz <= 10; ++sz) {
    std::cout << "Testing n=" << sz << "... ";
    std::cout.flush();
    
    std::vector<char> alphabet = buildAlphabet(sz);

    PermutationGenerator genAll(alphabet);
    auto start = std::chrono::high_resolution_clock::now();
    auto permsAll = extractAll(genAll);
    auto end = std::chrono::high_resolution_clock::now();
    long timeAll = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    PermutationGenerator genLinear(alphabet);
    int total = static_cast<int>(permsAll.size());
    int rndNum = getRandomIndex(total);
    start = std::chrono::high_resolution_clock::now();
    extractByNumLinear(genLinear, rndNum);
    end = std::chrono::high_resolution_clock::now();
    long timeLinear = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    PermutationGenerator genDirect(alphabet);
    rndNum = getRandomIndex(total);
    start = std::chrono::high_resolution_clock::now();
    extractByNumDirect(genDirect, rndNum);
    end = std::chrono::high_resolution_clock::now();
    long timeDirect = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    timingOutput << sz << "\t\t" << timeAll << "\t\t" << timeLinear << "\t\t" << timeDirect << "\n";
    std::cout << "OK (getAllPerms: " << timeAll << " us)\n";
  }
  
  timingOutput.close();
  std::cout << "\nData saved to result/timing_data.txt\n";
  return 0;
}
