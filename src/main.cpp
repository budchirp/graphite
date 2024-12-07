#include <exception>

#include "cli/cli.hpp"
#include "utils/logger/logger.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  auto cli = CLI();
  try {
    cli.parser->parse(argc, argv);
  } catch (const exception &error) {
    Logger::error(cli.parser->app_name + ": error: " + error.what());
    return 1;
  }

  return 0;
}
