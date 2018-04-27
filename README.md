# cpptext
Fast Text Preprocessing Library

Bharat Srikishan <bs2982@columbia.edu>

Colby James Wise <cjw2165@columbia.edu>

Peter Richards <p.richards@columbia.edu>

## Requirements and Dependencies

- C++ 14
- Threading library (pthread, ...)
- GNU Scientific Library (GSL)
- OpenMP
- Boost
    + Program Options
    + Filesystem

## Project Structure
- `bin/` is where CMake puts our binaries
- `build/` is for CMake build files
- `src/` has all our C++ code and a main file which serves a demo of the library, here we also have shell scripts for building and running our code and tests
- `data/` is the directory to download and unzip the blog corpus dataset: [The Blog Authorship Corpus](http://u.cs.biu.ac.il/~koppel/BlogCorpus.htm)

## Building

First make sure to put some text files into the `data/` directory. We primarily used the [The Blog Authorship Corpus](http://u.cs.biu.ac.il/~koppel/BlogCorpus.htm)
Our main file normalizes and processes data, building a search index. Then it runs a simple search program that allows you to type in single word search queries
and shows your results.

Building and running is straightforward:

`./build.sh && ./run.sh`

To run unit tests do:

`./run_tests.sh`

To clean do:

`./clean.sh`

To delete all build files and binaries do:

`./distclean.sh`

## Sample Datasets

[The Blog Authorship Corpus](http://u.cs.biu.ac.il/~koppel/BlogCorpus.htm)

[Classic Literature in ASCII](https://www.kaggle.com/mylesoneill/classic-literature-in-ascii/data)

