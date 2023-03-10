#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>
#include <pthread.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include "hash.h"

clock_t begin;
pthread_mutex_t lock;

static int verbose_flag = 0;
static int newlines_flag = 0;
static char *format;

FILE *blob;
char *wordlist;
char *target_hash;
char *all_chars = "0123456789abcdef";
int count = 0;
void (*hash_fun_ptr)(char *, char *);
int found = 0;
int applied_hashing = 1;
int hash_length = 0;

static struct option long_options[] = {
    { "verbose",  no_argument,       0,               1  },
    { "format",   required_argument, 0,              'f' },
    { "wordlist", required_argument, 0,              'w' },
    { "target",   required_argument, 0,              't' },
    { "amount",   required_argument, 0,              'a' },
    { "newlines", no_argument,       0,              'n' },
    { "help",     no_argument,       0,              'h' },
    {  0,         0,                 0,               0  }
};

/**
 * Vraćanje veličine datoteke
 * @param char[] Niz poruke
 */
void manage_read(char msg_line[100])
{
    // Čitanje niza poruke iz datoteke.
    fgets(msg_line, 100, blob);

    // Vraća se ako je došlo do kraja poruke.
    if (msg_line == NULL) {
        return;
    }
}

/**
 * Worker funkcija
 * @param ptr Ulazni parametar za povratni poziv funkcije
 */
void *worker_callback(void *ptr)
{
    char msg_line[256];

    pthread_mutex_lock(&lock);
    manage_read(msg_line);
    pthread_mutex_unlock(&lock);

    while (msg_line && found == 0) {
        if (found == 1) {
            pthread_exit(0);
            return 0;
        }

        if (strlen(msg_line) == 0) {
            pthread_mutex_lock(&lock);
            manage_read(msg_line);
            pthread_mutex_unlock(&lock);
            continue;
        }

        // Eliminacija "novog reda" na kraju.
        if (newlines_flag == 0 && msg_line) {
            msg_line[strlen(msg_line) - 1] = '\0';
        }

        // Dodijeljivanje odgovarajuće memorije.
        char hash[hash_length];
        hash_fun_ptr(msg_line, hash);

        // Ponovno pokretanje hash funkcije, ali ovaj put na samom hashu.
        if (applied_hashing > 1) {
            for (int m = 0; m < applied_hashing - 1; m++) {
                hash_fun_ptr(hash, hash);
            }
        }

        if (verbose_flag != 0) {
            printf("%s -> %s%c", hash, msg_line, newlines_flag == 0 ? '\n' : '\0');
        }

        count++;

        // Uspoređivanje dobivenog hasha s ciljanim hashom.
        if (strcmp(target_hash, hash) == 0) {
            if (newlines_flag != 0) {
                printf("Rezultat je: %s", msg_line);
            } else {
                printf("Rezultat je: %s\n", msg_line);
            }

            // Dohvaćanje vremena izvršavanja.
            clock_t end = clock();
            double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

            printf("Pronađeno nakon %i pokušaja i %f sekundi!\n", count, time_spent);
            printf("Brzina je bila %.2f KHs.\n", count / time_spent / 1000);

            // Zatvaranje datoteke blob.
            if (blob) {
                fclose(blob);
            }

            exit(0);

            // Potonji kod uzrokuje pogrešku tokom pokretanja programa.

            // Signalizacija ostalim dretvama da je zadatak završen.
            pthread_mutex_lock(&lock);
            found = 1;
            pthread_mutex_unlock(&lock);

            break;
        }

        pthread_mutex_lock(&lock);
        manage_read(msg_line);
        pthread_mutex_unlock(&lock);
    }

    // Izlazak iz dretve.
    pthread_exit(0);

    return 0;
}

/**
 * Ispis poruke pomoći/upute
 */
void print_help()
{
    printf("hashcrack\n\n");
    printf("Usage:   hashcrack [options]\n\n");
    printf("Options: --wordlist, -w [filename]: The wordlist to check against\n");
    printf("         --format,   -f [hash_fun]: The hash function (md5|sha|sha1|sha256|sha512)\n");
    printf("         --target,   -t [hash]:     The hash to check\n");
    printf("         --amount,   -a [amount]:   Amount of hashing\n");
    printf("         --newlines, -n:            Whether to hash with newlines\n");
    printf("         --help,     -h:            Prints this message\n\n");
    printf("Examples:\n");
    printf("  Crack the MD5 hash of the word 'test'\n");
    printf("    hashcrack -f md5 -t 098f6bcd4621d373cade4e832627b4f6 -w /path/to/wordlist\n");
    printf("  Crack the double MD5 hash of the word 'test'\n");
    printf("    hashcrack -f md5 -t fb469d7ef430b0baf0cab6c436e70375 -a 2 -w /path/to/wordlist\n");
    printf("  Crack the MD5 hash of the word 'test' that's followed by a newline\n");
    printf("    hashcrack -f md5 -t d8e8fca2dc0f896fd7cb4cb0031ba249 -n -w /path/to/wordlist\n\n");
}

/**
 * Funkcija main
 *
 * @param c Zastavica argumenta
 * @param option_index Indeks opcije
 */
void process_args(int c, int option_index)
{
    switch(c) {
        case 0:
            break;
        case 'f':
            format = optarg;

            if (strcmp(format, "sha512") == 0) {
                hash_fun_ptr = &sha512;
                hash_length = 128;
            }
            else if (strcmp(format, "sha256") == 0) {
                hash_fun_ptr = &sha256;
                hash_length = 65;
            }
            else if (strcmp(format, "sha1") == 0) {
                hash_fun_ptr = &sha1;
                hash_length = 40;
            }
            else if (strcmp(format, "md5") == 0) {
                hash_fun_ptr = &md5;
                hash_length = 33;
            }
            else {
                printf("Nepoznat format %s.\n", format);
                exit(1);
            }

            break;
        case 'w':
            wordlist = (char *) malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(wordlist, optarg);
            break;
        case 't':
            target_hash = (char *) malloc((strlen(optarg) + 1) * sizeof(char));
            strcpy(target_hash, optarg);
            break;
        case 'n':
            newlines_flag = 1;
            break;
        case 'a':
            applied_hashing = atoi(optarg);

            if (applied_hashing < 1) {
                applied_hashing = 1;
            }

            break;
        case 'v':
            verbose_flag = 1;
            break;
        case 'h':
            print_help();
            exit(1);
    }
}

/**
 * Funkcija main
 *
 * @param argc Broj argumenata
 * @param argv Lista argumenata
 * @return Status programa
 */
int main(int argc, char **argv)
{
    begin = clock();
    int option_index = 0;

    // Dohvaćanje broja jezgri.
    long int cores = sysconf(_SC_NPROCESSORS_ONLN);

    printf("Kreiranje %ld dretvi.\n", cores);

    pthread_mutex_init(&lock, NULL);
    int c;

    while ((c = getopt_long(argc, argv, "f:w:t:a:nhv", long_options, &option_index)) != -1) {
        process_args(c, option_index);
    }

    // Ispis funkcije help ukoliko nedostaje argument.
    if (!wordlist || !format || !target_hash) {
        print_help();
        return 1;
    }

    printf("Čitanje %s i dobivanje %s.\n", wordlist, format);

    if (newlines_flag != 0) {
        printf("Sa novim redovima.\n");
    }

    if (applied_hashing > 1) {
        printf("Hashiranje %i puta.\n", applied_hashing);
    }

    FILE *msgs = fopen(wordlist, "r");
    blob = msgs;

    if (msgs == NULL) {
        printf("Neispravna datoteka liste riječi.\n");
        return 1;
    }

    // Kreiranje onoliko dretvi koliko ima jezgri.
    for (int i = 0; i < cores; i++) {
        pthread_t worker;

        // Kreiranje dretve.
        pthread_create(&worker, NULL, worker_callback, NULL);
    }

    pthread_exit(NULL);

    return 0;
}