#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>

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
  char *string; // token string
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
  if (token->kind != TK_RESERVED || token->string[0] != op) {
    return false;
  }
  token = token->next;
  
  return true;
}

// read another token if the next token is the expected special character
// report an error otherwise
void expect(char op) {
  if (token->kind != TK_RESERVED || token->string[0] != op) {
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
  int value = token->value;
  token = token->next;

  return value;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

// generate a new token and link it to cur
Token *new_token(TokenKind kind, Token *current, char *string) {
  Token *token = calloc(1, sizeof(Token));
  token->kind = kind;
  token->string = string;
  current->next = token;

  return token;
}

// tokenize the input string p and return it
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *current = &head;

  while (*p) {
    // skip whitespaces
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      current = new_token(TK_RESERVED, current, p++);
      continue;
    }

    if (isdigit(*p)) {
      current = new_token(TK_NUMBER, current, p);
      current->value = strtol(p, &p, 10);
      continue;
    }

    error("Cannot tokenize.");
  }

  new_token(TK_EOF, current, p);
  
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Incorrect number of arguments.\n");
    return 1;
  }

  // tokenize
  token = tokenize(argv[1]);

  // assembly header
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");

  // statement must start with a number, so check that and output the first mov
  printf("   mov rax, %d\n", expect_number());

  // consume '+ <number>' or '- <number>' and output assembly
  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }  

  printf("  ret\n");
  return 0;
}
