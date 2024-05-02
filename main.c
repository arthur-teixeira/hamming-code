#include <assert.h>
#include <stdint.h>

// Hamming code (7,4)
#define NUM_PARITY_BITS 3
#define NUM_DATA_BITS 4
#define NUM_BITS NUM_PARITY_BITS + NUM_DATA_BITS

void calculate_parity_bits(uint8_t buffer[NUM_BITS]) {
  for (int j = 0; j < NUM_BITS; j++) {
    uint8_t acc = 0;
    if (((j + 1) & j) != 0) {
      continue;
    }

    for (int i = 0; i < NUM_BITS; i++) {
      if (((i + 1) & (j + 1)) > 0) {
        acc += buffer[i] & 1;
      }
    }
    buffer[j] = acc & 1;
  }
}

void split_into_buffer(uint8_t data, uint8_t buffer[NUM_BITS]) {
  for (int i = 0, j = 0; i < NUM_BITS; i++) {
    int ii = NUM_BITS - i - 1;

    if (((ii + 1) & ii) == 0) {
      continue;
    }

    buffer[ii] = (data >> (NUM_BITS - j)) & 1;
    j++;
  }
}

void check_buffer(uint8_t buf[NUM_BITS], uint8_t expected[NUM_BITS]) {
  for (int i = 0; i < NUM_BITS; i++) {
    assert(expected[i] == buf[i]);
  }
}

uint16_t collect_buffer(uint8_t buffer[NUM_BITS]) {
  uint16_t acc = 0;
  for (int i = 0; i < NUM_BITS; i++) {
    acc = (acc << 1) | (buffer[i] & 1);
  }

  acc <<= 9;

  return acc;
}

int main() {
  // TODO: Work with bitfields instead of u16
  uint8_t buf[NUM_BITS] = {0};
  split_into_buffer(0b11110000, buf);
  uint8_t expected1[NUM_BITS] = {0, 0, 1, 0, 1, 1, 1};
  check_buffer(buf, expected1);

  calculate_parity_bits(buf);
  uint8_t expected2[NUM_BITS] = {1, 1, 1, 1, 1, 1, 1};
  check_buffer(buf, expected2);

  uint16_t result = collect_buffer(buf);
  assert(result == 0b1111111000000000);

  return 0;
}
