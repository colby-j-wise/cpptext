#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/filesystem.hpp>

class Processor {
  public:
    Processor(const std::string data_path, const std::string output_path, const size_t num_threads);
    void getWordCounts();
    bool notPunctuation(std::string word);
    void printWordCounts();

  private:
    std::string data_path;
    std::string output_path;
    size_t num_threads;
    
    std::vector<boost::filesystem::path> files;
    std::unordered_map< std::string, std::unordered_map<std::string, int>> word_count_map;
    
};
