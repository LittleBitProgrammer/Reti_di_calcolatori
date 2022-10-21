/**
 * @file    package_utility.h
 * @author  Roberto Vecchio, Francesco Mabilia & Gaetano Ippolito
 * @brief   Il seguente header file, viene utilizzato per definire i pacchetti di livello applicazione che verranno comunicati sulla rete tra client/server
 * 
 * @type    Libreria
 * @version 1.0
 */

/* Direttive precompilatore volte a definire la libreria @package_utility.h */
#ifndef PACKAGE_UTILITY_H
#define PACKAGE_UTILITY_H

/* 
 * ==========================
 * =         Import         =
 * ==========================
 */
#include <time.h>
#include <netinet/in.h>
#include "bool_utility.h"

#define MAX_CODE_LEN 21

/* 
 * ================================
 * =      Type Definition         =
 * ================================
 */

/**
 * @brief    Struttura utile a rappresentare il pacchetto da comunicare al "server centro vaccinale", per procedere alla sottoscrizione del sistema "Green pass" ed ottenere il
 *           documento di valità.
 * 
 * @property card_code - campo utile a contenere il codice di tessera sanitaria, formata da 20 caratteri decimali. Viene aggiunto un carattere in più
 *           per memorizzare il carattere di terminazione stringa.
 *
 * @property vaccination_date - Data di vaccinazione dell'utente, memorizzata attraverso una struttura struct tm, definita in @time.h
 * 
 * @struct   <--------------------Vaccinated_package------------------------>
 *           <-------- card_code --------> <-------- vaccination date ------>
 *           ________________________________________________________________
 *          |     21 byte (char[21])     |       56 byte (struct tm)        | 
 *          ----------------------------------------------------------------
 */
typedef struct
{
    char card_code[MAX_CODE_LEN];
    struct tm vaccination_date;
} Vaccinated_package;

/**
 * @brief 
 * 
 * @property 
 * @property 
 * 
 * @struct
 */
typedef struct
{
    Vaccinated_package vaccinated_package;
    struct tm expiration_date;
} Subscribe_package;

/**
 * @brief 
 * 
 * @property 
 * @property 
 * @property
 * 
 * @struct
 */
typedef struct
{
    bool open_file_flag;
    bool write_file_flag;
    bool read_file_flag;
} File_flags;

/**
 * @brief    Struttura utile a rappresentare la response del server "centrale"/"centro vaccinale" al client "vaccinato". In tale pacchetto, previsto dal protocollo a 
 *           livello applicazione, sono contenute le informazioni inerenti all'esito della sottoscrizione al sistema "Green pass" e ad errori che possono avvenire durante 
 *           tale processo. In questo modo, il client può essere notificato dell'avvenuta iscrizione e di eventuali errori che possono intercorrere durante la registrazione 
 *           al sistema.
 * 
 * @property result_flag - tale valore booleano è utile ad interpretare l'esito dell'avvenuta registrazione presso il sistema "Green pass" 
 * @property file_flag - struttura utile a rappresentare gli errori che possono intercorrere durante la registrazione al sistema. In particolare, abbiamo progettato 
 *           il sistema "Green pass", in modo che salvi permanentemente le informazioni all'interno di un file organizzato in campi, con analogia alle tabelle di un 
 *           database relazionale. Per tal motivo si è deciso di notificare il client sui possibili errori, che possono avvenire durante l'apertura, la scrittura e 
 *           la lettura su fiel
 * 
 * @struct   <------------------------------File_result----------------------------->
 *           <--------- result_flag ---------> <------------ file_flags ------------>
 *           ________________________________________________________________________
 *          |          4 byte (int)          |       12 byte (struct File_flags)     |
 *          --------------------------------------------------------------------------
 */
typedef struct
{
    bool result_flag;
    File_flags file_flags;
} File_result;

/**
 * @brief 
 * 
 * @property 
 * @property 
 * @property
 * @property 
 * @property
 * 
 * @struct
 */
typedef struct
{
    bool is_green_pass_valid;
    struct tm expiration_date;
    struct tm last_update;
    char motivation[13];
    File_flags file_flags;
} Reviser_package;

/**
 * @brief 
 * 
 * @property 
 * @property 
 * 
 * @struct
 */
typedef struct
{
    int index_list;
    char motivation[13];
} Administrator_request_package;

/**
 * @brief 
 * 
 * @property 
 * @property 
 * @property
 * 
 * @struct
 */
typedef struct
{
    bool not_updatable;
    char code[MAX_CODE_LEN];
    Reviser_package reviser_package;
} Administrator_response_package;

/* Direttiva precompilatore utilizzata per indicare la delimitazione di fine libreria @package_utility.h */
#endif //PACKAGE_UTILITY_H
