#include "cli.h"
#include "turing.h"
#include "util.h"

void test_cli_verbose() {
    Turing::Cli cli = Turing::Cli();
    const char *argv1[] = {"", "-v", "filename", "000"};
    cli.parse(sizeof(argv1) / sizeof(char *), argv1);
    EXPECT(cli.isVerbose());
    cli.reset();

    const char *argv2[] = {"", "--verbose", "filename", "000"};
    cli.parse(sizeof(argv2) / sizeof(char *), argv2);
    EXPECT(cli.isVerbose());
    cli.reset();

    const char *argv3[] = {"", "--verbose"};
    int res = cli.parse(sizeof(argv3) / sizeof(char *), argv3);
    EXPECT(res < 0);
}

void test_cli_help() {
    Turing::Cli cli = Turing::Cli();
    int res = 0;

    const char *argv1[] = {"", "-h"};
    res = cli.parse(sizeof(argv1) / sizeof(char *), argv1);
    EXPECT(cli.isHelp());
    EXPECT(res == 0);
    cli.reset();

    const char *argv2[] = {"", "--help"};
    res = cli.parse(sizeof(argv2) / sizeof(char *), argv2);
    EXPECT(cli.isHelp());
    EXPECT(res == 0);
    cli.reset();

    const char *argv3[] = {"", "--help", "good", "000"};
    res = cli.parse(sizeof(argv3) / sizeof(char *), argv3);
    EXPECT(cli.isHelp());
    EXPECT(res == 0);
}

void test_cli_all() {
    Turing::Cli cli = Turing::Cli();
    int res = 0;

    const char *argv1[] = {"", "dir/a.tm", "000"};
    res = cli.parse(sizeof(argv1) / sizeof(char *), argv1);
    EXPECT(res == 0);
    EXPECT(!cli.isHelp());
    EXPECT(!cli.isVerbose());
    EXPECT(cli.getInput() == "000");
    EXPECT(cli.getInputFile() == "dir/a.tm");
    cli.reset();

    const char *argv2[] = {"", "tm", "--verbose", "1"};
    res = cli.parse(sizeof(argv2) / sizeof(char *), argv2);
    EXPECT(res == 0);
    EXPECT(!cli.isHelp());
    EXPECT(cli.isVerbose());
    EXPECT(cli.getInput() == "1");
    EXPECT(cli.getInputFile() == "tm");
    cli.reset();

    const char *argv3[] = {"", "a.tm", "-v", "110"};
    res = cli.parse(sizeof(argv3) / sizeof(char *), argv3);
    EXPECT(res == 0);
    EXPECT(!cli.isHelp());
    EXPECT(cli.isVerbose());
    EXPECT(cli.getInput() == "110");
    EXPECT(cli.getInputFile() == "a.tm");
    cli.reset();

    const char *argv4[] = {"", "a.tm", "-h", "110"};
    res = cli.parse(sizeof(argv4) / sizeof(char *), argv4);
    EXPECT(res == 0);
    EXPECT(cli.isHelp());
    EXPECT(!cli.isVerbose());
    EXPECT(cli.getInput() == "110");
    EXPECT(cli.getInputFile() == "a.tm");
    cli.reset();

    const char *argv5[] = {"", "a.tm", "-v", "110", "hello"};
    res = cli.parse(sizeof(argv5) / sizeof(char *), argv5);
    EXPECT(res < 0);
    cli.reset();

    const char *argv6[] = {"", "110"};
    res = cli.parse(sizeof(argv6) / sizeof(char *), argv6);
    EXPECT(res < 0);
    cli.reset();

    const char *argv7[] = {"", "110", "--verbose"};
    res = cli.parse(sizeof(argv7) / sizeof(char *), argv7);
    EXPECT(res < 0);
    cli.reset();
}

int main() {
    test_cli_help();
    test_cli_verbose();
    test_cli_all();
    return 0;
}
