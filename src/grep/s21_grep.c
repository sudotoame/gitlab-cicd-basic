#include "s21_grep.h"

int main(int argc, char* argv[]) {
  FLAGS args = args_parser(argc, argv);
  int exit_code = 0;

  if (optind >= argc) {
    fprintf(stderr, "Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
    exit_code = 1;
  }

  if (exit_code == 0) output(args, argc, argv, &exit_code);

  return exit_code;
}