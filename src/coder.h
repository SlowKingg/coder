
#include <inttypes.h>
#include <string.h>

enum {
    MaxCodeLength = 4
};

typedef struct {
    uint8_t code[MaxCodeLength];
    size_t length;
} CodeUnits;


int encode(uint32_t code_point, CodeUnits *code_units);
