//***************************************************************************
//
//  Herbert Morales
//  Z1959955
//  CSCI-463-1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

/**
 * Print the final binary from a vector
 *
 * Use the exponent and significand in order
 * to calculate where the binary point would go,
 * where the implied 1 and significand would go
 * to complete the binary. If the exponent is
 * below zero, if it is equal negative 1, then
 * the size of the vector to be printed would be
 * 26 characters, it would be filled with zeros,
 * the second character of the vector would be
 * the binary point, the third character in the
 * vector would be the implied 1. Then loop through
 * the bits of the significand and add each to the
 * vector. The same process is done when exponents
 * are less than -1, equal to zero, or more than zero.
 * Finally the vector is printed.
 *
 * @param exponent Exponent from binary.
 * @param significand Significand from binary.
 *
 ********************************************************************************/
void printFinalFloat(int exponent, uint32_t significand);
/**
 * Print the parameter as a formatted binary.
 *
 * Loop through the bits of the hexadecimal
 * and print each bit to the screen, printing
 * a space after every fourth bit is printed.
 *
 * @param x Hexadecimal to be printed as formatted binary
 *
 ********************************************************************************/
void printbinary(uint32_t x);
/**
 * Process, format and print parameter.
 *
 * Call printbinary() and pass in x.
 * Take in hexadecimal, extract the sign
 * and print it to the screen. Loop through
 * the bits that represent the exponent and
 * calculate the exponent based on the bits
 * that are 1. Print exponent to the screen.
 * Use a mask to extract the significand from
 * the entire hexadecimal and print to the screen
 * as hex. Follow rules for determining if the
 * number represents negative infinity or positive
 * infinity, if neither then call printFinalFloat()
 * in order to print the final binary.
 *
 * @param x Hexadecimal to be processed and printed
 *
 ********************************************************************************/
void printBinFloat(uint32_t x);
/**
 * Read user input until EOF
 *
 * Read user input until nothing is entered.
 * The input is converted to hex and then
 * printBinFloat() is called with the user
 * input as the parameter.
 *
 ********************************************************************************/
int main() {
  uint32_t x;

  while (std::cin >> std::hex >> x) {
    printBinFloat(x);
  }

  return 0;
}

void printFinalFloat(int exponent, uint32_t significand) {
  int vsize = 0;
  exponent = exponent - 127; // apply proper exponent
  std::vector<char> fbin;
  uint32_t mask = 0x800000;
  // check how to handle printing depending on exponent
  if (exponent < 0) {
    // exponent is negative
    if (exponent == -1) {
      // vector size will be 26
      vsize = 26;
      fbin.assign(vsize, '0'); // fill char vector with 0
      fbin.at(1) = '.';        // place decimal
      fbin.at(2) = '1';        // place implied 1
      // place significand
      for (int i = 0; i < 24; i++) {
        if (significand & (mask >> i)) {
          // place a 1 in vector after implied 1
          fbin.at(2 + i) = '1';
        }
      }
    } else {
      // exponent is less than -1
      // vector size is 26 + ((exponent * -1) -1)
      vsize = 26 + ((exponent * -1) - 1);
      fbin.assign(vsize, '0'); // fill char vector with 0
      fbin.at(1) = '.';        // place decimal
      int dplaces = (exponent * -1) + 1 + 0;
      fbin.at(dplaces) = '1'; // place implied 1 before significand

      // after dplaces, place significand
      for (int i = 0; i < 24; i++) {
        if (significand & (mask >> i)) {
          // place a 1 in vector after implied 1
          fbin.at(dplaces + i) = '1';
        }
      }
    }
  } else {
    // exponent is not negative
    if (exponent == 0) {
      vsize = 23 + 2;          // dec and implied 1
      fbin.assign(vsize, '0'); // fill char vector with 0
      fbin.at(0) = '1';        // place implied 1
      fbin.at(1) = '.';        // place dec
      // place significand into vector
      for (int i = 1; i < 24; i++) {
        if (significand & (mask >> i)) {
          // place 1 in vector after implied 1
          fbin.at(i + 1) = '1';
        }
      }
    } else {
      // exponent is more than zero
      vsize = exponent + 3;    // dec, implied 1 and zero
      fbin.assign(vsize, '0'); // fill with zero
      fbin.at(0) = '1';        // implied 1 before significand
      // place significand into vector
      for (int i = 0; i < 24; i++) {
        // place 1 in vector after implied 1
        if (significand & (mask >> i)) {
          fbin.at(i) = '1';
        }
      }
      // place decimal
      fbin.at(vsize - 2) = '.';
    }
  }
  // print vector
  for (unsigned i = 0; i < fbin.size(); i++) {
    std::cout << fbin.at(i);
  }
}

void printbinary(uint32_t x) {
  for (int i = 0; i < 32; i++) {
    if (i % 4 == 0) {
      std::cout << " ";
    }
    if (x & (0x80000000 >> i)) {
      // print a 1
      std::cout << '1';
    } else {
      std::cout << '0';
    }
  }
  std::cout << std::endl;
}

void printBinFloat(uint32_t x) {
  uint32_t mask = 0x80000000;
  int cursor = 0; // this is the exponent
  uint32_t significand = (x & 0x7FFFFF);

  // attempt to print sign bit
  std::cout << "0x" << std::hex << std::setw(8) << std::right
            << std::setfill('0') << x << " =";
  printbinary(x);
  std::cout << "sign: ";
  std::cout << (x & 0x80000000 ? '1' : '0') << std::endl;

  for (int i = 1; i <= 8; i++) {
    if (x & (mask >> i)) {
      cursor += (1 << (8 - i)); // calculate exponent
    }
  }

  std::cout << " exp: ";
  std::cout << "0x";
  std::cout << std::setw(8) << std::right << std::setfill('0') << (cursor - 127)
            << " ";
  std::cout << "(" << std::dec << (cursor - 127) << ")" << std::endl;

  std::cout << " sig: ";
  std::cout << "0x";
  std::cout << std::hex << std::setw(8) << std::right << std::setfill('0')
            << (significand) << std::endl;
  // print final binfloat
  std::cout << (x & 0x80000000 ? '-' : '+');

  // handle pos and neg infinity
  if ((cursor & 0xFF) == 0xFF) {
    // exponent is all ones
    if ((significand & 0x7FFFFF) == 0x000000) {
      // significand is all zeroes
      if (!(x & 0x80000000)) {
        // sign is zero
        // this number represents positive infinity
        std::cout << "inf";
      } else {
        // sign is one
        // this number represents negative infinity
        std::cout << "inf";
      }
    }
  } else {
    if (((cursor - 127) == -127) && (significand == 0x00000000)) {
      std::cout << "0"; // only a zero needs to be printed
    } else {
      printFinalFloat(cursor, significand);
    }
  }
  std::cout << std::endl;
}
