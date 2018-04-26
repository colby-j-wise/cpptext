#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>

#include <boost/filesystem.hpp>

class Processor {
  public:
    Processor(const std::string input_path, const std::string output_path, const size_t num_threads);
    void saveDictionary();
    void saveFileIdMap();
    void saveFileWordCounts();
    void saveWordCounts();
    void saveSearchIndex();
    void buildSearchIndex();
    std::vector<std::string> search(std::string query);
    void process();

  private:
    std::string input_path;
    std::string output_path;
    size_t num_threads;
    std::vector<boost::filesystem::path> files;
    // full dictionary
    std::set<std::string> dictionary;
    // map from file name to map of word to count
    std::unordered_map< std::string, std::unordered_map<std::string, int>> file_word_count_map;
    // total word count map
    std::unordered_map<std::string, int> word_counts;
    // map from file id to filename
    std::map<int, std::string> file_id_map;
    // search index
    // "computer" -> [(count, fileId), (count, fileId), ...]
    std::map<std::string, std::multimap<long, long, std::greater<long>>> index;
};
