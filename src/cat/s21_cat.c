#include <stdio.h>
#include <string.h>

int quantity_flag(int argc, char **argv);
void file_print(int argc, char **argv);
int checking_flag(int argc, char **argv, int *flag);
void print_file(int num_flag, const int *flag, int argc, char **argv);

int main(int argc, char **argv) {
  int num_flag = 0;
  char text;
  num_flag = quantity_flag(argc, argv);
  if (num_flag != 0) {
    int flag[8] = {0, 0, 0, 0, 0, 0, 0};
    int incorr;
    incorr = checking_flag(num_flag, argv, flag);
    if (incorr == 0) {
      // printf("%d\n", flag[4]);
      print_file(num_flag, flag, argc, argv);
    }
  } else {
    if (argc == 1) {
      while (scanf("%c", &text) != '\n') {
        printf("%c", text);
      }
    } else {
      file_print(argc, argv);
    }
  }
}

int quantity_flag(int argc, char **argv) {
  int sum_flag = 1;
  for (int i = 1; i < argc;) {
    if (argv[i][0] == '-') {
      sum_flag++;
      i++;
    } else
      i = argc;
  }
  return sum_flag;
}

int checking_flag(int num_flag, char **argv, int *flag) {
  int sum_flag = 1;
  int incorr = 0;
  int k = 0;
  const char *existing_flags[8] = {"b", "e", "n", "s", "t", "v", "E", "T"};
  for (int i = 1; i < num_flag;) {
    int len = strlen(argv[i]);
    for (int j = 1; j < len; j++) {
      for (int m = 0; m < 8; m++) {
        if (strstr(argv[i], existing_flags[m]) != NULL) {
          flag[m] = 1;
          sum_flag++;
        }
      }
      if (k == 0) {
        if (!(argv[i][j] == 'b' || argv[i][j] == 'e' || argv[i][j] == 'v' ||
              argv[i][j] == 't' || argv[i][j] == 'n' || argv[i][j] == 's' ||
              argv[i][j] == 'E' || argv[i][j] == 'T')) {
          printf(
              "s21_cat: illegal option -- %c\nusage: cat [-benstuv] [file "
              "...]\n",
              argv[i][j]);
          incorr = j;
          k = 1;
        }
      }
    }
    i++;
    if (sum_flag != i) {
      sum_flag++;
      i = num_flag + 1;
    } else
      incorr = 0;
  }
  return incorr;
}

void file_print(int argc, char **argv) {
  char ch[1024];
  for (int i = 1; i < argc; i++) {
    FILE *file = fopen(argv[i], "r");
    if (file != NULL) {
      while (fgets(ch, 1024, file) != NULL) {
        printf("%s", ch);
      }
      fclose(file);
    } else {
      printf("s21_cat: %s: No such file or directory\n", argv[i]);
    }
  }
}

void print_file(int num_flag, const int *flag, int argc, char **argv) {
  int last_enter = '\n';
  int s = 0;
  int next = 1;
  for (int i = num_flag; i < argc; i++) {
    FILE *file;
    file = fopen(argv[i], "r");
    if (file != NULL) {
      int number = 1;
      int ch;
      while ((ch = fgetc(file)) != EOF) {
        if ((flag[3]) && ch == '\n' && last_enter == '\n') {
          s++;
          if (s > 1) next = 0;
        } else {
          s = 0;
          next = 1;
        }
        if (((flag[2] && !(flag[0])) || ((flag[0]) && ch != '\n')) &&
            last_enter == '\n') {
          if (!flag[3]) {
            printf("%6d\t", number++);
          } else {
            if (s > 1)
              next = 0;
            else
              printf("%6d\t", number++);
          }
        }
        if ((flag[4] || flag[7]) && ch == '\t') {
          printf("^");
          ch = 'I';
        }
        if ((flag[1] || flag[6]) && ch == '\n') {
          if (!flag[3])
            printf("$");
          else {
            if (s > 1)
              next = 0;
            else
              printf("$");
          }
        }
        if (flag[5] || flag[1] || flag[4]) {
          if ((ch >= 0 && ch < 9) || (ch > 10 && ch < 32)) {
            printf("^");
            ch += 64;
          }
        }
        last_enter = ch;
        if (next) putchar(ch);
      }
      fclose(file);
    } else
      printf("s21_cat: %s: No such file or directory\n", argv[i]);
  }
}
