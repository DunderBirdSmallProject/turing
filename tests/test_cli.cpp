#include "cli.h"
#include "turing.h"

void test_cli_verbose() {
    Turing::Cli cli = Turing::cli();
    char *argv[] = {"", "-v"};
    cli.parse(1, argv);
}
