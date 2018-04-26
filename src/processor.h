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
    void saveFileWordCounts();
    void saveWordCounts();
    void saveSearchIndex();
    void buildSearchIndex();
    void process();

  private:
    std::string input_path;
    std::string output_path;
    size_t num_threads;
    
    std::vector<boost::filesystem::path> files;
    // per thread dictionaries
    std::vector<std::set<std::string>> dictionaries;
    // full dictionary
    std::set<std::string> dictionary;
    // per thread word counts
    std::vector<std::unordered_map<std::string, std::unordered_map<std::string, int>>> word_count_maps;
    // map from file name to map of word to count
    std::unordered_map< std::string, std::unordered_map<std::string, int>> file_word_count_map;
    // total word count map
    std::unordered_map<std::string, int> word_counts;
    // search index
    // "computer" -> [("file1.xml", count), ("file2.xml", count), ...]
    //std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> index;
    // "computer" -> [("file1.xml", [pos1, pos2, ...]), ("file2.xml", [pos1, ...]), ...]
    //std::unordered_map<std::string, std::vector<std::pair<std::string, std::vector<int>>>> index;
    //std::unordered_map<std::string, std::vector<SearchResult>> index;
    // map from file id to filename
    //std::map<int, std::string> file_id_map;
    // "computer" -> [(count, fileId), (count, fileId), ...]
    std::unordered_map<std::string, std::multimap<long, long, std::greater<long>>> index;
};
