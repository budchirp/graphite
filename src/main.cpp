#include <exception>
#include <iostream>

#include "cli/cli.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  const auto cli = CLI();
  try {
    cli.parse(argc, argv);
  } catch (const exception &error) {
    cerr << error.what() << endl;
    return 1;
  }

  return 0;
}
