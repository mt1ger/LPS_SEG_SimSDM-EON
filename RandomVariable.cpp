#include "RandomVariable.h"
#include <cmath>
#include <cstdlib>

double
universal_rv() {
  return (double)rand() / (double)RAND_MAX;
}

int
uniform_rv(int numNodes) {
  return (int)(universal_rv() * numNodes);
}

double
exponential_rv(double lambda) {
  double exp;

  exp = ((-1) / lambda) * log(universal_rv());
  return (exp);
}
