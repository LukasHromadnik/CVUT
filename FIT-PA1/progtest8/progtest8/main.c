/*
 * ⁄kolem je vytvo¯it sadu funkcÌ (ne cel˝ program, sadu funkcÌ), kter· bude simulovat se¯aÔovacÌ n·draûÌ.
 *
 * P¯edpokl·d·me, ûe naöe se¯aÔovacÌ n·draûÌ je umÌstÏnÈ na hlavnÌm ûelezniËnÌm koridoru, kde jezdÌ vlaky mezi velk˝mi dopravnÌmi uzly.
 * D·le na se¯aÔovacÌ n·draûÌ jezdÌ mÌstnÌ lok·lka. Vlaky mohou vÈzt libovolnÈ mnoûstvÌ vagÛn˘, pro kaûd˝ vagÛn zn·me jeho cÌlovÈ se¯aÔovacÌ n·draûÌ.
 * Na se¯aÔovacÌm n·draûÌ se potk· vlak jedoucÌ po hlavnÌm koridoru a mÌstnÌ lok·lka. Kaûd˝ z vlak˘ m· nÏjakÈ vagÛny (p¯ÌpadnÏ û·dnÈ vagÛny).
 * Na se¯aÔovacÌm n·draûÌ dojde k v˝mÏnÏ vagÛn˘:
 *
 *  ï vagÛny, kterÈ p¯ivezla lok·lka, budou zapojeny za vlak jedoucÌ po koridoru (na jeho konec),
 *  ï vagÛny, kterÈ p¯ivezl vlak po koridoru a kterÈ zde konËÌ, budou zapojeny za lok·lku,
 *  ï p¯i p¯epojov·nÌ je zachov·v·no po¯adÌ (vagÛny se neprohazujÌ).
 *
 */
#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DETAILED_PRINTs

/* TVAN
 je struktura, kter· reprezentuje jeden n·kladnÌ vagÛn. M· sloûky m_Next, kter· odkazuje na p¯Ìpadn˝ dalöÌ vagÛn v ¯adÏ (musÌ b˝t NULL pro poslednÌ
 vagÛn), m_To, kter· ud·v· jmÈno cÌlovÈho se¯aÔovacÌho n·draûÌ a m_Cargo, kter· ud·v· n·klad vagÛnu (do n·kladu jako p¯epravce nezasahujete,
 jeho obsah V·s nemusÌ zajÌmat).
 */
typedef struct TVan
{
    struct TVan   * m_Next;
    char          * m_To;
    void          * m_Cargo;
} TVAN;

/*
 TTRAIN
 je struktura popisujÌcÌ vlak (lok·lku i vlak jedoucÌ po koridoru). M· sloûky m_Start, kter· je odkazem na prvnÌ vagÛn (NULL, pokud jede samotn·
 lokomotiva bez vagÛn˘) a m_Count ud·vajÌcÌ poËet vagÛn˘ v tomto vlaku (musÌ korespondovat s dÈlkou spojovÈho seznamu vagÛn˘).
 */
typedef struct TTrain
{
    TVAN          * m_Start;
    int             m_Count;
} TTRAIN;
#endif /* __PROGTEST__ */

/**
 borrowVan
 je funkce pro zap˘jËenÌ vagÛnu. V naöÌ implementaci funkce vytvo¯Ì novou strukturu TVAN a vyplnÌ jejÌ sloûky. Sloûky m_Next a m_Cargo
 nastavÌ na NULL a sloûku m_To vyplnÌ podle parametru.
 */

TVAN * borrowVan ( const char * to )
{
    TVAN *b = (TVAN *) malloc(sizeof(TVAN));
    if(b == NULL) return NULL;
    
    b->m_Next = NULL;
    b->m_Cargo = NULL;
    b->m_To = strdup(to);
    
    return b;
}

/**
 returnVans
 je funkce, kter· slouûÌ k vr·cenÌ zap˘jËen˝ch vagÛn˘. V naöÌ reprezentaci funkce uvolnÌ pamÏù alokovanou pro reprezentaci vagÛn˘.
 (D·le upravÌ sloûky ve struktu¯e vlaku (nastavÌ NULL/0). Sloûkou m_Cargo se zab˝vat nemusÌte, bude nastavena na NULL,
 z·kaznÌci si n·klad p¯ed odevzd·nÌm vagÛnu vyloûÌ).
 */
void   returnVans ( TTRAIN * train )
{
    TVAN *drop;// = (TVAN *)malloc(sizeof(TVAN));
    TVAN *van;// = (TVAN *)malloc(sizeof(TVAN));
    van = train->m_Start;
    if(train->m_Start == NULL) return;
    while(van->m_Next != NULL)
    {
        
        drop = van;
        van = van->m_Next;
        free(drop);
    }
    free(van);
    train->m_Start = NULL;
    train->m_Count = 0;
}

/**
 connectVan
 je funkce, kter· zapojÌ vagÛn do vlaku. Funkce m· 3 parametry: vlak, kam m· b˝t vagÛn zapojen, vagÛn, kter˝ m· b˝t zapojen a p¯Ìznak,
 zda m· b˝t vagÛn zapojen na zaË·tek vlaku (parametr toStart nenulov˝) nebo na konec vlaku (parametr toStart nulov˝).
 Funkce odpovÌdajÌcÌm zp˘sobem upravÌ spojov˝ seznam a reprezentaci vlaku.
 */
void   connectVan ( TTRAIN * train, TVAN * van, int toStart )
{
#ifdef DETAILED_PRINT
    printf("FUNC: %7s ", "connect");
#endif
    if(toStart == 0)
    {
        if(train->m_Start == NULL)
        {
            train->m_Start = van;
            train->m_Count++;
            return;
        }
        van->m_Next = NULL;
#ifdef DETAILED_PRINT
        printf("[%s]\n", van->m_To);
#endif
        TVAN *a = (TVAN *)malloc(sizeof(TVAN));
        a = train->m_Start;
        while(a->m_Next != NULL)
        {
            a = a->m_Next;
        }
        a->m_Next = van;
        a = a->m_Next;
        train->m_Count++;
    }
    else
    {
#ifdef DETAILED_PRINT
        printf("to start [%s]\n", van->m_To);
#endif
        van->m_Next = train->m_Start;
        train->m_Start = van;
        train->m_Count++;
    }
    van = train->m_Start;
    printf("      train with %d vans: ", train->m_Count);
    while(van->m_Next != NULL)
    {
        printf("[%s]->",van->m_To);
        van = van->m_Next;
    }
    printf("[%s]\n",van->m_To);
}

/**
 route
 je funkce, kter· simuluje vlastnÌ se¯aÔovacÌ n·draûÌ. Parametrem je jmÈno se¯aÔovacÌho n·draûÌ (podle toho vybÌr· vagÛny z vlaku), vlak train,
 kter˝ p¯ijel po koridoru a mÌstnÌ lok·lka localTrain. Funkce zajistÌ spr·vnÈ p¯epojenÌ vagÛn˘ mezi vlaky, tj. odpovÌdajÌcÌ ˙pravy struktur,
 kterÈ vlaky a vagÛny reprezentujÌ.
 
 ï vagÛny, kterÈ p¯ivezla lok·lka, budou zapojeny za vlak jedoucÌ po koridoru (na jeho konec),
 ï vagÛny, kterÈ p¯ivezl vlak po koridoru a kterÈ zde konËÌ, budou zapojeny za lok·lku,
 ï p¯i p¯epojov·nÌ je zachov·v·no po¯adÌ (vagÛny se neprohazujÌ).
 */

char * printTrain( TTRAIN * t)
{
    TVAN *van = (TVAN *)malloc(sizeof(TVAN));
    van = t->m_Start;
    char * str = (char *)malloc(t->m_Count * 20);
    char * tmp = (char *)malloc(t->m_Count * 20);
    if(van != NULL)
    {
        strcpy(str,"[");
        while(van->m_Next != NULL)
        {
            sprintf(tmp,"[%s]->",van->m_To);
            strcat(str, tmp);
            van = van->m_Next;
        }
        sprintf(tmp,"[%s]]",van->m_To);
        strcat(str,tmp);
    }
    else return NULL;
    free(tmp);
    return str;
}
void   route ( const char * stationName, TTRAIN * train, TTRAIN * localTrain )
{
    if(train == localTrain) return;
    TVAN *van = (TVAN *)malloc(sizeof(TVAN));
    van = train->m_Start;
    printf("ROUTING @%s:\n\t[",stationName);
    while(van->m_Next != NULL)
    {
        printf("[%s]->",van->m_To);
        van = van->m_Next;
    }
    printf("[%s]]",van->m_To);
    van->m_Next = localTrain->m_Start;
    van = localTrain->m_Start;
    train->m_Count++;
    printf("  +  [");
    while(van->m_Next != NULL)
    {
        printf("[%s]",van->m_To);
        van = van->m_Next;
        train->m_Count++;
    }
    printf("[%s]]\n",van->m_To);
    localTrain->m_Start = NULL;
    localTrain->m_Count = 0;
    printf("\t%s  +  %s\n",printTrain(train),printTrain(localTrain));
    
    TVAN * prev = (TVAN *)malloc(sizeof(TVAN));
    van = train->m_Start;
    while(van->m_Next != NULL)
    {
        
        if(strcmp(train->m_Start->m_To, stationName) == 0)
        {
            printf("\n\tconnect FIRST[%s] to local\n",train->m_Start->m_To);
            train->m_Start = van->m_Next;
            train->m_Count--;
            
            localTrain->m_Start = van;
            localTrain->m_Start->m_Next = NULL;
            localTrain->m_Count++;
            van = train->m_Start;
            
            printf("\t%s  +  %s\n",printTrain(train),printTrain(localTrain));
            printf("train:%d & local:%d\n",train->m_Count, localTrain->m_Count);
            
            continue;
        }
        prev = van;
        van = van->m_Next;
        if(strcmp(van->m_To, stationName) == 0)
        {
            printf("route [%s]\n",van->m_To);
            prev->m_Next = van->m_Next;
            train->m_Count--;
            
            van->m_Next = NULL;
            
            if(localTrain->m_Start == NULL)
            {
                localTrain->m_Start = van;
                localTrain->m_Count++;
            }
            else
            {
                prev = localTrain->m_Start;
                while(prev->m_Next != NULL)
                {
                    prev = prev->m_Next;
                }
                prev->m_Next = van;
                localTrain->m_Count++;
            }
            
            
            printf("\t %s  +  %s\n",printTrain(train),printTrain(localTrain));
            printf("train:%d & local:%d\n",train->m_Count, localTrain->m_Count);
            van = train->m_Start;
        }
    }
    van = train->m_Start;
    printf("      train with %d vans: ", train->m_Count);
    while(van->m_Next != NULL)
    {
        printf("[%s]->",van->m_To);
        van = van->m_Next;
    }
    printf("[%s]\n", van->m_To);
    van = localTrain->m_Start;
    printf("      train with %d vans: ", localTrain->m_Count);
    if(localTrain->m_Start == NULL) return;
    while(van->m_Next != NULL)
    {
        printf("[%s]->",van->m_To);
        van = van->m_Next;
    }
    printf("[%s]\n", van->m_To);
}
#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    TTRAIN a = { NULL, 0 }, b = { NULL, 0 }, c = { NULL, 0 };
    char   tmpStr[100];
    
    connectVan ( &a, borrowVan ( "Praha" ), 0 );
    connectVan ( &a, borrowVan ( "Brno" ), 1 );
    connectVan ( &a, borrowVan ( "Ostrava" ), 0 );
    connectVan ( &a, borrowVan ( "Brno" ), 0 );
    printf("================================================================================================================\n");
    printf("res:  train with 4 vans: [Brno]->[Praha]->[Ostrava]->[Brno]");
    printf("\n****************************************************************************************************************\n\n");
    connectVan ( &b, borrowVan ( "Praha" ), 1 );
    connectVan ( &b, borrowVan ( "Plzen" ), 1 );
    connectVan ( &b, borrowVan ( "Ostrava" ), 0 );
    connectVan ( &b, borrowVan ( "Praha" ), 0 );
    printf("================================================================================================================\n");
    printf("res:  train with 4 vans: [Plzen]->[Praha]->[Ostrava]->[Praha]");
    printf("\n****************************************************************************************************************\n\n");
    route ( "Brno", &a, &b  );
    printf("================================================================================================================\n");
    printf("res:  train with 6 vans: [Praha]->[Ostrava]->[Plzen]->[Praha]->[Ostrava]->[Praha]\n");
    printf("res:  train with 2 vans: [Brno]->[Brno]");
    printf("\n****************************************************************************************************************\n\n");
    returnVans ( &a );
    returnVans ( &b );
    
    connectVan ( &a, borrowVan ( "Praha" ), 1 );
    connectVan ( &a, borrowVan ( "Brno" ), 1 );
    connectVan ( &a, borrowVan ( "Ostrava" ), 0 );
    connectVan ( &a, borrowVan ( "Brno" ), 0 );
    connectVan ( &a, borrowVan ( "Plzen" ), 0 );
    printf("================================================================================================================\n");
    printf("res:  train with 5 vans: [Brno]->[Praha]->[Ostrava]->[Brno]->[Plzen]");
    printf("\n****************************************************************************************************************\n\n");
    connectVan ( &b, borrowVan ( "Praha" ), 1 );
    connectVan ( &b, borrowVan ( "Ostrava" ), 1 );
    connectVan ( &b, borrowVan ( "Usti nad Labem" ), 1 );
    printf("================================================================================================================\n");
    printf("res:  train with 3 vans: [Usti nad Labem]->[Ostrava]->[Praha]");
    printf("\n****************************************************************************************************************\n\n");
    connectVan ( &c, borrowVan ( "Kosice" ), 0 );
    connectVan ( &c, borrowVan ( "Plzen" ), 0 );
    connectVan ( &c, borrowVan ( "Ostrava" ), 0 );
    printf("================================================================================================================\n");
    printf("res:  train with 3 vans: [Kosice]->[Plzen]->[Ostrava]");
    printf("\n****************************************************************************************************************\n\n");
    route ( "Brno", &a, &b  );
    printf("================================================================================================================\n");
    printf("res:  train with 6 vans: [Praha]->[Ostrava]->[Plzen]->[Usti nad Labem]->[Ostrava]->[Praha]\n");
    printf("res:  train with 2 vans: [Brno]->[Brno]");
    printf("\n****************************************************************************************************************\n\n");
    route ( "Praha", &a, &c  );
    printf("================================================================================================================\n");
    printf("res:  train with 7 vans: [Ostrava]->[Plzen]->[Usti nad Labem]->[Ostrava]->[Kosice]->[Plzen]->[Ostrava]\n");
    printf("res:  train with 2 vans: [Praha]->[Praha]");
    printf("\n****************************************************************************************************************\n\n");
    
    returnVans ( &a );
    returnVans ( &b );
    returnVans ( &c );
    // */
    strncpy ( tmpStr, "Praha", sizeof ( tmpStr ) );
    connectVan ( &a, borrowVan ( tmpStr ), 1 );
    strncpy ( tmpStr, "Brno", sizeof ( tmpStr ) );
    connectVan ( &a, borrowVan ( tmpStr ), 1 );
    strncpy ( tmpStr, "Ostrava", sizeof ( tmpStr ) );
    connectVan ( &a, borrowVan ( tmpStr ), 0 );
    strncpy ( tmpStr, "Brno", sizeof ( tmpStr ) );
    connectVan ( &a, borrowVan ( tmpStr ), 0 );
    printf("================================================================================================================\n");
    printf("res:  train with 4 vans: [Brno]->[Praha]->[Ostrava]->[Brno]");
    printf("\n****************************************************************************************************************\n\n");
    strncpy ( tmpStr, "Praha", sizeof ( tmpStr ) );
    connectVan ( &b, borrowVan ( tmpStr ), 1 );
    strncpy ( tmpStr, "Plzen", sizeof ( tmpStr ) );
    connectVan ( &b, borrowVan ( tmpStr ), 1 );
    strncpy ( tmpStr, "Ostrava", sizeof ( tmpStr ) );
    connectVan ( &b, borrowVan ( tmpStr ), 0 );
    strncpy ( tmpStr, "Praha", sizeof ( tmpStr ) );
    connectVan ( &b, borrowVan ( tmpStr ), 0 );
    printf("================================================================================================================\n");
    printf("res:  train with 4 vans: [Plzen]->[Praha]->[Ostrava]->[Praha]");
    printf("\n****************************************************************************************************************\n\n");
    route ( "Brno", &a, &b  );
    printf("================================================================================================================\n");
    printf("res:  train with 6 vans: [Praha]->[Ostrava]->[Plzen]->[Praha]->[Ostrava]->[Praha]\n");
    printf("res:  train with 2 vans: [Brno]->[Brno]");
    printf("\n****************************************************************************************************************\n\n");
    returnVans ( &a );
    returnVans ( &b );
    
    return 0;
}
#endif  /* __PROGTEST__ */
