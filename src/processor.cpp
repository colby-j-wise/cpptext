#include "processor.h"

#include <omp.h>
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

void Processor::saveDictionary() {
  fs::path dict_path{output_path};
  dict_path /= "dictionary.txt";
  // create dictionary path
  fs::ofstream dict_fs(dict_path);
  for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
    dict_fs << *it << std::endl;
  }
}

void Processor::saveFileIdMap() {
  fs::path file_id_map_path{output_path};
  file_id_map_path /= "file_ids.txt";
  fs::ofstream file_id_map_fs(file_id_map_path);
  for (auto it = file_id_map.begin(); it != file_id_map.end(); ++it) {
    file_id_map_fs << it->first << " " << it->second << std::endl;
  }
}

void Processor::saveFileWordCounts() {
  for (auto it = file_word_count_map.begin(); it != file_word_count_map.end(); ++it) {
    fs::path map_path{output_path};
    map_path /= it->first;
    fs::ofstream map_fs(map_path);
    for (auto wordIt = it->second.begin(); wordIt != it->second.end(); ++wordIt) {
      map_fs << wordIt->first << " " << wordIt->second << std::endl;
    }
  }
}

void Processor::saveWordCounts() {
  fs::path word_count_path{output_path};
  word_count_path /= "wordCounts.txt";
  fs::ofstream word_count_fs(word_count_path);
  for (auto it = word_counts.begin(); it != word_counts.end(); ++it) {
    word_count_fs << it->first << " " << it->second << std::endl;
  }
}

void Processor::saveSearchIndex() {
  fs::path search_index_path{output_path};
  search_index_path /= "searchIndex.txt";
  fs::ofstream search_index_fs(search_index_path);
  for (auto it = index.begin(); it != index.end(); ++it) {
    search_index_fs << it->first << ": ";
    for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      search_index_fs << it2->first << " " << it2->second << ", ";
    }
    search_index_fs << std::endl;
  }
}

void Processor::buildSearchIndex() {
  // per thread indexes
  std::vector<std::map<std::string, std::multimap<long, long, std::greater<long>>>> indexes;
  for (size_t i = 0; i < num_threads; ++i) {
    indexes.emplace_back();
  }
  #pragma omp parallel for num_threads(num_threads)
  for (size_t i = 0; i < files.size(); ++i) {
    int thread_num = omp_get_thread_num();
    std::string key = files[i].stem().string() + ".counts";
    auto wcm = file_word_count_map[key];
    for (auto it = wcm.begin(); it != wcm.end(); ++it) {
      // first is the word, second is count
      indexes[thread_num][it->first].emplace(it->second, i);
    }
  }
  // merge per thread indexes
  for (size_t i = 0; i < num_threads; ++i) {
    for (auto it = indexes[i].begin(); it != indexes[i].end(); ++it) {
      for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) { 
        index[it->first].emplace(it2->first, it2->second);
      }
    }
  }
  saveSearchIndex();
}

// returns list of files most relevant to a search query
std::vector<std::string> Processor::search(std::string query) {
  // TODO: check that the query is only one word
  std::vector<std::string> results;
  auto query_index = index[query];
  if (query_index.empty()) {
    return results;
  }
  for (auto it = query_index.begin(); it != query_index.end(); ++it) {
    results.emplace_back(file_id_map[it->second]);
    //std::cout << file_id_map[it->second] << " " << it->first << std::endl;
  }
  return results;
}

void Processor::process() {
  // per thread file id map
  std::vector<std::map<int, std::string>> file_id_maps;
  // per thread dictionaries
  std::vector<std::set<std::string>> dictionaries;
  // per thread word counts
  std::vector<std::unordered_map<std::string, std::unordered_map<std::string, int>>> word_count_maps;
  for (size_t i = 0; i < num_threads; ++i) {
    file_id_maps.emplace_back();
    dictionaries.emplace_back();
    word_count_maps.emplace_back();
  }
  #pragma omp parallel for num_threads(num_threads)
  for (size_t i = 0; i < files.size(); ++i) {
    int thread_num = omp_get_thread_num();
    std::string key = files[i].stem().string() + ".counts";
    fs::ifstream ifs(files[i]);
    // add to file id map
    file_id_maps[thread_num][i] = fs::canonical(files[i]).string();
    // for each line
    for (std::string line; getline(ifs, line);) {
      // tokenize the line using boost::split for now,
      // but we should use a configurable regex in the future
      std::vector<std::string> toks;
      boost::split(toks, line, boost::is_any_of("\t\v\f\r "));
      // for each word
      for (std::string tok : toks) {
        dictionaries[thread_num].insert(tok);
        if (word_count_maps[thread_num][key].find(tok) == word_count_maps[thread_num][key].end()) {
          word_count_maps[thread_num][key][tok] = 1;
        } else {
          word_count_maps[thread_num][key][tok] += 1;
        }
      }
    }
  }
  // merge per thread dictionaries into full dictionary
  // merge per thread word_count_maps into single word_count_map
  for (size_t i = 0; i < num_threads; ++i) {
    file_id_map.insert(file_id_maps[i].begin(), file_id_maps[i].end());
    dictionary.insert(dictionaries[i].begin(), dictionaries[i].end());
    file_word_count_map.insert(word_count_maps[i].begin(), word_count_maps[i].end());
  }
  // merge file word counts together into total word count
  for (auto it = file_word_count_map.begin(); it != file_word_count_map.end(); ++it) {
    for (auto i = it->second.begin(); i != it->second.end(); ++i) { 
      if (word_counts.find(i->first) == word_counts.end()) { 
        word_counts[i->first] = i->second;
      } else {
        word_counts[i->first] += i->second;
      }
    }
  }
  saveDictionary();
  saveFileIdMap();
  saveFileWordCounts();
  saveWordCounts();
}




