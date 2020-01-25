#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <cstdlib>
#include <iostream>

#include "float-repr.hpp"

template <typename Float>
Float strToFloat(const char *str, char **strEnd);

template <>
float strToFloat<float>(const char *str, char **strEnd) {
  return std::strtof(str, strEnd);
}

template <>
double strToFloat<double>(const char *str, char **strEnd) {
  return std::strtod(str, strEnd);
}

template <typename Float>
int programMain(int argc, char **argv) {
  if (argc == 1) {
    std::cerr << "usage: " << argv[0] << " <number>" << std::endl;
    return 1;
  }

  std::cerr << "size of " << tlo::FloatTraits<Float>::TYPE_NAME
            << " (in bits): " << tlo::FloatTraits<Float>::ACTUAL_SIZE
            << std::endl;

  if (tlo::FloatTraits<Float>::ACTUAL_SIZE !=
      tlo::FloatTraits<Float>::STANDARD_SIZE) {
    std::cerr << "error: unsupported " << tlo::FloatTraits<Float>::TYPE_NAME
              << " size" << std::endl;
    return 2;
  }

  Float number = strToFloat<Float>(argv[1], nullptr);
  const tlo::FloatRepr<Float> repr(number);
  repr.outputDetails(std::cout);
  return 0;
}

#endif  // PROGRAM_HPP
