#include "normalizer.h"

#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <regex>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

Normalizer::Normalizer(const std::string data_path, const std::string output_path, const size_t num_threads) : data_path(data_path), output_path(output_path), num_threads(num_threads) {
  try {
    // throw an exception if data_path doesn't point to a directory
    if (!fs::is_directory(data_path)) {
      throw std::invalid_argument("data_path is not a directory");
    }
    // check that the output path directory exists
    if (fs::exists(output_path)) {
      if (!fs::is_directory(output_path)) {
        throw std::invalid_argument("output_path is not a directory");
      }
    } else {
      // create the output directory if it doesn't exist
      fs::create_directory(output_path);
    }
    // read and store file paths
    for (auto&& file : fs::directory_iterator(data_path)) {
      if (fs::is_regular_file(file)) {
        files.push_back(file.path());
      }
    }
  } catch (const fs::filesystem_error& e) {
    std::cout << e.what() << "\n";
  }
}

// constructor only for unit testing
Normalizer::Normalizer() { }

void Normalizer::filterWhitespaceLines() {
  line_predicates.emplace_back(whitespaceOnly);
}

// create lowercase function and add to token_transformers
void Normalizer::lowercase() {
  token_transformers.emplace_back(lowercaseFunc);
}

// remove punctuation
void Normalizer::removePunc() {
  token_transformers.emplace_back(removepuncFunc);
}

// remove digits
void Normalizer::removeDigits() {
  token_transformers.emplace_back(removedigitFunc);
}

// set stopwords
void Normalizer::setStopwords(std::unordered_set<std::string> stopwords) {
  stop_words = stopwords;
}

// add regex to list
void Normalizer::addRegex(std::string regex, std::string replace_with) {
  if (!replace_with.empty()) {
    regex_list.push_back( std::make_pair(std::regex(regex), replace_with) );
  }
  else {
    regex_list.push_back( std::make_pair(std::regex(regex), "") );
  }
}

// Iterate through regex_list removing regex
std::string Normalizer::runRegex(std::string &line) {
  for (auto pair : regex_list) {
    line = std::regex_replace(line, pair.first, pair.second);
  }
  return line;
}

void Normalizer::process() {
  #pragma omp parallel for num_threads(num_threads)
  for (size_t i = 0; i < files.size(); ++i)
  {
    fs::path op{output_path};
    op /= files[i].filename();
    // create output file in the output directory with the same name as input file
    fs::ofstream ofs(op);
    fs::ifstream ifs(files[i]);
    // for each line
    for (std::string line; getline(ifs, line);)
    {
      // ignore the line if any of the line predicates returns true
      bool ignore = false;
      for (std::function<bool(std::string)> pred : line_predicates)
      {
        if (pred(line)) {
          ignore = true;
        }
      }
      // if the line is not empty and should not be ignored
      if (line.length() != 0 && !ignore)
      {
        // remove all regexs
        line = runRegex(line);

        // tokenize the line using boost::split for now,
        // but we should use a configurable regex in the future
        std::vector<std::string> toks;
        boost::split(toks, line, boost::is_any_of("\t\v\f\r "));
        // for each token
        for (std::string& tok : toks)
        {
          for (auto transformer : token_transformers) {
            transformer(tok);
          }
          // remove stopwords if necessary
          if (!stop_words.empty() && stop_words.find(tok) != stop_words.end()) {
            tok = "";
          }
          // write token to output file and add a space to separate each token
          ofs << tok << " ";
        }
        ofs << std::endl;
      }

    }
    //break;
  }
}

//for stopwords unit testing
std::string Normalizer::process_string(std::string process_string, std::unordered_set<std::string> stop_words) {
    std::string finished_string;
    std::vector<std::string> toks;
    boost::split(toks, process_string, boost::is_any_of("\t\v\f\r "));
    for (std::string& tok : toks) {
      if (!stop_words.empty() && stop_words.find(tok) != stop_words.end()) {
        tok = "";
        finished_string.append(tok);
      }
      else {
        finished_string.append(tok);
        finished_string.append(" ");
      }
    }
    //gets rid of last space
    finished_string.pop_back();
    return finished_string;
}
