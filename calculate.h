#include "calculator.h"

pDigit multiply(pDigit first, pDigit second);
pDigit mul(pDigit digit, int x, int under);
pNum beforeCal(pNum max, pNum min, int max_before, int min_before, bool carry, bool* result);
pNum afterCal(pNum max, pNum min, int max_after, int min_after, bool* result);
pDigit plus(pDigit first, pDigit second);
pDigit minus(pDigit first, pDigit second);
pDigit divide(pDigit first, pDigit second);