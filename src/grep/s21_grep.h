#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int regex;  // -e: Регулярное выражение для поиска
  int ignore_case;  // -i: Игнорировать регистр (0/1)
  int invert_match;  // -v: Инвертировать результат поиска (0/1)
  int count_only;  // -c: Выводить только количество совпадений (0/1)
  int files_only;  // -l: Выводить только имена файлов (0/1)
  int line_numbers;  // -n: Показывать номера строк (0/1)
  int no_filenames;  // -h: Не выводить имена файлов (0/1)
  int suppress_errors;  // -s: Подавлять ошибки (0/1)
  int regex_file;  // -f: Путь к файлу с регулярными выражениями
  int only_matched;  // -o: Выводить только совпадающие части строк (0/1)
  char* pattern;
  //   int len_pattern;
  //   int mem_pattern;
} FLAGS;

FLAGS args_parser(int argc, char** argv);
FLAGS* parser_switch(int* rez, FLAGS* args);
FLAGS* if_pattern_null(FLAGS* args, char** argv);
FLAGS* nofilename_activated(FLAGS* args, int argc);
void output_line(char* line, int n);
int output(FLAGS args, int argc, char** argv, int* exit_code);
void no_filename(FLAGS args, char* filename);
void count_only(FLAGS args, char* filename, int count_match);
void files_only(FLAGS args, char* filename, int count_match);
int process_file(FLAGS args, char* filename, regex_t* reg, int* exit_code);

#endif
