#include "processor.h"

#include <iostream>
#include <algorithm>
#include <boost/filesystem/fstream.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

Processor::Processor(const std::string input_path, const std::string output_path, const size_t num_threads) : input_path(input_path), output_path(output_path), num_threads(num_threads) {
  try {
    // throw an exception if input_path doesn't point to a directory
    if (!fs::is_directory(input_path)) {
      throw std::invalid_argument("input_path is not a directory");
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
    for (auto&& file : fs::directory_iterator(input_path)) {
      if (fs::is_regular_file(file)) {
        files.push_back(file.path());
      }
    }
  } catch (const fs::filesystem_error& e) {
    std::cout << e.what() << "\n";
  }
}

void Processor::getWordCounts() 
{
  #pragma omp parallel for num_threads(num_threads)
  for (size_t i = 0; i < files.size(); ++i) 
  {
    std::string key = files[i].filename().string();
    fs::ifstream ifs(files[i]);

    for (std::string line; getline(ifs, line);)
    {
      std::vector<std::string> words;
      boost::split(words, line, boost::is_any_of("\t\v\f\r "));

      for (auto word : words)
      {
        if ( word_count_map[key].find(word) == word_count_map[key].end() )
        {
          word_count_map[key][word] = 1;
        }
        else {
          word_count_map[key][word] += 1;
        }
      }
    }
  }
}

void Processor::printWordCounts(){
  if (! word_count_map.empty()) 
  { 
    std::unordered_map< std::string, std::unordered_map<std::string, int>>::iterator outer_itr;
    std::unordered_map<std::string, int>::iterator inner_itr;

    for (outer_itr = word_count_map.begin(); outer_itr != word_count_map.end(); ++outer_itr)
    {
        //std::cout << "Filename: " << outer_itr.first <<  std::endl;
        //std::cout << "Word   |    Count " << std::endl;
        // for (outer_itr->second; outer_itr->second != outer_itr->second.end(); ++(outer_itr->second))  
        //     std::cout << outer_itr->second->first << "       " << outer_itr->second->second;
    }
  }
  else {
    std::cout << "Word Count Dictionary is empty. Cannot Print!" << std::endl;
  }
}



