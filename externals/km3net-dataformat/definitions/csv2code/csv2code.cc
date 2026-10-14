#include "csv2code.hh"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "csv2cpp.hh"
#include "csv2julia.hh"
#include "csv2python.hh"
#include "csv2shell.hh"
#include "csv2csh.hh"

using namespace std;

CSV readCSV(istream& in) {
  // format of the CSV is type,name,number,comment
  CSV csv;
  string line;
  while (getline(in, line)) {
    if (line.size() < 2 || line[0] == '#') {
      continue;
    }
    string type, name, number, comment;
    stringstream sline(line);
    getline(sline, type, ',');
    getline(sline, name, ',');
    getline(sline, number, ',');
    getline(sline, comment);
    csv.types.emplace_back(type);
    csv.names.emplace_back(name);
    csv.numbers.emplace_back(number);
    csv.comments.emplace_back(comment);
  }
  return csv;
}

/**
 * Helper program to convert an input CSV file, with a format of
 * #type,name,number,comment
 *
 * into a source code file of the requested language.
 */
int main(int argc, char** argv) {
  if (argc < 5) {
    throw runtime_error("need at least 4 arguments");
  }
  string lang = argv[1];     // language tag : hh,py,jl,sh
                             // CPP,PYTHON,JULIA,SHELL
  string infile = argv[2];   // input file name, input content should be CSV
  string outfile = argv[3];  // output file name
  string base = argv[4];     // base name of the definition
  string description =
      argc > 5 ? argv[5] : "some description";   // optional description
  string url = argc > 6 ? argv[6] : "some url";  // optional url

  ifstream in(infile);
  if (!in.is_open()) {
    std::cerr << "Cannot open input file " << infile << "\n";
    exit(1);
  }
  ofstream out(outfile);
  if (!out.is_open()) {
    std::cerr << "Cannot open output file " << outfile << "\n";
    exit(2);
  }

  Header header{base, description, url};
  CSV csv = readCSV(in);

  if (lang == "hh") {
    convert<CPP>(out, header, csv);
  } else if (lang == "py") {
    convert<PYTHON>(out, header, csv);
  } else if (lang == "jl") {
    convert<JULIA>(out, header, csv);
  } else if (lang == "sh") {
    convert<SHELL>(out, header, csv);
  } else if (lang == "csh") {
    convert<CSH>(out, header, csv);
  } else {
    throw runtime_error(
        "unknown output language requested. I only know hh, py");
  }

  return 0;
}
