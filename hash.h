#ifndef HASH_H
#define HASH_H

// Pretvaranje hash-a u heksadecimalni zapis.
void hash_to_hex(unsigned char *, char *, int);

// Funkcije hashiranja.
void sha512(char *, char[128]);
void sha256(char *, char[65]);
void sha1(char *, char[40]);
void md5(char *, char[33]);

#endif