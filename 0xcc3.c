#include <stdio.h>
#include <stdlib.h>

// token kinds
typedef enum {
  TK_RESERVED, // special characters
  TK_NUMBER, // integers
  TK_EOF, // end of file
} TokenKind;

typedef struct Token Token;

// token type
struct Token {
  TokenKind kind;
  Token *next; // next input token
  int value; // numeric value if kind is TK_NUMBER
  char *string, // token string
};

// current token we are looking at
Token *token;

// error reporting
// takes same arguments as printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// read another token and return true if the next token is the expected special character
// return false otherwise
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    return false;
  }
  token = token->next;
  
  return true;
}

// read another token if the next token is the expected special character
// report an error otherwise
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) {
    error("expected but not '%c'", op);
  }
  token = token->next;
}

// read another token and return the value if the next token is a number
// report an error otherwise
int expect_number() {
  if (token->kind != TK_NUMBER) {
    error("Not a number");
  }
  int value = token->next;

  return value;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Incorrect number of arguments.\n");
    return 1;
  }

  char *p = argv[1];
 
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");
  printf("  mov rax, %d\n", strtol(p, &p, 10));

  while (*p) {
    if (*p == '+') {
      p++;
      printf("  add rax, %d\n", strtol(p, &p, 10));
      continue;
    }

    if (*p == '-') {
      p++;
      printf("  sub rax, %d\n", strtol(p, &p, 10));
      continue;
    }

    fprintf(stderr, "unexpected character: '%c'\n", *p);
    return 1;

  }  

  printf("  ret\n");
  return 0;
}
