#include "km3net-dataformat/offline/Head.hh"
#include "km3net-dataformat/offline/io_ascii.hh"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

void expect_equal(const std::vector<std::string>& actual,
                  const std::vector<std::string>& expected,
                  const std::string& label)
{
  if (actual == expected) {
    return;
  }

  std::cerr << label << " mismatch" << std::endl;
  std::cerr << "  expected:";
  for (const auto& value : expected) {
    std::cerr << " [" << value << "]";
  }
  std::cerr << std::endl;
  std::cerr << "  actual:";
  for (const auto& value : actual) {
    std::cerr << " [" << value << "]";
  }
  std::cerr << std::endl;
  std::exit(1);
}

void expect_true(bool condition, const std::string& message)
{
  if (!condition) {
    std::cerr << message << std::endl;
    std::exit(1);
  }
}

} // namespace

int main()
{
  Head h;

  h.set_line("seed", "42");
  h.set_line("detector", "aaa bbb");
  h.set_line("detector", "ccc ddd");
  h.set_line("detector", "ccc ddd eee");

  const std::vector<std::string> expected_keys = {
    "detector",
    "detector_1",
    "detector_2"
  };
  const std::vector<std::string> expected_lines = {
    "aaa bbb",
    "ccc ddd",
    "ccc ddd eee"
  };

  const auto keys = h.matching_keys("detector");
  const auto lines = h.get_lines("detector");
  std::ostringstream output;

  expect_equal(keys, expected_keys, "matching_keys");
  expect_equal(lines, expected_lines, "get_lines");

  expect_true(write(h, output), "write(Head, ostream) returned false");

  const std::string rendered = output.str();
  expect_true(rendered.find("seed: 42") != std::string::npos,
              "Rendered header does not contain the seed line");
  expect_true(rendered.find("detector_2: ccc ddd eee") != std::string::npos,
              "Rendered header does not contain the final detector line");

  std::cout << "OK" << std::endl;
  return 0;
}
