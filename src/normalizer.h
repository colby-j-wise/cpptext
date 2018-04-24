#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <boost/filesystem.hpp>

class Normalizer {
  public:
    //Normalizer(const std::string data_path, const std::string output_path, const size_t num_threads);
    Normalizer(const std::string data_path, const std::string output_path, const size_t num_threads);
    void filterWhitespaceLines();
    void lowercase();
    void removepunc();
    void removedigits();
    void process();
    void initstopwords(std::unordered_set<std::string> stopwords);
    std::string removestopword(std::string word);
  private:
    std::string data_path;
    std::string output_path;
    size_t num_threads;
    std::vector<boost::filesystem::path> files;
    std::vector<std::function<bool(std::string)>> line_predicates;
    std::vector<std::function<void(std::string&)>> token_transformers;
    std::unordered_set<std::string> stop_words;
};
