#include "csv2shell.hh"

template <>
void convertHeader<SHELL>(std::ostream& out, const Header& header) {
  out << "#!/usr/bin/env zsh\n";
  out << "# " << header.description << "\n";
  out << "# " << header.url << "\n";
  out << "\n";
}

template <>
void convertCSV<SHELL>(std::ostream& out, const CSV& csv) {
  for (auto i = 0; i < csv.types.size(); i++) {
    out << "export " << csv.names[i] << "=" << csv.numbers[i] << "\n";
  }
  out << "\n";
}
