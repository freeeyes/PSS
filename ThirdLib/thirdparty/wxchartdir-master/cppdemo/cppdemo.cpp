#include <iostream>

// Generate function prototypes
#define GENFUNC(fname) \
int fname(int argc, char* argv[]);
#include "cppdemo_gen.h"
#undef GENFUNC

// Include sources of all functions
#include "cppdemo_inc.h"

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

int main(int argc, char *argv[])
{
  // Generate function invocations
#define GENFUNC(fname) \
  std::cout << "Invoking " STRINGIFY(fname) << std::endl; \
  fname(argc, argv);
#include "cppdemo_gen.h"
#undef GENFUNC

  return 0;
}
