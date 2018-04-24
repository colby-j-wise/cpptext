#include <iostream>
#include <string>
#include <unordered_set>

#include <boost/program_options.hpp>

#include "normalizer.h"
#include "processor.h"

namespace po = boost::program_options;

po::variables_map parse_args(int argc, char** argv) {
  try {
    po::options_description desc("Options");
    desc.add_options()
      ("help", "Print help")
      ("data_path", po::value<std::string>()->required(), "Directory of text files to read")
      ("output_path", po::value<std::string>()->required(), "Directory to place output files NOTE: the files in this directory will be overwritten!")
      ("processor_output_path", po::value<std::string>()->required(), "Directory to place processor output files NOTE: the files in this directory will be overwritten!")
      ("num_threads", po::value<size_t>()->default_value(4), "Number of threads")
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

int main(int argc, char *argv[]) {
  auto options = parse_args(argc, argv);
  std::string data_path = options["data_path"].as<std::string>();
  std::string output_path = options["output_path"].as<std::string>();
  std::string processor_output_path = options["processor_output_path"].as<std::string>();
  size_t num_threads = options["num_threads"].as<size_t>();
  std::unordered_set<std::string> stop_words = {"post"};

  std::cout << "Data directory: " << data_path << "\n";
  std::cout << "Output directory: " << output_path << "\n";
  std::cout << "Processor Output director: " << processor_output_path << "\n";
  std::cout << "Number of threads: " << num_threads << "\n";

  Normalizer normalizer(data_path, output_path, num_threads);
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
  //processor.getWordCounts();
  //processor.printWordCounts();
  //processor.buildReverseIndex();
}
