#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_CHARS 100

struct detaljiLozinke
{
    char lozinka[MAX_CHARS]; // pretpostavka da nečija lozinka neće biti veća od 100 znakova
    double potrebnoVrijeme;
    long int pokusaji;
    int bfProbijen;  // probijanje sa brute force algoritmom
    int nbfProbijen; // probijanje sa pre brute force algoritmom
    short int identifikator;
};

int main(void)
{

    int probijeneLozinke = 0;
    int preBruteForceProbijeneLozinke = 0;
    int brojLozinki;

    printf("Koliko lozinki zelite testirati?\n");
    scanf("%d", &brojLozinki);

    printf("Unesite %d lozinki:\n", brojLozinki);

    struct detaljiLozinke listaLozinki[brojLozinki];
    for (int i = 0; i < brojLozinki; i++)
    {
        struct detaljiLozinke trenutnaLozinka;
        scanf("%s", trenutnaLozinka.lozinka);
        trenutnaLozinka.pokusaji = 0;
        trenutnaLozinka.bfProbijen = 0;
        trenutnaLozinka.nbfProbijen = 0;
        trenutnaLozinka.identifikator = i + 1;
        listaLozinki[i] = trenutnaLozinka;
    }

    long int pokusaji = 0;

    // pre brute force algoritam
    // txt datoteka sa https://github.com/danielmiessler/SecLists/tree/master/Passwords/Common-Credentials
    char lozinkeDatoteke[MAX_CHARS];
    FILE *ld;
    int pronadeneLozinke = 0;
    int duljina;
    int preBruteForcePokusaji = 0;

    ld = fopen("pass.txt", "r");
    if (ld == NULL)
    {
        perror("Pogreska pri otvaranju datoteke!");
        return 1;
    }

    time_t pocetak, kraj;
    double potrebnoVrijeme;
    pocetak = clock();

    // struct detaljiLozinke listaLozinki[] = {pass1, pass2, pass3};

    while (fgets(lozinkeDatoteke, MAX_CHARS, ld) != NULL)
    {
        preBruteForcePokusaji++;
        duljina = strlen(lozinkeDatoteke);
        if (duljina > 0 && lozinkeDatoteke[duljina - 1] == '\n')
            lozinkeDatoteke[duljina - 1] = '\0';

        for (int i = 0; i < brojLozinki; i++)
        {
            if (listaLozinki[i].nbfProbijen != 1)
            {
                if (strcmp(listaLozinki[i].lozinka, lozinkeDatoteke) == 0)
                {
                    kraj = clock();
                    kraj = kraj - pocetak;
                    listaLozinki[i].potrebnoVrijeme = ((double)kraj) / CLOCKS_PER_SEC;
                    printf("Lozinka %d pronadjena!\n", listaLozinki[i].identifikator);
                    listaLozinki[i].nbfProbijen = 1;
                    listaLozinki[i].pokusaji = preBruteForcePokusaji;
                    probijeneLozinke++;
                    preBruteForceProbijeneLozinke++;
                }
            }
        }
        if (preBruteForceProbijeneLozinke == brojLozinki)
            break;
    }

    fclose(ld);

    for (int i = 0; i < brojLozinki; i++)
    {
        if (listaLozinki[i].nbfProbijen == 1)
        {
            printf("Za lozinku %d je trebalo %ld pokusaja.\n", listaLozinki[i].identifikator, listaLozinki[i].pokusaji);
            printf("%f sekundi je trajalo probijanje.\n", listaLozinki[i].potrebnoVrijeme);
            if (listaLozinki[i].pokusaji <= 10)
            {
                printf("Ova lozinka je u top 10 najpopularnijih lozinki!\n");
            }
            else if (listaLozinki[i].pokusaji > 10 && listaLozinki[i].pokusaji <= 100)
            {
                printf("Ova lozinka je u top 100 najpopularnijih lozinki!\n");
            }
            else if (listaLozinki[i].pokusaji > 100 && listaLozinki[i].pokusaji <= 1000)
            {
                printf("Ova lozinka je u top 1000 najpopularnijih lozinki!\n");
            }
            else if (listaLozinki[i].pokusaji > 1000 && listaLozinki[i].pokusaji <= 10000)
            {
                printf("Ova lozinka je u top 10 000 najpopularnijih lozinki!\n");
            }
            else if (listaLozinki[i].pokusaji > 10000 && listaLozinki[i].pokusaji <= 100000)
            {
                printf("Ova lozinka je u top 100 000 najpopularnijih lozinki!\n");
            }
            else
            {
                printf("Ova lozinka je u top 1 000 000 najpopularnijih lozinki!\n");
            }
        }
    }

    if (preBruteForceProbijeneLozinke == brojLozinki)
        return 0;

    printf("Koristi se brute force algoritam!\n");

    char pogadacLozinki[MAX_CHARS]; // pretpostavka da nečija lozinka neće biti veća od 100 znakova
    pogadacLozinki[0] = '0';
    pogadacLozinki[1] = '\0';
    pokusaji = 0;
    int duljinaTrenutneLozinke = 1;
    int elementPoljaTrenutneLozinke = 0;
    int m = 0;
    int j = 0;
    int x = 0;

    while (1)
    {
        for (int i = 0; i < brojLozinki; i++)
        {
            if (strcmp(listaLozinki[i].lozinka, pogadacLozinki) == 0)
            {
                if (listaLozinki[i].bfProbijen != 1 && listaLozinki[i].nbfProbijen != 1)
                {
                    printf("Lozinka %d pronadjena!\n", listaLozinki[i].identifikator);
                    kraj = clock();
                    kraj = kraj - pocetak;
                    listaLozinki[i].potrebnoVrijeme = ((double)kraj) / CLOCKS_PER_SEC;
                    listaLozinki[i].bfProbijen = 1;
                    listaLozinki[i].pokusaji = pokusaji;
                    probijeneLozinke++;
                    printf("Za lozinku %d je trebalo %ld pokusaja.\n", listaLozinki[i].identifikator, listaLozinki[i].pokusaji);
                    printf("Za lozinku %d je trebalo %f sekundi da se probije.\n\n", listaLozinki[i].identifikator, listaLozinki[i].potrebnoVrijeme);
                }
            }
        }

        if (probijeneLozinke == brojLozinki)
        {
            printf("Sve lozinke su pronadjene!\n");
            break;
        }

        if (pogadacLozinki[elementPoljaTrenutneLozinke] == '9')
        {
            pogadacLozinki[elementPoljaTrenutneLozinke] = 'A';
            // resetiranje
            if (elementPoljaTrenutneLozinke > 0)
            {
                for (m = 0; m < elementPoljaTrenutneLozinke; m++)
                {
                    pogadacLozinki[m] = '0';
                }
            }
            elementPoljaTrenutneLozinke = 0;
        }
        else if (pogadacLozinki[elementPoljaTrenutneLozinke] == 'Z')
        {
            pogadacLozinki[elementPoljaTrenutneLozinke] = 'a';
            if (elementPoljaTrenutneLozinke > 0)
            {
                for (m = 0; m < elementPoljaTrenutneLozinke; m++)
                {
                    pogadacLozinki[m] = '0';
                }
            }
            elementPoljaTrenutneLozinke = 0;
        }
        else if (pogadacLozinki[elementPoljaTrenutneLozinke] == 'z')
        {
            elementPoljaTrenutneLozinke++;
            if (elementPoljaTrenutneLozinke == duljinaTrenutneLozinke)
            {
                pogadacLozinki[duljinaTrenutneLozinke] = '0';
                duljinaTrenutneLozinke++;
                pogadacLozinki[duljinaTrenutneLozinke] = '\0';
                for (x = 0; x < duljinaTrenutneLozinke - 1; x++)
                {
                    pogadacLozinki[x] = '0';
                }
                elementPoljaTrenutneLozinke = 0;
            }
            else
            {
                for (m = 0; m < elementPoljaTrenutneLozinke; m++)
                {
                    pogadacLozinki[m] = '0';
                }
            }
        }
        else
        {
            pogadacLozinki[elementPoljaTrenutneLozinke]++;
            if (elementPoljaTrenutneLozinke > 0)
            {
                for (m = 0; m < elementPoljaTrenutneLozinke; m++)
                {
                    pogadacLozinki[m] = '0';
                }
                elementPoljaTrenutneLozinke = 0;
            }
        }

        pokusaji++;
    }

    return 0;
}