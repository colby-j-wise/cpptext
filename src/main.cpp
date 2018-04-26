#include <iostream>
#include <string>
#include <unordered_set>
#include <regex>
#include <boost/program_options.hpp>

#include "normalizer.h"
#include "processor.h"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

po::variables_map parse_args(int argc, char** argv) {
  try {
    po::options_description desc("Options");
    desc.add_options()
      ("help", "Print help")
      ("data_path", po::value<std::string>()->required(), "Directory of text files to read")
      ("output_path", po::value<std::string>()->required(), "Directory to place output files NOTE: the files in this directory will be overwritten!")
      ("processor_output_path", po::value<std::string>()->required(), "Directory to place processor output files NOTE: the files in this directory will be overwritten!")
      ("num_threads", po::value<size_t>()->default_value(8), "Number of threads")
      ;
    po::variables_map vm;
    po::parsed_options opts(po::command_line_parser(argc, argv).options(desc).run());
    po::store(opts, vm);
    if (vm.count("help")) {
      std::cout << desc << std::endl;
      exit(-1);
    }
    po::notify(vm);

    return vm;
  } catch (po::error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    exit(-1);
  } catch (std::exception& e) {
    std::cerr << "Exception Error: " << e.what() << std::endl;
    exit(-1);
  }
}

// get valid substring from s from pos - limit to pos + limit
std::string string_range(std::string s, int pos, int limit) {
  int start = pos - limit;
  size_t end = pos + limit;
  if (start < 0) { 
    start = 0;
  }
  if (end > s.length()) {
    end = s.length();
  }
  return s.substr(start, end - start);
}

// search loop for demo
void run_search(Processor processor) {
  std::string in;
  while (in != "q") {
    std::cout << std::endl;
    std::cout << "Enter a search term (q to exit): ";
    std::cin >> in;
    auto files = processor.search(in);
    if (files.empty()) {
      std::cout << "No Results\n";
      continue;
    }
    std::cout << "Results:\n";
    int filecount = 0;
    for (auto f : files) {
      fs::path p{f};
      std::cout << std::endl;
      fs::ifstream ifs(p);
      std::cout << p.filename().string() << ":" << std::endl;
      int linecount = 0;
      for (std::string line; getline(ifs, line);) {
        size_t n = line.find(" " + in + " ");
        // ignore this line if it doesn't contain our search term
        if (n == std::string::npos) {
          continue;
        }
        std::cout << "\t'" << string_range(line, n, 40) << "'\n";
        linecount++;
        // only print 3 lines from each file
        if (linecount > 2) {
          break;
        }
      }
      filecount++;
      // only show up to 5 files per search
      if (filecount == 4) {
        break;
      }
    }
  }
}

int main(int argc, char *argv[]) {
  auto options = parse_args(argc, argv);
  std::string data_path = options["data_path"].as<std::string>();
  std::string output_path = options["output_path"].as<std::string>();
  std::string processor_output_path = options["processor_output_path"].as<std::string>();
  size_t num_threads = options["num_threads"].as<size_t>();
  std::unordered_set<std::string> stop_words = {"post"};

  std::cout << "Data directory: " << data_path << "\n";
  std::cout << "Output directory: " << output_path << "\n";
  std::cout << "Processor Output directory: " << processor_output_path << "\n";
  std::cout << "Number of threads: " << num_threads << "\n";

  Normalizer normalizer(data_path, output_path, num_threads);
  // remove regex
  normalizer.addRegex("<[^>]+>", ""); // remove </ html>
  normalizer.addRegex("\\s\\s+", " "); // replace consequetive whitespace "\\s\\s+" with a single space " "
  // simple function to remove the "urlLink" label in our blogs dataset
  auto removeUrlLink = [](std::string& s) {
    if (s == "urlLink") {
      s = "";
    }
  };
  // remove dates
  normalizer.addTokenTransformer(removeUrlLink);
  // remove any lines that are just whitespace
  normalizer.filterWhitespaceLines();
  // lowercase all words
  normalizer.lowercase();
  // remove punctuation all words
  normalizer.removePunc();
  // remove digits from text
  normalizer.removeDigits();
  // remove stopwords
  normalizer.setStopwords(stop_words);
  // run normalizer
  normalizer.process();


  // probably need to add a processor class to compute summary stats in the future
  Processor processor(output_path, processor_output_path, num_threads);
  processor.process();
  // build search index
  processor.buildSearchIndex();
  // search demo
  run_search(processor);
}
