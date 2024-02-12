#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    COMMAND_HELP = 1,
    COMMAND_INSERT,
    COMMAND_DELETE,
    COMMAND_SELECT,
    COMMAND_UPDATE,
    COMMAND_TRANSFER,
    COMMAND_COUNT,
    COMMAND_EXIT,
    COMMAND_UNKNOWN
}Menu;

typedef struct{
    int id;
    char nickname[16], data_creare_cont[11];
    int balanta_coins, level, exp_points;
}Player;

void ToLowerCase(char* string)
{
    // Functie pentru a converti un string la litere mici
    for (int caracter = 0; caracter < strlen(string); ++caracter)
    {
        if (string[caracter] >= 'A' && string[caracter] <= 'Z')
            string[caracter] += 32;
    }
    return;
}

Menu GetCommand(char* comanda)
{
    ToLowerCase(comanda); // Vrem ca o comanda sa poata fi apelata indiferent de modul in care sunt scrise literele (mari/mici)
    if (strcmp(comanda, "help") == 0)
    {
        return COMMAND_HELP;
    }
    else if (strcmp(comanda, "insert") == 0)
    {
        return COMMAND_INSERT;
    }
    else if (strcmp(comanda, "delete") == 0)
    {
        return COMMAND_DELETE;
    }
    else if (strcmp(comanda, "select") == 0)
    {
        return COMMAND_SELECT;
    }
    else if (strcmp(comanda, "update") == 0)
    {
        return COMMAND_UPDATE;
    }
    else if (strcmp(comanda, "transfer") == 0)
    {
        return COMMAND_TRANSFER;
    }
    else if (strcmp(comanda, "count") == 0)
    {
        return COMMAND_COUNT;
    }
    else if (strcmp(comanda, "exit") == 0)
    {
        return COMMAND_EXIT;
    }
    return COMMAND_UNKNOWN; // Comanda nu este recunoscuta
}

void Actualizare_Fisier(char* nume_fisier, Player* playeri, int nr_playeri)
{
    // Functie pentru a actualiza un fisier dupa ce se realizeaza operatii asupra bazei de date 
    FILE* fisier = fopen(nume_fisier, "w");
    fprintf(fisier, "id nickname data_creare_cont balanta_coins level exp_points"); // Legenda bazei de date
    for (int i = 0; i < nr_playeri; ++i)
    {
        fprintf(fisier, "\n");
        fprintf(fisier, "%d %s %s %d %d %d", playeri[i].id, playeri[i].nickname, playeri[i].data_creare_cont, playeri[i].balanta_coins, playeri[i].level, playeri[i].exp_points);
    }
    fclose(fisier);
}

void verificaEmptyDB(char* nume_fisier, Player* playeri, int* nr_playeri, char* optiune)
{
    if ((*nr_playeri) != 0)
        return;
    printf("\nSe pare ca baza de date este goala! Vrei sa se insereze doua intrari default? (Y/N): ");
    scanf("%c", optiune);
    if(*optiune == '\n')
        scanf("%c", optiune);
    while(*optiune != 'Y' && *optiune != 'N')
    {
        printf("Sintaxa gresita! Te rog sa introduci una din cele doua litere (Y/N): ");
        scanf("\n");
        scanf("%c", optiune);
    }
    if ((*optiune) == 'Y')
    {
        (*nr_playeri) += 2;
        playeri[0].id = 0;
        strcpy(playeri[0].nickname, "Andrei");
        strcpy(playeri[0].data_creare_cont, "21/11/2022");
        playeri[0].balanta_coins = 10;
        playeri[0].level = 30;
        playeri[0].exp_points = 29;
        playeri[1].id = 1;
        strcpy(playeri[1].nickname, "Marius");
        strcpy(playeri[1].data_creare_cont, "20/05/2021");
        playeri[1].balanta_coins = 50;
        playeri[1].level = 70;
        playeri[1].exp_points = 139;
        Actualizare_Fisier(nume_fisier, playeri, *nr_playeri);
    }
}

void Afisare_Conturi(Player* playeri, int nr_playeri)
{
    // Functie pentru a afisa toate conturile
    if (nr_playeri == 0)
    {
        printf("Se pare ca baza de date este goala!");
        return;
    }
    for (int i = 0; i < nr_playeri; ++i)
        printf("\n----------\nID: %d\nNickname: %s\nData crearii contului (ZZ/LL/AAAA): %s\nBalanta coins: %d\nLevel: %d\nExperience points: %d", playeri[i].id, playeri[i].nickname, playeri[i].data_creare_cont, playeri[i].balanta_coins, playeri[i].level, playeri[i].exp_points);
    printf("\n----------");
}

void Citire_Fisier(char* nume_fisier, Player** playeri, int* nr_playeri)
{
    // Functie pentru a citi baza de date din fisier
    FILE* fisier = fopen(nume_fisier, "r");
    if (fisier == NULL)
    {
        perror("Eroare la deschidere fisier");
        return;
    }

    int skip_prima_linie, cnt_caractere_sarite = 0;
    
    // Prima linie din fisier este folosita drept legenda tabel, deci trebuie sarita.
    
    skip_prima_linie = fgetc(fisier);
    while (skip_prima_linie != '\n' && !feof(fisier))
    {
        cnt_caractere_sarite++;
        skip_prima_linie = fgetc(fisier);
    }

    // Am ajuns la inceputul celei de-a doua linii.
    while (!feof(fisier))
    {
        int dimensiune = *nr_playeri + 1;
        Player *temp = (Player*)realloc(*playeri, dimensiune * sizeof(Player));
        if (temp == NULL)
        {
            printf("Nu s-a putut aloca memorie. \n");
            exit(1);
        }
        else
        {
            //fscanf(fisier, "%d%s%s%d%d%d", &playeri[*nr_playeri]->id, playeri[*nr_playeri]->nickname, playeri[*nr_playeri]->data_creare_cont, &playeri[*nr_playeri]->balanta_coins, &playeri[*nr_playeri]->level, &playeri[*nr_playeri]->exp_points);
            fscanf(fisier, "%d%s%s%d%d%d", &temp[*nr_playeri].id, temp[*nr_playeri].nickname, temp[*nr_playeri].data_creare_cont, &temp[*nr_playeri].balanta_coins, &temp[*nr_playeri].level, &temp[*nr_playeri].exp_points);
            fscanf(fisier, "\n");
            (*nr_playeri)++;
            *playeri = temp;
        }
    }
    fclose(fisier);
}

int Get_ID(Player* playeri, int nr_playeri)
{
    // Functie pentru a gasi ID-ul potrivit cand este apelata comanda de inserare
    // i.e. daca in baza de date apar ID-urile 0, 1, 2, 3 si este sters contul cu ID 1, vrem ca acel ID sa poata fi folosit din nou
    int fr[100];
    for (int i = 0; i < nr_playeri; ++i)
        fr[playeri[i].id] = -1;
    for (int i = 0; i < 100; ++i)
        if (fr[i] != -1)
            return i;
}

int idToPosition(int id_cautat, int nr_playeri, Player* playeri)
{
    // Functie pentru a identifica pozitia din vectorul "playeri" asociata ID-ului cautat, daca acesta exista
    // In caz contrar, se returneaza o valoare potrivita
    for (int i = 0; i < nr_playeri; ++i)
        if (playeri[i].id == id_cautat)
            return i;
    return -1;
}

int nicknameToPosition(char* nickname_cautat, int nr_playeri, Player* playeri)
{
    // Functie pentru a identifica pozitia din vectorul "playeri" asociata nickname-ului cautat, daca acesta exista
    // In caz contrar, se returneaza o valoare potrivita
    for (int i = 0; i < nr_playeri; ++i)
        if (strcmp(playeri[i].nickname, nickname_cautat) == 0)
            return i;
    return -1;
}

void Command_Help()
{
    // Implementarea comenzii help
    printf("help - lista comenzi\n"
    "insert - adauga un cont\n"
    "select - afiseaza conturile dupa id/nickname\n"
    "   ex. sintaxa: select * - afiseaza toate conturile\n"
    "update - modifica un cont dupa id/nickname\n"
    "transfer - realizeaza transfer de balanta intre doua conturi\n"
    "count - numara cate conturi indeplinesc o conditie pentru Balanta coins/ Level/ Experience\n"
    "   ex. sintaxa: balanta_coins > 1000; level < 10; exp_points = 5000\n"
    "delete - sterge un cont \n");
}

void Command_Insert(char* nume_fisier, Player* playeri[], int *nr_playeri)
{
    // Implementarea comenzii insert
    FILE* fisier = fopen(nume_fisier, "a");
    int nr_playeri_noi = 0;
    char litera, lit[1], nickname_aux[15];
    lit[1] = 0;
    int cnt = 0;

    printf("Introdu numarul de conturi pe care vrei sa le inserezi: ");
    scanf("%d", &nr_playeri_noi);
    
    int i = *nr_playeri;
    int nr_playeri_final = (*nr_playeri) + nr_playeri_noi;

    Player *temp = (Player*)realloc(*playeri, nr_playeri_final * sizeof(Player));
    if (temp == NULL)
    {
        printf("Nu s-a putut aloca memorie.\n");
        fclose(fisier);
        exit(1);
    }
    else {
        for (i; i < nr_playeri_final; ++i)
        {
            printf("\n");
            temp[i].id = Get_ID(*playeri, *nr_playeri);
            printf("ID: %d\n", temp[i].id);

            printf("Nickname: ");
            scanf("\n");
            fgets(temp[i].nickname, 16, stdin);
            if (temp[i].nickname[strlen(temp[i].nickname) - 1] == '\n')
                temp[i].nickname[strlen(temp[i].nickname) - 1] = 0;

            printf("Data crearii contului (ZZ/LL/AAAA): ");
            fgets(temp[i].data_creare_cont, 11, stdin);
            if(temp[i].data_creare_cont[strlen(temp[i].data_creare_cont) - 1] == '\n')
                temp[i].data_creare_cont[strlen(temp[i].data_creare_cont) - 1] = 0;

            printf("Balanta coins: ");
            scanf("%d", &temp[i].balanta_coins);

            printf("Level: ");
            scanf("%d", &temp[i].level);
            
            printf("Experience points: ");
            scanf("%d", &temp[i].exp_points);

            fprintf(fisier, "\n");
            fprintf(fisier, "%d %s %s %d %d %d", temp[i].id, temp[i].nickname, temp[i].data_creare_cont, temp[i].balanta_coins, temp[i].level, temp[i].exp_points);
            (*nr_playeri)++;
        }
        *playeri = temp;
        fclose(fisier);
    }
}

void Command_Delete(char* nume_fisier, Player** playeri, int *nr_playeri, char* default_insert)
{
    // Implementarea comenzii delete
    printf("Cum vrei sa gasesti contul pe care vrei sa-l stergi? (ID/Nickname): ");
    char criteriu[10];
    scanf("\n");
    scanf("%s", criteriu);
    ToLowerCase(criteriu);
    int poz_de_sters;
    if (strcmp(criteriu, "id") != 0 && strcmp(criteriu, "nickname") != 0)
    {
        printf("Sintaxa gresita! Consulta \"help\".");
        return;
    }
    else if (strcmp(criteriu, "id") == 0)
    {
        printf("Introdu ID-ul asociat contului de sters: ");
        int id_de_sters;
        scanf("%d", &id_de_sters);
        poz_de_sters = idToPosition(id_de_sters, *nr_playeri, *playeri);
        if (poz_de_sters == -1)
        {
            printf("ID-ul asociat nu exista!");
            return;
        }
    }
    else if (strcmp(criteriu, "nickname") == 0)
    {
        printf("Introdu nickname-ul asociat contului de sters: ");
        char nickname_cautat[16];
        scanf("%s", nickname_cautat);
        poz_de_sters = nicknameToPosition(nickname_cautat, *nr_playeri, *playeri);
        if (poz_de_sters == -1)
        {
            printf("Nickname-ul asociat nu exista!");
            return;
        }
    }
    Player *temp = *playeri;
    for (int i = poz_de_sters; i < (*nr_playeri) - 1; ++i)
        temp[i] = temp[i + 1];
    (*nr_playeri) -= 1;
    int dimensiune = *nr_playeri;
    temp = (Player*)realloc(*playeri, dimensiune * sizeof(Player));
    printf("Stergere cont...\nStergerea a fost efectuata cu succes!");
    Actualizare_Fisier(nume_fisier, *playeri, *nr_playeri);
    *default_insert = '?';
}

void Command_Select(Player* playeri, int *nr_playeri)
{
    // Implementarea comenzii select
    printf("Ce criteriu vrei sa folosesti in selectie? (ID/Nickname/*): ");
    char criteriu[10];
    scanf("\n");
    scanf("%s", criteriu);
    ToLowerCase(criteriu);
    int poz_de_afisat;
    if (strcmp(criteriu, "*") != 0 && strcmp(criteriu, "id") != 0 && strcmp(criteriu, "nickname") != 0)
    {
        printf("Sintaxa gresita! Consulta \"help\".");
        return;
    }
    else if (strcmp(criteriu, "*") == 0)
    {
        Afisare_Conturi(playeri, *nr_playeri);
        return;
    }
    else if (strcmp(criteriu, "id") == 0)
    {
        printf("Introdu ID cautat: ");
        int id_cautat;
        scanf("%d", &id_cautat);
        poz_de_afisat = idToPosition(id_cautat, *nr_playeri, playeri);
        if (poz_de_afisat == -1)
        {
            printf("ID-ul cautat nu exista!");
            return;
        }        
    }
    else if (strcmp(criteriu, "nickname") == 0)
    {
        printf("Introdu nickname cautat: ");
        char nickname_cautat[16];
        scanf("%s", nickname_cautat);
        poz_de_afisat = nicknameToPosition(nickname_cautat, *nr_playeri, playeri);
        if (poz_de_afisat == -1)
        {
            printf("Nickname-ul cautat nu exista!");
            return;
        }
    }
    printf("\n----------\nID: %d\nNickname: %s\nData crearii contului (ZZ/LL/AAAA): %s\nBalanta coins: %d\nLevel: %d\nExperience points: %d", playeri[poz_de_afisat].id, playeri[poz_de_afisat].nickname, playeri[poz_de_afisat].data_creare_cont, playeri[poz_de_afisat].balanta_coins, playeri[poz_de_afisat].level, playeri[poz_de_afisat].exp_points);
    printf("\n----------");
}

void Command_Update(char* nume_fisier, Player* playeri, int *nr_playeri)
{
    // Implementarea comenzii update
    printf("Cum vrei sa gasesti contul pe care vrei sa-l actualizezi? (ID/Nickname): ");
    char criteriu[10];
    scanf("\n");
    scanf("%s", criteriu);
    ToLowerCase(criteriu);
    int poz_de_actualizat;
    if (strcmp(criteriu, "id") != 0 && strcmp(criteriu, "nickname") != 0)
    {
        printf("Sintaxa gresita! Consulta \"help\".");
        return;
    }
    else if (strcmp(criteriu, "id") == 0)
    {
        printf("Introdu ID-ul asociat contului de actualizat: ");
        int id_de_actualizat;
        scanf("%d", &id_de_actualizat);
        poz_de_actualizat = idToPosition(id_de_actualizat, *nr_playeri, playeri);
        if (poz_de_actualizat == -1)
        {
            printf("ID-ul asociat nu exista!");
            return;
        }
    }
    else if (strcmp(criteriu, "nickname") == 0)
    {
        printf("Introdu nickname-ul asociat contului de actualizat: ");
        char nickname_cautat[16];
        scanf("%s", nickname_cautat);
        poz_de_actualizat = nicknameToPosition(nickname_cautat, *nr_playeri, playeri);
        if (poz_de_actualizat == -1)
        {
            printf("Nickname-ul asociat nu exista!");
            return;
        }
    }
    printf("ID: %d\n", playeri[poz_de_actualizat].id);
    printf("Nickname: ");
    scanf("\n");
    fgets(playeri[poz_de_actualizat].nickname, 16, stdin);
    if (playeri[poz_de_actualizat].nickname[strlen(playeri[poz_de_actualizat].nickname) - 1] == '\n')
        playeri[poz_de_actualizat].nickname[strlen(playeri[poz_de_actualizat].nickname) - 1] = 0;

    printf("Data crearii contului (ZZ/LL/AAAA): ");
    fgets(playeri[poz_de_actualizat].data_creare_cont, 11, stdin);
    if (playeri[poz_de_actualizat].data_creare_cont[strlen(playeri[poz_de_actualizat].data_creare_cont) - 1] == '\n')
        playeri[poz_de_actualizat].data_creare_cont[strlen(playeri[poz_de_actualizat].data_creare_cont) - 1] = 0;

    printf("Balanta coins: ");
    scanf("%d", &playeri[poz_de_actualizat].balanta_coins);

    printf("Level: ");
    scanf("%d", &playeri[poz_de_actualizat].level);
        
    printf("Experience points: ");
    scanf("%d", &playeri[poz_de_actualizat].exp_points);
    Actualizare_Fisier(nume_fisier, playeri, *nr_playeri);
}

void Command_Transfer(char* nume_fisier, Player* playeri, int *nr_playeri)
{
    // Implementarea comenzii transfer
    int poz_sender, poz_receiver, sender_id, receiver_id;
    printf("Cum vrei sa gasesti contul din care vrei sa transferi coins? (ID/Nickname): ");
    char criteriu[10];
    scanf("\n");
    scanf("%s", criteriu);
    ToLowerCase(criteriu);
    if (strcmp(criteriu, "id") != 0 && strcmp(criteriu, "nickname") != 0)
    {
        printf("Sintaxa gresita! Consulta \"help\".");
        return;
    }
    else if (strcmp(criteriu, "id") == 0)
    {
        printf("Introdu ID-ul asociat contului din care vrei sa transferi coins: ");
        scanf("%d", &sender_id);
        poz_sender = idToPosition(sender_id, *nr_playeri, playeri);
        if (poz_sender == -1)
        {
            printf("ID-ul asociat nu exista!");
            return;
        }
    }
    else if (strcmp(criteriu, "nickname") == 0)
    {
        printf("Introdu nickname-ul asociat contului din care vrei sa transferi coins: ");
        char nickname_cautat[16];
        scanf("%s", nickname_cautat);
        poz_sender = nicknameToPosition(nickname_cautat, *nr_playeri, playeri);
        if (poz_sender == -1)
        {
            printf("Nickname-ul asociat nu exista!");
            return;
        }
    }
    printf("Cum vrei sa gasesti contul care vrei sa primeasca coins? (ID/Nickname): ");
    scanf("\n");
    scanf("%s", criteriu);
    ToLowerCase(criteriu);
    if (strcmp(criteriu, "id") != 0 && strcmp(criteriu, "nickname") != 0)
    {
        printf("Sintaxa gresita! Consulta \"help\".");
        return;
    }
    else if (strcmp(criteriu, "id") == 0)
    {
        printf("Introdu ID-ul asociat contului care vrei sa primeasca coins: ");
        scanf("%d", &receiver_id);
        poz_receiver = idToPosition(receiver_id, *nr_playeri, playeri);
        if (poz_receiver == -1)
        {
            printf("ID-ul asociat nu exista!");
            return;
        }
    }
    else if (strcmp(criteriu, "nickname") == 0)
    {
        printf("Introdu nickname-ul asociat contului care vrei sa primeasca coins: ");
        char nickname_cautat[16];
        scanf("%s", nickname_cautat);
        poz_receiver = nicknameToPosition(nickname_cautat, *nr_playeri, playeri);
        if (poz_receiver == -1)
        {
            printf("Nickname-ul asociat nu exista!");
            return;
        }
    }
    int cantitate_coins;
    printf("Introdu cantitatea de coins pe care vrei sa o transferi intre cele doua conturi: ");
    scanf("%d", &cantitate_coins);
    while (playeri[poz_sender].balanta_coins < cantitate_coins)
    {
        printf("Contul din care vrei sa transferi coins nu detine atat!\n");
        printf("Introdu o alta cantitate de coins: ");
        scanf("%d", &cantitate_coins);
    }
    playeri[poz_sender].balanta_coins -= cantitate_coins;
    playeri[poz_receiver].balanta_coins += cantitate_coins;
    printf("Transfer realizat cu succes!");
    Actualizare_Fisier(nume_fisier, playeri, *nr_playeri);
}

void Command_Count(Player* playeri, int* nr_playeri)
{
    // Implementarea comenzii count
    printf("Introdu conditia necesara: ");
    char element[30], operator;
    int valoare, cnt = -1;
    scanf("%s %c %d", element, &operator, &valoare);
    ToLowerCase(element);
    //printf("element = %s\noperator = %c\nvaloare = %d", element, operator, valoare);
    if (strcmp(element, "balanta_coins") != 0 && strcmp(element, "level") != 0 && strcmp(element, "exp_points"))
    {
        printf("Sintaxa gresita! Consulta \"help\".");
        // Daca sunt introduse 2 cuvinte despartite prin spatiu, dupa ce se afiseaza eroarea al doilea cuvant va fi folosit drept noua comanda
        // Evitam acest lucru prin stocarea celui de-al doilea cuvant intr-un string
        char clear[30];
        scanf("%s", clear);
        return;
    }
    else if (!strchr(">=<", operator))
    {
        printf("Sintaxa gresita! Consulta \"help\".");
        return;
    }
    else
    {
        cnt = 0;
        if (strcmp(element, "balanta_coins") == 0)
        {
            switch (operator)
            {
                case '>':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].balanta_coins > valoare)
                            cnt++;
                    break;
                case '=':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].balanta_coins == valoare)
                            cnt++;
                    break;
                case '<':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].balanta_coins < valoare)
                            cnt++;
                    break;
            }
        }
        else if (strcmp(element, "level") == 0)
        {
            switch (operator)
            {
                case '>':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].level > valoare)
                            cnt++;
                    break;
                case '=':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].level == valoare)
                            cnt++;
                    break;
                case '<':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].level < valoare)
                            cnt++;
                    break;
            }
        }
        else if (strcmp(element, "exp_points") == 0)
        {
            switch (operator)
            {
                case '>':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].exp_points > valoare)
                            cnt++;
                    break;
                case '=':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].exp_points == valoare)
                            cnt++;
                    break;
                case '<':
                    for (int i = 0; i < *nr_playeri; ++i)
                        if (playeri[i].exp_points < valoare)
                            cnt++;
                    break;
            }
        }
    }
    if (cnt == 0)
        printf("Nu a fost gasit niciun cont care sa indeplineasca cerinta!");
    else if (cnt == 1)
        printf("A fost gasit un cont care sa indeplineasca cerinta!");
    else if (cnt > 1)
        printf("Au fost gasite %d conturi care sa indeplineasca cerinta!", cnt);
}

int main()
{
    char comanda[30];
    int running = 1, optiune = 0, nr_playeri = 0;
    char default_insert = '?';
    //Player playeri[100];
    Player* playeri = (Player*)calloc(1, sizeof(Player));
    //playeri = (Player*)realloc(playeri, 2 * sizeof(Player));
    Citire_Fisier("database.txt", &playeri, &nr_playeri);
    // printf("nr playeri = %d\n", nr_playeri);
    printf("\nComenzi disponibile: help, insert, delete, select, update, transfer, count, exit");
    while (running)
    {
        if (default_insert == '?')
            verificaEmptyDB("database.txt", playeri, &nr_playeri, &default_insert);
        printf("\n\nIntrodu comanda: ");
        scanf("%s", comanda);
        optiune = GetCommand(comanda);
        switch(optiune)
        {
            case COMMAND_HELP:
                Command_Help();
                break;
            case COMMAND_INSERT:
                Command_Insert("database.txt", &playeri, &nr_playeri);
                break;
            case COMMAND_DELETE:
                Command_Delete("database.txt", &playeri, &nr_playeri, &default_insert);
                break;
            case COMMAND_SELECT:
                Command_Select(playeri, &nr_playeri);
                break;
            case COMMAND_UPDATE:
                Command_Update("database.txt", playeri, &nr_playeri);
                break;
            case COMMAND_TRANSFER:
                Command_Transfer("database.txt", playeri, &nr_playeri);
                break;
            case COMMAND_COUNT:
                Command_Count(playeri, &nr_playeri);
                break;
            case COMMAND_EXIT:
                printf("Iesire din program...\n");
                running = 0;
                free(playeri);
                break;
            case COMMAND_UNKNOWN:
                printf("Introdu o comanda corecta! Tasteaza \"help\" pentru detalii.\n");
                break;
        }
    }
    return 0;
}