#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct flags {
  int e, i, v, c, l, n, h, s, f, o;
  char tampler[1024];
  int len_tampler;
} flags;

void flag_parsing(int argc, char **argv, flags *flag);
void open_file(const flags *flag, char *nfile, regex_t *regex, int argc);
void print_grep(char **all_tamplates, const int *num_tamp, FILE *file,
                char *name_file, int print_name);
void tamplers_from_file(flags *flag, char *nfile);
void add_tamplers(flags *flag, char *tampler);
void print_match(regex_t *regex, char *sh);
void output(flags *flag, int argc, char **argv);

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Usage: grep [-eivclnhsfo] [FILE ...]\n");
  } else {
    flags flag = {0};
    flag_parsing(argc, argv, &flag);
    if (!flag.len_tampler) {
      add_tamplers(&flag, argv[optind]);
      optind++;
    }
    output(&flag, argc, argv);
  }
  return 0;
}

void flag_parsing(int argc, char **argv, flags *flag) {
  int options;
  while ((options = getopt(argc, argv, "ivclnhsf:oe:")) != -1) {
    switch (options) {
      case 'e':
        flag->e = 1;
        add_tamplers(flag, optarg);
        break;
      case 'i':
        flag->i = REG_ICASE;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        tamplers_from_file(flag, optarg);
        break;
      case 'o':
        flag->o = 1;
        break;
      case '?':
        printf(
            "grep: invalid option -- '%c'\n Usage: grep [OPTION]... tamplers "
            "[FILE]...\nTry 'grep --help' for more information.",
            optopt);
        break;
    }
  }
}

void add_tamplers(flags *flag, char *tampler) {
  if (flag->len_tampler) {
    strcat(flag->tampler + flag->len_tampler, "|");
    flag->len_tampler++;
  }
  flag->len_tampler +=
      sprintf(flag->tampler + flag->len_tampler, "(%s)", tampler);
}

void tamplers_from_file(flags *flag, char *ffile) {
  FILE *file = fopen(ffile, "r");
  if (!file) {
    if (!flag->s) {
      perror(ffile);
    }
    exit(1);
  }
  char *sh = NULL;
  size_t length = 0;
  int read = getline(&sh, &length, file);
  while (read != -1) {
    if (sh[read - 1] == '\n') sh[read - 1] = '\0';
    add_tamplers(flag, sh);
    read = getline(&sh, &length, file);
  }
  free(sh);
  fclose(file);
}

void open_file(const flags *flag, char *nfile, regex_t *regex, int argc) {
  FILE *file = fopen(nfile, "r");
  if (!file) {
    if (!flag->s) {
      perror(nfile);
    }
    exit(1);
  }
  char *sh = NULL;
  size_t length = 0;
  int read = getline(&sh, &length, file), line_count = 1, match_count = 0;
  while (read != -1) {
    int result = regexec(regex, sh, 0, NULL, 0);
    if ((!result && !flag->v) || (result && flag->v)) {
      match_count++;
      if (!flag->c && !flag->l) {
        if ((!flag->h && argc - optind > 1) && (!flag->o || !flag->v))
          printf("%s:", nfile);
        if ((flag->n) && (!flag->o || !flag->v)) printf("%d:", line_count);
        if (flag->o && !flag->v)
          print_match(regex, sh);
        else if ((!flag->o && !flag->v) || (!flag->o && flag->v)) {
          for (int i = 0; i < read; i++) {
            printf("%c", sh[i]);
          }
          if (sh[read - 1] != '\n') printf("\n");
        }
      }
    }
    read = getline(&sh, &length, file);
    line_count++;
  }
  if (flag->c) {
    if (!flag->h && argc - optind > 1 && !flag->l) printf("%s:", nfile);
    if (flag->l && match_count)
      match_count = 1;
    else
      printf("%d\n", match_count);
  }
  if ((flag->l && match_count)) printf("%s\n", nfile);
  free(sh);
  fclose(file);
}

void print_match(regex_t *regex, char *sh) {
  regmatch_t match;
  int offset = 0, flag = 1;
  while (flag) {
    int result = regexec(regex, sh + offset, 1, &match, 0);
    if (result)
      flag = 0;
    else {
      for (int i = match.rm_so; i < match.rm_eo; i++) {
        printf("%c", sh[i + offset]);
      }
      printf("\n");
      offset += match.rm_eo;
    }
  }
}

void output(flags *flag, int argc, char **argv) {
  regex_t regex;
  int error = 0;
  error = regcomp(&regex, flag->tampler, REG_EXTENDED + flag->i);
  if (error) {
    perror("Error");
    exit(1);
  }
  for (int i = optind; i < argc; i++) {
    open_file(flag, argv[i], &regex, argc);
  }
  regfree(&regex);
}