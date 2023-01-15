#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include "hash.h"

/**
 * Funkcija za pretvaranje hash-a u heksadecimalni zapis.
 *
 * @param hash Hash za pretvaranje u hex
 * @param output_buffer Izlazni međuspremnik
 * @param length Duljina međuspremnika
 */
void hash_to_hex(unsigned char *hash, char *output_buffer, int length)
{
    int i = 0;

    for (i = 0; i < length; i++)
    {
        sprintf(output_buffer + (i * 2), "%02x", hash[i]);
    }

    output_buffer[length * 2] = 0;
}

/**
 * Funkcija za kreiranje SHA512 hash-a.
 *
 * @param string String za pretvaranje u hash
 * @param buffer Izlazni međuspremnik
 */
void sha512(char *string, char output_buffer[128])
{
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512_CTX sha512;
    SHA512_Init(&sha512);
    SHA512_Update(&sha512, string, strlen(string));
    SHA512_Final(hash, &sha512);

    // Izrada hex zapisa.
    hash_to_hex(hash, output_buffer, SHA512_DIGEST_LENGTH);
}

/**
 * Funkcija za kreiranje SHA256 hash-a.
 *
 * @param string String za pretvaranje u hash
 * @param buffer Izlazni međuspremnik
 */
void sha256(char *string, char output_buffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);

    // Izrada hex zapisa.
    hash_to_hex(hash, output_buffer, SHA256_DIGEST_LENGTH);
}

/**
 * Funkcija za kreiranje SHA1 hash-a.
 *
 * @param string String za pretvaranje u hash
 * @param buffer Izlazni međuspremnik
 */
void sha1(char *string, char output_buffer[40])
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha;
    SHA1_Init(&sha);
    SHA1_Update(&sha, string, strlen(string));
    SHA1_Final(hash, &sha);

    // Izrada hex zapisa.
    hash_to_hex(hash, output_buffer, SHA_DIGEST_LENGTH);
}

/**
 * Funkcija za kreiranje MD5 hash-a.
 *
 * @param string String za pretvaranje u hash
 * @param buffer Izlazni međuspremnik
 */
void md5(char *string, char output_buffer[33])
{
    unsigned char hash[MD5_DIGEST_LENGTH];
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, string, strlen(string));
    MD5_Final(hash, &md5);

    // Izrada hex zapisa.
    hash_to_hex(hash, output_buffer, MD5_DIGEST_LENGTH);
}