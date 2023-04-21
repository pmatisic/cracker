# cracker  

[![MIT License](https://img.shields.io/github/license/pmatisic/cracker?color=000000&style=flat-square)](https://github.com/pmatisic/cracker/blob/master/LICENSE)  

© Faculty of Organization and Informatics, University of Zagreb, 2022.  

---

### Kompajliranje

Aplikacije je moguće kompajlirati sa:
```bash
$ make
```

### Pokretanje

Ako želite testirati aplikaciju `hashcrack`, onda je potrebno pokrenuti:
```bash
$ ./hashcrack
```

U nastavku je naveden primjer konkretnog testiranja aplikacije:
```bash
$ ./hashcrack -f md5 -t 098f6bcd4621d373cade4e832627b4f6 -w pass.txt 
```

U _helper_-u aplikacije su navedeni primjeri korištenja:
```
Usage:   hashcrack [options]

Options: --wordlist, -w [filename]: The wordlist to check against
         --format,   -f [hash_fun]: The hash function (md5|sha|sha1|sha256|sha512)
         --target,   -t [hash]:     The hash to check
         --amount,   -a [amount]:   Amount of hashing
         --newlines, -n:            Whether to hash with newlines
         --help,     -h:            Prints this message

Examples:
  Crack the MD5 hash of the word 'test'
    hashcrack -f md5 -t 098f6bcd4621d373cade4e832627b4f6 -w /path/to/wordlist
  Crack the double MD5 hash of the word 'test'
    hashcrack -f md5 -t fb469d7ef430b0baf0cab6c436e70375 -a 2 -w /path/to/wordlist
  Crack the MD5 hash of the word 'test' that's followed by a newline
    hashcrack -f md5 -t d8e8fca2dc0f896fd7cb4cb0031ba249 -n -w /path/to/wordlist
```

Za aplikaciju `passcrack` je dovoljno pokrenuti:
```bash
$ ./pass
```

Tekstualne datoteke potrebne za izvođenje aplikacija mogu se preuzeti [ovdje](https://github.com/danielmiessler/SecLists/tree/master/Passwords).
