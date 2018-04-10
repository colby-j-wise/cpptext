#pragma once

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

class Normalizer {
  public:
    Normalizer(const std::string data_path, const std::string output_path, const size_t num_threads);
    void filterWhitespaceLines();
    void process();
  private:
    std::string data_path;
    std::string output_path;
    size_t num_threads;
    std::vector<boost::filesystem::path> files;
    std::vector<std::function<bool(std::string)>> line_predicates;
    // TODO: add lowercase token transformer to this list
    std::vector<std::function<std::string(std::string)>> token_transformers;
};

