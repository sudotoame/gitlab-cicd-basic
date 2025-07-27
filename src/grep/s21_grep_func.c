#include "s21_grep.h"

FLAGS args_parser(int argc, char** argv) {
  FLAGS args = {0};
  int rez = 0;
  while ((rez = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    parser_switch(&rez, &args);
  }
  if_pattern_null(&args, argv);
  nofilename_activated(&args, argc);
  return args;
}

FLAGS* parser_switch(int* rez, FLAGS* args) {
  switch (*rez) {
    case 'e':
      args->regex = 1;
      args->pattern = optarg;  // optarg - значение аргумента
      break;
    case 'i':
      args->ignore_case = REG_ICASE;
      break;
    case 'v':
      args->invert_match = 1;
      break;
    case 'c':
      args->count_only = 1;
      break;
    case 'l':
      args->files_only = 1;
      args->count_only = 1;
      break;
    case 'n':
      args->line_numbers = 1;
      break;
    case 'h':
      args->no_filenames = 1;
      break;
    case 's':
      args->suppress_errors = 1;
      break;
    // case 'f':
    //   args->regex_file = 1;
    //   //
    //   break;
    // case 'o':
    //   args->only_matched = 1;
    //   break;
    case '?':
      break;
  }
  return args;
}

FLAGS* if_pattern_null(FLAGS* args, char** argv) {
  if (args->pattern == NULL) {
    args->pattern = argv[optind];
    optind++;
  }
  return args;
}

FLAGS* nofilename_activated(FLAGS* args, int argc) {
  if (argc - optind == 1) {
    args->no_filenames = 1;
  }
  return args;
}

void output_line(char* line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

int output(FLAGS args, int argc, char** argv, int* exit_code) {
  regex_t re;
  // 0 если есть совпадения, любое другое значение ошибка
  int error = regcomp(&re, args.pattern, args.ignore_case);

  if (error) {
    perror("Error\n");
    *exit_code = 1;
  }
  if (*exit_code == 0) {
    for (int i = optind; i < argc; i++) {
      process_file(args, argv[i], &re, exit_code);
    }
    regfree(&re);
  }
  return *exit_code;
}

int process_file(FLAGS args, char* filename, regex_t* reg, int* exit_code) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    if (args.suppress_errors == 0) {
      perror(filename);
    }
    *exit_code = 1;
  }
  if (*exit_code == 0) {
    char* line = NULL;
    size_t line_len = 0;
    ssize_t read = 0;
    int count_line = 1;
    int count_match = 0;

    while ((read = getline(&line, &line_len, file)) != -1) {
      int is_match = regexec(reg, line, 0, NULL, 0);

      if (args.invert_match) is_match = !is_match;  // инвертируем совпадения

      if (is_match == 0) {
        count_match++;
        if (args.count_only == 0 && args.files_only == 0) {
          if (args.line_numbers) {
            printf("%d:", count_line);
          }
          output_line(line, read);
        }
      }
      count_line++;
    }
    free(line);

    count_only(args, filename, count_match);
    files_only(args, filename, count_match);

    fclose(file);
  }
  return *exit_code;
}

void no_filename(FLAGS args, char* filename) {
  if (args.no_filenames == 0) {
    printf("%s:", filename);
  }
}

void count_only(FLAGS args, char* filename, int count_match) {
  if (args.count_only && args.files_only == 0) {
    no_filename(args, filename);
    printf("%d\n", count_match);
  }
}

void files_only(FLAGS args, char* filename, int count_match) {
  if (args.files_only && count_match > 0) {
    printf("%s\n", filename);
  }
}