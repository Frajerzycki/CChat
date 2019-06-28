#include <openssl/rsa.h>
typedef struct user {
  unsigned int sd;
  RSA* public_key;
} user;
