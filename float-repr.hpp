#ifndef FLOAT_REPR_HPP
#define FLOAT_REPR_HPP

#include <bitset>
#include <climits>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <ostream>
#include <sstream>
#include <string_view>

namespace tlo {
template <typename Float>
struct FloatTraits;

template <>
struct FloatTraits<double> {
  using UInt = std::uint64_t;
  static constexpr int STANDARD_SIZE = 64;
  static constexpr int EXPONENT_SIZE = 11;
  static constexpr int SIGNIFICAND_SIZE = 52;
  static constexpr int BIAS = 1023;
  static constexpr UInt EXPONENT_MASK = 0b11111111111;
  static constexpr UInt SIGNIFICAND_MASK = 0xFFFFFFFFFFFFF;
  static constexpr int ACTUAL_SIZE = sizeof(double) * CHAR_BIT;
  static constexpr std::string_view TYPE_NAME = "double";
};

template <>
struct FloatTraits<float> {
  using UInt = std::uint32_t;
  static constexpr int STANDARD_SIZE = 32;
  static constexpr int EXPONENT_SIZE = 8;
  static constexpr int SIGNIFICAND_SIZE = 23;
  static constexpr int BIAS = 127;
  static constexpr UInt EXPONENT_MASK = 0b11111111;
  static constexpr UInt SIGNIFICAND_MASK = 0b11111111111111111111111;
  static constexpr int ACTUAL_SIZE = sizeof(float) * CHAR_BIT;
  static constexpr std::string_view TYPE_NAME = "float";
};

template <typename Float>
struct FloatRepr {
  using UInt = typename FloatTraits<Float>::UInt;

  const Float number;

  FloatRepr(Float number_) : number(number_) {}

  UInt toUInt() const {
    return *static_cast<const UInt *>(static_cast<const void *>(&number));
  }

  std::bitset<FloatTraits<Float>::STANDARD_SIZE> allBits() const {
    return std::bitset<FloatTraits<Float>::STANDARD_SIZE>(toUInt());
  }

  bool signBit() const {
    return toUInt() >> (FloatTraits<Float>::STANDARD_SIZE - 1);
  }

  std::bitset<1> signBitSet() const { return std::bitset<1>(signBit()); }

  UInt exponent() const {
    return (toUInt() >> FloatTraits<Float>::SIGNIFICAND_SIZE) &
           FloatTraits<Float>::EXPONENT_MASK;
  }

  std::bitset<FloatTraits<Float>::EXPONENT_SIZE> exponentBits() const {
    return std::bitset<FloatTraits<Float>::EXPONENT_SIZE>(exponent());
  }

  UInt significand() const {
    return toUInt() & FloatTraits<Float>::SIGNIFICAND_MASK;
  }

  std::bitset<FloatTraits<Float>::SIGNIFICAND_SIZE> significandBits() const {
    return std::bitset<FloatTraits<Float>::SIGNIFICAND_SIZE>(significand());
  }

  int unbiasedExponent() const {
    return static_cast<int>(exponent()) - FloatTraits<Float>::BIAS;
  }

  std::string realValue() const {
    std::ostringstream oss;
    oss << "(-1)^" << signBitSet() << " * 0b1." << significandBits() << " * 2^"
        << unbiasedExponent();
    return oss.str();
  }

  void outputDetails(std::ostream &ostream) const {
    std::streamsize oldPrecision = ostream.precision();
    ostream.precision(std::numeric_limits<Float>::max_digits10);
    ostream << "number: " << number << std::endl;
    ostream << "binary representation: " << allBits() << std::endl;
    ostream << "sign bit: " << signBitSet() << std::endl;
    ostream << "exponent: " << exponent() << std::endl;
    ostream << "exponent bits: " << exponentBits() << std::endl;
    ostream << "significand bits: " << significandBits() << std::endl;
    ostream << "unbiased exponent: " << unbiasedExponent() << std::endl;
    ostream << "real value: " << realValue() << std::endl;
    ostream.precision(oldPrecision);
  }
};
}  // namespace tlo

#endif  // FLOAT_REPR_HPP
