#include "s21_cat.h"

Arguments args_parser(int argc, char** argv) {
  Arguments args = {false};
  const struct option l_arg[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                 {"number", no_argument, NULL, 'n'},
                                 {"squeeze-blank", no_argument, NULL, 's'},
                                 {0, 0, 0, 0}};
  int arg;
  while ((arg = getopt_long(argc, argv, "beEnstTv", l_arg, NULL)) != -1) {
    switch (arg) {
      case 'b':
        args.number_nonblank = true;
        break;
      case 'n':
        args.number_all = true;
        break;
      case 's':
        args.squeeze_blanks = true;
        break;
      case 'e':
        args.show_ends = true;
        args.enable_visible = true;
        break;
      case 't':
        args.show_tabs = true;
        args.enable_visible = true;
        break;
      case 'E':
        args.show_ends = true;
        break;
      case 'T':
        args.show_tabs = true;
        break;
      case '?':
      default:
        break;
    }
  }
  return args;
}

void no_args() {
  char ch;
  while ((ch = getchar()) != EOF) {
    putchar(ch);
  }
}

/**
 * @note optind - индекс нефлагового аргумента в argv[]
 * @note getline - возвращает количество прочитанных символов
 */
int with_args(Arguments* args, char** argv, int* exit_code) {
  FILE* file = fopen(argv[optind], "r");
  if (!file) {
    perror("fopen");
    *exit_code = 1;
    ;
  }

  if (*exit_code == 0) {
    char* line = NULL;
    size_t line_size = 0;
    ssize_t read;
    read = getline(&line, &line_size, file);

    int line_count = 1;
    int is_empty = 0;
    while (read != -1) {
      number_lines(args, line, read, &is_empty, &line_count);
      if (!(args->squeeze_blanks && is_empty > 1)) {
        output_file(args, line, read);
      }
      read = getline(&line, &line_size, file);
    }
    free(line);
    fclose(file);
  }
  return *exit_code;
}

void output_file(Arguments* const args, char* line, int n) {
  for (int i = 0; i < n; i++) {
    if (args->show_tabs && line[i] == '\t') {
      print_tabs();
    } else if (args->show_ends && line[i] == '\n') {
      print_end();
    } else {
      if (args->enable_visible) {
        line[i] = print_invisible_symbols(line[i]);
      }
      putchar(line[i]);
    }
  }
}

/**
 * @note n and b arguments function implementation
 */
void number_lines(Arguments* args, char* line, int read, int* is_empty,
                  int* line_count) {
  if (is_empty_line(read, line))
    (*is_empty)++;
  else
    *is_empty = 0;

  if (args->number_nonblank) {
    if (!(*is_empty)) {
      number_print(line_count);
      (*line_count)++;
    }
  } else if (args->number_all) {
    number_print(line_count);
    (*line_count)++;
  }
}

/**
 * @note for -t arg
 */
void print_tabs() { printf("^I"); }

/**
 * @note for -e arg
 */
void print_end() {
  putchar('$');
  putchar('\n');
}

/**
 * @note for -v arg
 */
char print_invisible_symbols(char line_ch) {
  if (line_ch == '\n' || line_ch == '\t') return line_ch;
  if (line_ch < 0) {
    printf("M-");
    line_ch = line_ch & 0x7F;
  }
  if (line_ch >= 0 && line_ch <= 31) {
    putchar('^');
    line_ch += 64;
  } else if (line_ch == 127) {
    putchar('^');
    line_ch = '?';
  }
  return line_ch;
}

/**
 * @return если строка пуста
 */
int is_empty_line(int read, char* line) {
  return (read == 1 && line[0] == '\n');
}

void number_print(int* line_count) { printf("%6d\t", *line_count); }