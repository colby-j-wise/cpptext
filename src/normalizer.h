#pragma once

#include <string>
#include <vector>
#include <unordered_set>

#include <boost/filesystem.hpp>

class Normalizer {
  public:
    Normalizer(const std::string data_path, const std::string output_path, const size_t num_threads);
    // constructor for unit testing
    Normalizer();
    void filterWhitespaceLines();
    void lowercase();
    void removePunc();
    void removeDigits();
    void setStopwords(std::unordered_set<std::string> stopwords);
    void process();
    // normalization functions
    std::function<bool(std::string)> whitespaceOnly = [](std::string line) { return std::all_of(line.begin(), line.end(), isspace); };
    std::function<void(std::string&)> lowercaseFunc = [](std::string& tok) { std::transform(tok.begin(), tok.end(), tok.begin(), ::tolower); };
    std::function<void(std::string&)> removepuncFunc = [](std::string& tok) { tok.erase (std::remove_if (tok.begin (), tok.end (), ispunct), tok.end ()); };
    std::function<void(std::string&)> removedigitFunc = [](std::string& tok) { tok.erase( std::remove_if(tok.begin(), tok.end(), isdigit), tok.end ()); };
  private:
    std::string data_path;
    std::string output_path;
    size_t num_threads;
    std::vector<boost::filesystem::path> files;
    std::vector<std::function<bool(std::string)>> line_predicates;
    std::vector<std::function<void(std::string&)>> token_transformers;
    std::unordered_set<std::string> stop_words;
    // data for unit testing only
    std::string data;
};
