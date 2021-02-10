#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

void runApp();
bool sprawdzUzytkownika(char log[], char pass[]);
void kontrolerGlowneMenu(char login[], char pass[]);
void kontroler(char login[], char pass[]);
int opcjaGlowneMenu = 0;


struct KontoBankowe {
    char imie[20];
    char nazwisko[20];
    int dzien;
    int miesiac;
    int rok;
    char nazwaUzytkownika[50];
    double stanKonta;
    double stanKontaOszczednosciowego;
    char numerKonta[100];
    char numerKontaOszczednosciowego[100];
    char adres[200];
    char haslo[100];
};


struct Przelew {
    char imieOdbiorcy[20];
    char nazwiskoOdbiorcy[20];
    double kwota;
    char numerKontaOdbiorcy[100];
};

struct HistoriaPrzelewow {
    char imieW[20];
    char nazwiskoW[20];
    double kwota;
    char imieO[20];
    char nazwiskoO[20];
    char date[200];
};


struct KontoBankowe db[999] = {};
int i = 1; // start od 1 bo 0 miejsce jest zarezerwowane dla konta tworzonego automatycznie przy starcie aplikacji

struct HistoriaPrzelewow historia[999] = {};
int index_historii = 0;

char * substring(char text[], int start, int koniec) {
    char* substr = malloc(strlen(text));
    int x = 0;
    for (int i=start; i<=koniec; i++) {
        substr[x] = text[i];
        x++;
    }
    return substr;
}

void szyfruj1(char string[]) {
    for (int i=0; i<strlen(string); i++) {
        string[i] = string[i] + 5;
    }
}

void odszyfruj1(char string[]) {
    for (int i=0; i<strlen(string); i++) {
        string[i] = string[i] - 5;
    }
}

// DB INIT
void inicjalizujBazeDanych() {
    struct KontoBankowe uzytkownik;
    strcpy(uzytkownik.imie, "Michal");
    strcpy(uzytkownik.nazwisko, "Malek");
    uzytkownik.dzien = 7;
    uzytkownik.miesiac = 9;
    uzytkownik.rok = 1998;
    strcpy(uzytkownik.nazwaUzytkownika, "Miszu98");
    char pass[] = "123";
    szyfruj1(pass);
    strcpy(uzytkownik.haslo, pass);
    uzytkownik.stanKonta = 5000;
    uzytkownik.stanKontaOszczednosciowego = 0;
    strcpy(uzytkownik.numerKonta, "87653456987600563498121234");
    strcpy(uzytkownik.numerKontaOszczednosciowego, "45678998777623112499878765");
    strcpy(uzytkownik.adres, "ul. Kwiatowa 23, Piekary Slaskie");
    db[0] = uzytkownik;
}

// MENU 1
void menu() {
    printf("\n");
    printf("\n");
    printf("                        # # # # # # # # # # # # # # # # # # # # # # # # # # # # \n");
    printf("                      #                                                         #\n");
    printf("                      #            * * *       *       *    *   *   *           #\n");
    printf("                      #            *    *     * *      * *  *   *  *            #\n");
    printf("                      #            *   *     *   *     *  * *   * *             #\n");
    printf("                      #            *    *   * * * *    *   **   *  *            #\n");
    printf("                      #            * * *   *       *   *    *   *   *           #\n");
    printf("                      #                                                         #\n");
    printf("                        # # # # # # # # # # # # # # # # # # # # # # # # # # # #\n");
    printf("                               #                                       #\n");
    printf("                               #                                       #\n");
    printf("                               #       1) Zaloguj sie                  #\n");
    printf("                               #       2) Zarejestruj sie              #\n");
    printf("                               #       3) Zmien haslo                  #\n");
    printf("                               #       4) Wyjdz                        #\n");
    printf("                               #                                       #\n");
    printf("                               #                                       #\n");
    printf("                               #                                       #\n");
    printf("                                # # # # # # # # # # # # # # # # # # # #\n");
}


char * generujNumerKonta() {
    char genNumerKonta[30] = "";
    char liczba[5];
    srand(time(NULL));
    for (int x=0; x<26; x++) {
        int random = rand() % 10;
        itoa(random, liczba, 10);
        strcat(genNumerKonta, liczba);
    }
    return strdup(genNumerKonta);
}

char * generujNumerKontaOszczednosciowego() {
    char genNumerKonta[30] = "";
    char liczba[5];
    srand(time(NULL));
    for (int x=0; x<26; x++) {
        int random = rand() % 9;
        itoa(random, liczba, 10);
        strcat(genNumerKonta, liczba);
    }
    return strdup(genNumerKonta);
}

char * pobierzDate() {
    time_t current_time;
    char * c_time_string;

    current_time = time(NULL);

    c_time_string = ctime(&current_time);
    return c_time_string;
}

// Obsługa błędów rejestracji
bool sprawdzHaslo(char haslo[]) {
    int literyMale = 0;
    int literyDuze = 0;
    int znakiSpecjalne = 0;
    int liczby = 0;

    int x = 0;
    while (x < strlen(haslo)) {
        if (haslo[x] >= 'a' && haslo[x] <= 'z') {
            literyMale++;
        } else if (haslo[x] >= 'A' && haslo[x] <= 'Z') {
            literyDuze++;
        } else if (haslo[x] >= '0' && haslo[x] <= '9') {
            liczby++;
        } else {
            znakiSpecjalne++;
        }
        x++;
    }
    
    if (literyDuze > 0 && literyMale >= 8) {
        if (liczby >= 1) {
            if (znakiSpecjalne == 1) {
                return true;
            }
        }
    }
    return false;
}


bool sprawdzCzyIstniejeTakiUzytkownik(char login[]) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        if (strcmp(db[x].nazwaUzytkownika, login) == 0) {
            return true;
        }
    }
    return false;
}

bool sprawdzCzyIstniejeTakiUzytkownikF(char login[], char pass[]) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        char str[100];
        strcpy(str, db[x].haslo);
        odszyfruj1(str);
        if (strcmp(db[x].nazwaUzytkownika, login) == 0 && strcmp(str, pass) == 0) {
            return true;
        }
    }
    return false;
}

bool sprawdzNazweUzytkownika(char login[]) {
    int literyMale = 0;
    int literyDuze = 0;
    int liczby = 0;

    int x = 0;
    while (x < strlen(login)) {
        if (login[x] >= 'a' && login[x] <= 'z') {
            literyMale++;
        } else if (login[x] >= 'A' && login[x] <= 'Z') {
            literyDuze++;
        } else if (login[x] >= '0' && login[x] <= '9') {
            liczby++;
        } 
        x++;
    }

    if (strlen(login) >= 4) {
        if (literyDuze > 0 && literyMale >= 3) {
            if (liczby > 0) {
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool sprawdzKwote(char login[], char pass[], double kwota) {
    int x=0;
    char str[100];
    strcpy(str, db[x].haslo);
    odszyfruj1(str);
    while (strcmp(db[x].nazwaUzytkownika, login) != 0 && strcmp(str, pass) != 0) {
        x++;
        strcpy(str, db[x].haslo);
        odszyfruj1(str);
    }
    if ((db[x].stanKonta - kwota) >= 0) {
        return true;
    } else {
        return false;
    }
}

bool sprawdzDateUrodzenia(int dzien, int miesiac, int rok) {
    int actualYear = 2021; // hard coded 
    if (dzien > 0 && dzien <= 31) { 
        if (miesiac > 0 && miesiac <= 12) {
            if (rok > 999 && (actualYear - rok) >= 18) {
                return true;
            }
        }
    }
    return false;
}

// REJESTRACJA
void rejestracja() {
    struct KontoBankowe uzytkownik;
    printf("                                ---------------------------------------\n");
    printf("                                             REJESTRACJA\n");
    printf("                                ---------------------------------------\n");
    printf("\n");

    printf("                                        Nazwa uzytkownika:");
    scanf(" %s", uzytkownik.nazwaUzytkownika);
    while(!sprawdzNazweUzytkownika(uzytkownik.nazwaUzytkownika) || sprawdzCzyIstniejeTakiUzytkownik(uzytkownik.nazwaUzytkownika)) {
        if (sprawdzCzyIstniejeTakiUzytkownik(uzytkownik.nazwaUzytkownika)) {
            printf("\n");
            printf("                                 Uzytkownik o takiej nazwie juz istnieje\n");
            printf("                                        Nazwa uzytkownika:");
            scanf(" %s", uzytkownik.nazwaUzytkownika);
        } else {
            printf("\n");
            printf("                                 Login musi zawierac co najmniej 4 znaki\n");
            printf("                                 Litery duze, male i liczby\n");
            printf("                                        Nazwa uzytkownika:");
            scanf(" %s", uzytkownik.nazwaUzytkownika);
        }
    }

    printf("                                 Haslo musi spelniac wymagania\n");
    printf("                                 - co najmniej 1 duza litera\n");
    printf("                                 - co najmniej 8 malych liter\n");
    printf("                                 - 1 znak specjalny\n");
    printf("                                 - co najmniej 1 liczba\n");
    printf("                                        Haslo:");
    scanf(" %s", uzytkownik.haslo);
    while(!sprawdzHaslo(uzytkownik.haslo)) {
        printf("\n");
        printf("                                 Haslo musi spelniac wymagania\n");
        printf("                                 - 1 duza litera\n");
        printf("                                 - co najmniej 8 malych liter\n");
        printf("                                 - 1 znak specjalny\n");
        printf("                                 - co najmniej 1 liczba\n");
        printf("                                        Haslo:");
        scanf(" %s", uzytkownik.haslo);
    }

    szyfruj1(uzytkownik.haslo);
    strcpy(uzytkownik.haslo, uzytkownik.haslo);
    printf("                                        Imie:");
    scanf(" %s", uzytkownik.imie);
    printf("                                        Nazwisko:");
    scanf(" %s", uzytkownik.nazwisko);


    printf("                                        Data urodzenia(dd/mm/yyyy):");
    scanf("%d/%d/%d", &uzytkownik.dzien, &uzytkownik.miesiac, &uzytkownik.rok);
    while(!sprawdzDateUrodzenia(uzytkownik.dzien, uzytkownik.miesiac, uzytkownik.rok)) {
        printf("\n");
        printf("                                Data musi byc w formacie dd/mm/yyyy\n");
        printf("                                        Data urodzenia(dd/mm/yyyy):");
        scanf("%d/%d/%d", &uzytkownik.dzien, &uzytkownik.miesiac, &uzytkownik.rok);
    }
    printf("                                        Adres:");
    char c = getchar();
    gets(uzytkownik.adres);
    uzytkownik.stanKonta = 100;
    strcpy(uzytkownik.numerKonta, generujNumerKonta());
    strcpy(uzytkownik.numerKontaOszczednosciowego, generujNumerKontaOszczednosciowego());
    uzytkownik.stanKontaOszczednosciowego = 0;
    db[i] = uzytkownik;
    i++;
    
    runApp();
}

// MENU 2
void glowneMenu() {
    printf("\n");
    printf("\n");
    printf("                        # # # # # # # # # # # # # # # # # # # # # # # # # # # # \n");
    printf("                      #                                                         #\n");
    printf("                      #       * *      * *   * * * *   *     *   *     *        #\n");
    printf("                      #       *   *   *  *   *         *  *  *   *     *        #\n");
    printf("                      #       *     *    *   * * * *   *   * *   *     *        #\n");
    printf("                      #       *          *   *         *    **   *     *        #\n");
    printf("                      #       *          *   * * * *   *     *     * *          #\n");
    printf("                      #                                                         #\n");
    printf("                        # # # # # # # # # # # # # # # # # # # # # # # # # # # #\n");
    printf("                               #                                       #\n");
    printf("                               #                                       #\n");
    printf("                               #       1) Stan konta                   #\n");
    printf("                               #       2) Stan konta oszczednosciowego #\n");
    printf("                               #       3) Historia Przelewow           #\n");
    printf("                               #       4) Wykonaj przelew              #\n");
    printf("                               #       5) Przelej na oszczednosciowe   #\n");
    printf("                               #       6) Wyciagnij z oszczednosciowego#\n");
    printf("                               #       7) Moje konto                   #\n");
    printf("                               #       8) Wyloguj                      #\n");
    printf("                               #       9) Wyjdz                        #\n");
    printf("                               #                                       #\n");
    printf("                                # # # # # # # # # # # # # # # # # # # #\n");

}


double znajdzStanKonta(char login[], char pass[]) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        char str[100];
        strcpy(str, db[x].haslo);
        odszyfruj1(str);
        if (strcmp(db[x].nazwaUzytkownika, login) == 0 && strcmp(str, pass) == 0) {
            return db[x].stanKonta;
        }
    }
    return 0;
}


double znajdzStanKontaOszczednosciowego(char login[], char pass[]) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        char str[100];
        strcpy(str, db[x].haslo);
        odszyfruj1(str);
        if (strcmp(db[x].nazwaUzytkownika, login) == 0 && strcmp(str, pass) == 0) {
            return db[x].stanKontaOszczednosciowego;
        }
    }
    return 0;
}

// STAN KONTA
void stanKonta(char login[], char pass[]) {
    printf("                                ---------------------------------------\n");
    printf("                                               STAN KONTA\n");
    printf("                                ---------------------------------------\n");
    printf("                                                %.2lf zl\n",znajdzStanKonta(login, pass));
    scanf("%df", &opcjaGlowneMenu);
    kontroler(login, pass);
}

// STAN KONTA OSZCZEDNOSCIOWEGO
void stanKontaOszczednosciowego(char login[], char pass[]) {
    printf("                                ---------------------------------------\n");
    printf("                                      STAN KONTA OSZCZEDNOSCIOWEGO\n");
    printf("                                ---------------------------------------\n");
    printf("                                                %.2lf zl\n",znajdzStanKontaOszczednosciowego(login, pass));
    scanf("%d", &opcjaGlowneMenu);
    kontroler(login, pass);
}

// HISTORIA PRZELEWOW
void znajdzHistoriePrzelewow(char login[], char pass[]) {
    printf("                                ---------------------------------------\n");
    printf("                                           HISTORIA PRZELEWOW\n");
    printf("                                ---------------------------------------\n");

    char imie[20];
    char nazwisko[20];

    for (int k=0; k<sizeof(db)/sizeof(db[0]); k++) {
        char str[100];
        strcpy(str, db[k].haslo);
        odszyfruj1(str);
        if (strcmp(db[k].nazwaUzytkownika, login) == 0 && strcmp(str, pass) == 0) {
            strcpy(imie, db[k].imie);
            strcpy(nazwisko, db[k].nazwisko);
            break;
        }
    }

    for (int k=0; k<sizeof(historia)/sizeof(historia[0]); k++) {
         if ((strcmp(historia[k].imieW, imie) == 0 && strcmp(historia[k].nazwiskoW, nazwisko) == 0)) {
            printf("                             OD %s %s - %.2lf zl, %s", historia[k].imieO, historia[k].nazwiskoO, historia[k].kwota, historia[k].date);
        } else if ((strcmp(historia[k].imieO, imie) == 0 && strcmp(historia[k].nazwiskoO, nazwisko) == 0)) {
            printf("                             DO: %s %s - %.2lf zl, %s", historia[k].imieW, historia[k].nazwiskoW, historia[k].kwota, historia[k].date);
        }
    }

    scanf("%d", &opcjaGlowneMenu);
    kontroler(login, pass);
}

// MOJE KONTO
void informacjeOKoncie(char login[], char pass[]) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        char str[100];
        strcpy(str, db[x].haslo);
        odszyfruj1(str);
        if (strcmp(db[x].nazwaUzytkownika, login) == 0 && strcmp(str, pass) == 0) {
            printf("                                ---------------------------------------\n");
            printf("                                               MOJE KONTO\n");
            printf("                                ---------------------------------------\n");
            printf("                                Numer konta:\n                                %s\n", db[x].numerKonta);
            printf("\n");
            printf("                                Nr k.oszczednosciowego:\n                                %s\n", db[x].numerKontaOszczednosciowego);
        }
    }
    scanf("%d", &opcjaGlowneMenu);
    kontroler(login, pass);
}


bool sprawdzCzyIstniejeKonto(char numerKonta[]) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        if(strcmp(db[x].numerKonta, numerKonta) == 0) {
            return true;
        }
    }
    return false;
}

// PRZELEW
void wykonajPrzelew(char login[], char pass[]) {
    printf("                                ---------------------------------------\n");
    printf("                                                PRZELEW\n");
    printf("                                ---------------------------------------\n");
    struct Przelew przelew;
    printf("                                Kwota:");
    scanf("%lf", &przelew.kwota);

    if (!sprawdzKwote(login, pass, przelew.kwota)) {
        printf("\n");
        printf("                                Przekroczono ilosc pieniedzy dostepnych\n");
        printf("                                na twoim koncie.\n");
        scanf("%d", &opcjaGlowneMenu);
        kontroler(login, pass);
    }

    printf("\n");
    printf("                                Imie odbiorcy:");
    scanf("%s", przelew.imieOdbiorcy);
    printf("\n");
    printf("                                Nazwisko odbiorcy:");
    scanf("%s", przelew.nazwiskoOdbiorcy);
    printf("\n");

    printf("                                Numer Konta:");
    scanf("%s", przelew.numerKontaOdbiorcy);
    while (!sprawdzCzyIstniejeKonto(przelew.numerKontaOdbiorcy)) {
        printf("                                Nie znaleziono konta o takim numerze.\n");
        printf("                                Numer Konta:");
        scanf("%s", przelew.numerKontaOdbiorcy);
    }

    struct HistoriaPrzelewow rekord;
    for (int x = 0; x < sizeof(db) / sizeof(db[0]); x++) {
        if(strcmp(db[x].numerKonta, przelew.numerKontaOdbiorcy) == 0) {
            db[x].stanKonta += przelew.kwota;
            strcpy(rekord.imieW, db[x].imie);
            strcpy(rekord.nazwiskoW, db[x].nazwisko);
            rekord.kwota = przelew.kwota;
            strcpy(rekord.date, pobierzDate());
            break;
        }
    }
    for (int z = 0; z < sizeof(db) / sizeof(db[0]); z++) {
        char str[100];
        strcpy(str, db[z].haslo);
        odszyfruj1(str);
        if (strcmp(db[z].nazwaUzytkownika, login) == 0 && strcmp(str, pass) == 0) {
            db[z].stanKonta -= przelew.kwota;
            strcpy(rekord.imieO, db[z].imie);
            strcpy(rekord.nazwiskoO, db[z].nazwisko);
            printf("                                ---------------------------------------\n");
            printf("                                Zrealizowano przelew\n");
            printf("                                Twoj aktualny stan konta: %.2lf\n", db[z].stanKonta);

            historia[index_historii] = rekord;
            index_historii++;

            scanf("%d", &opcjaGlowneMenu);
            kontroler(login, pass);
            break;
        }
    }

}

// ZMIANA HASLA
void zmienHaslo() {
    char login[100];
    char haslo[100];
    char noweHaslo[100];
    printf("                                ---------------------------------------\n");
    printf("                                             Zmiana Hasla:\n");
    printf("                                ---------------------------------------\n");
    printf("                                             Login:");
    scanf(" %s", login);
    printf("                                             Stare Haslo:");
    scanf(" %s", haslo);
    printf("                                             Nowe Haslo:");
    scanf(" %s", noweHaslo);

    int z=0;
    char str[100];
    strcpy(str, db[z].haslo);
    odszyfruj1(str);
    while (strcmp(db[z].nazwaUzytkownika, login) != 0 && strcmp(str, haslo) != 0) {
        z++;
        strcpy(str, db[z].haslo);
        odszyfruj1(str);
    }

    if (sprawdzCzyIstniejeTakiUzytkownikF(login, haslo)) {
        if (sprawdzHaslo(noweHaslo)) {
            szyfruj1(noweHaslo);
            strcpy(db[z].haslo, noweHaslo);
            runApp();
        } else {
            printf("\n");
            printf("                                Zmiana hasla nie powiodla sie\n");
            printf("                                Zbyt slabe haslo\n");
            zmienHaslo();
        }
    } else {
        printf("\n");
        printf("                         Nie mozna zmienic hasla dla konta ktore nie istnieje");
        runApp();
    }
}

// DODAWANIE OSZCZEDNOSCI
void dodajOszczednosci(char login[], char pass[]) {
    printf("                                ---------------------------------------\n");
    printf("                                                PRZELEW:\n");
    printf("                                ---------------------------------------\n");
    double kwota;
    printf("                                              Kwota: ");
    scanf("%lf", &kwota);

    int z = 0;
    char str[100];
    strcpy(str, db[z].haslo);
    odszyfruj1(str);
    while (strcmp(db[z].nazwaUzytkownika, login) != 0 && strcmp(str, pass) != 0) {
        z++;
        strcpy(str, db[z].haslo);
        odszyfruj1(str);
    }

    if (sprawdzKwote(login, pass, kwota)) {
        db[z].stanKonta -= kwota;
        db[z].stanKontaOszczednosciowego += kwota;
        kontrolerGlowneMenu(login, pass);
    } else {
        printf("                             Kwota wykracza poza stan twojego glownego konta");
        kontrolerGlowneMenu(login, pass);
    }
}

bool sprawdzKwoteZkontaOszczednosciowego(char login[], char pass[], double kwota) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        char str[100];
        strcpy(str, db[x].haslo);
        odszyfruj1(str);
        if (strcmp(db[x].nazwaUzytkownika, login) == 0 && strcmp(str, pass) == 0) {
            if (db[x].stanKontaOszczednosciowego - kwota >= 0) {
                return true;
            }
        }
    }
    return false;
}

// WYCIAGANIE OSZCZEDNOSCI
void wyciagnijOszczednosci(char login[], char pass[]) {
    printf("                                ---------------------------------------\n");
    printf("                                                PRZELEW:\n");
    printf("                                ---------------------------------------\n");
    double kwota;
    printf("                                              Kwota: ");
    scanf("%lf", &kwota);

    int z = 0;
    char str[100];
    strcpy(str, db[z].haslo);
    odszyfruj1(str);
    while (strcmp(db[z].nazwaUzytkownika, login) != 0 && strcmp(str, pass) != 0) {
        z++;
        strcpy(str, db[z].haslo);
        odszyfruj1(str);
    }

    if (sprawdzKwoteZkontaOszczednosciowego(login, pass, kwota)) {
        db[z].stanKonta += kwota;
        db[z].stanKontaOszczednosciowego -= kwota;
        kontrolerGlowneMenu(login, pass);
    } else {
        printf("                             Kwota wykracza poza stan twojego konta oszczednosciowego");
        kontrolerGlowneMenu(login, pass);
    }
}

void kontroler(char login[], char pass[]) {
    switch (opcjaGlowneMenu) {
        case 1:
            stanKonta(login, pass);
            break;
        case 2:
            stanKontaOszczednosciowego(login, pass);
            break;
        case 3:
            znajdzHistoriePrzelewow(login, pass);
            break;
        case 4:
            wykonajPrzelew(login, pass);
            break;
        case 5:
            dodajOszczednosci(login, pass);
            break;
        case 6:
            wyciagnijOszczednosci(login, pass);
            break;
        case 7:
            informacjeOKoncie(login, pass);
            break;
        case 8:
            runApp();
            break;
        case 9:
            printf("                                         --------------------\n");
            printf("                                         Zapraszamy ponownie!\n");
            break;
    }
}


void kontrolerGlowneMenu(char login[], char pass[]) {
    glowneMenu();
    scanf("%d", &opcjaGlowneMenu);
    switch (opcjaGlowneMenu) {
        case 1:
            stanKonta(login, pass);
            break;
        case 2:
            stanKontaOszczednosciowego(login, pass);
            break;
        case 3:
            znajdzHistoriePrzelewow(login, pass);
            break;
        case 4:
            wykonajPrzelew(login, pass);
            break;
        case 5:
            dodajOszczednosci(login, pass);
            break;
        case 6:
            wyciagnijOszczednosci(login, pass);
            break;
        case 7:
            informacjeOKoncie(login, pass);
            break;
        case 8:
            runApp();
            break;
        case 9:
            printf("                                         --------------------\n");
            printf("                                         Zapraszamy ponownie!\n");
            break;
    }
}

// LOGOWANIE
void logowanie() {
    char login[20];
    char pass[20];
    printf("                                ---------------------------------------\n");
    printf("                                               LOGOWANIE\n");
    printf("                                ---------------------------------------\n");
    printf("                                               Login:");
    scanf("%s", login);
    printf("                                               Haslo:");
    scanf("%s", pass);

    if (sprawdzUzytkownika(login, pass)) {
        kontrolerGlowneMenu(login, pass);
    } else {
        printf("                                Zly login lub haslo, sprobuj ponownie.\n");
        runApp();
    }
}


bool sprawdzUzytkownika(char log[], char pass[]) {
    for (int x=0; x<sizeof(db)/sizeof(db[0]); x++) {
        char str[100];
        strcpy(str, db[x].haslo);
        odszyfruj1(str);
        if (strcmp(db[x].nazwaUzytkownika, log) == 0 && strcmp(str, pass) == 0) {
            return true;
        }
    }
    return false;
}


void runApp() {
    menu();
    
    int opcjaMenu = 0;
    scanf("%d", &opcjaMenu);
    
    switch (opcjaMenu) {
        case 1:
            logowanie();
            break;
        case 2:
            rejestracja();
            break;
        case 3:
            zmienHaslo();
            break;
        case 4:
            printf("                                         --------------------\n");
            printf("                                         Zapraszamy ponownie!\n");
            break;
    }
}


int main() { 
    inicjalizujBazeDanych();
    runApp();
    return 0;
}






