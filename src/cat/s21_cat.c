#include "s21_cat.h"

int main(int argc, char **argv) {
  Arguments args = args_parser(argc, argv);
  int exit_code = 0;

  if (optind >= argc) {
    no_args();
  } else {
    with_args(&args, argv, &exit_code);
  }
  return 0;
}
