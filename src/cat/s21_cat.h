#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @param number_nonblank b or number-nonblank
 * @param number_all n or number
 * @param squeeze_blanks s or squeeze-blank
 * @param show_ends e and v or E
 * @param show_tabs t and v or T
 * @param enable_visibl v
 * */
typedef struct Arguments {
  bool number_nonblank;  // -b or --number-nonblank
  bool number_all;       // -n or --number
  bool squeeze_blanks;   // -s or --squeeze-blank
  bool show_ends;        // -e and -v or only -E
  bool show_tabs;        // -t and -v or only -T
  bool enable_visible;   // -v
} Arguments;

Arguments args_parser(int argc, char** argv);
void no_args();
int with_args(Arguments* args, char** argv, int* exit_code);
void output_file(Arguments* const args, char* line, int n);
int is_empty_line(int read, char* line);
void number_print(int* line_count);
void number_lines(Arguments* args, char* line, int read, int* is_empty,
                  int* line_count);
void print_symbols(Arguments* args, char* line, int count);
char print_invisible_symbols(char line_ch);
void print_tabs();
void print_end();

#endif