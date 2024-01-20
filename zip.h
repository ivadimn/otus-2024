#ifndef _ZIP_H_
#define _ZIP_H_

#include <stdint.h>

#define CFH_SIG   0x02014b50   //сигнатура заголовка файла центрального каталога
#define LFH_SIG   0x04034b50   //сигнатура заголовка локального файла 
#define EOCDR_SIG 0x06054b50   //сигнатура конца записи центрального каталога 
#define EOCDR_BASE_SZ 22

#define SECONDS(a) (((a) & (0x1F)) * 2)
#define MINUTES(a) (((a) & (0x3F)) >> 5)
#define HOURS(a) ((a) >> 11)

#define DAY(a) ((a) & (0x1F))
#define MONTH(a) ((((a) & (0xF)) >> 5) + 1)
#define YEAR(a) (((a) >> 9) + 1980)

/*
Определение структуры: Конец записи центрального каталога  
*/
//*********************************************************************
typedef struct __attribute__((__packed__)) {
    uint16_t disk_number;
    uint16_t cfh_start_disk;
    uint16_t cfh_disk_count;
    uint16_t cfh_count;
    uint32_t cfh_size;
    uint32_t cfh_offset;
    uint16_t comment_len;
} in_ecodr_t;

typedef struct __attribute__((__packed__)) {
    in_ecodr_t ecodr;
    char* comment;
    uint8_t is_valid;
} ecodr_t;

ecodr_t* create_ecodr();
void delete_ecodr(ecodr_t*);
void add_comment(ecodr_t*, char*);
void print_ecodr_info(ecodr_t*);
//***********************************************************************


/*
Структуры описывающие записи центрального каталга
*/
typedef struct __attribute__((__packed__)) {
    //4
    uint16_t version;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t compression;  //метод сжатия
    uint16_t modification_time;
    uint16_t modification_date; 
    uint32_t crc;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t filename_len;
    uint16_t extrafield_len;
    uint16_t filecomm_len;
    uint16_t disk_start;
    uint16_t internal_attr;
    uint32_t external_attr;
    uint32_t offset_local_header;
} cfh_short_t;

typedef struct {
    cfh_short_t cfh;
    char* filename;
    char* filecomment;
    uint8_t* extra;
} cfh_t;

cfh_t** create_cfh_array(uint16_t); 
cfh_t* create_cfh();
void delete_cfh(cfh_t*);
void delete_cfh_array(uint16_t, cfh_t**); 
void add_file(cfh_t*, char*);
void add_filecomment(cfh_t*, char*);
void add_extradata(cfh_t*, uint8_t*);
void print_file_info(uint16_t, cfh_t*); 


//********************************************************************************
cfh_t** zip_contains(char*, ecodr_t*);
int get_str_time(char*, uint16_t);
int get_str_date(char*, uint16_t);
int zip_preview(char*, ecodr_t*);

#endif