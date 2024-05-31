#include "lite.h"
#include "log.h"

static sqlite3 *db;

void open_sqlite(const char* dbname) {
    if (sqlite3_open(dbname, &db) != SQLITE_OK) {
        err_sys("Ошибка открытия БД: %s\n", sqlite3_errmsg(db));
    }
}

void close_sqlite() {
    sqlite3_close(db);
}

int is_valid_sqlite_param(const char* table, const char* column, char* err) {

    int not_null, pk, autoinc;
    const char *data_type = NULL;
    const char *collation = NULL;
    int result = sqlite3_table_column_metadata(db, NULL, table, column,
                                                &data_type, &collation, 
                                                &not_null, &pk, &autoinc);
    if(result == SQLITE_ERROR) {
        strcpy(err, sqlite3_errmsg(db));
        return -1;
    }

    if (strcmp(data_type, "TEXT") == 0 || strcmp(data_type, "BLOB") == 0) {
        sprintf(err, "колонка имеет не числовой тип данных: %s", data_type);
        return -1;
    }
    return 0;
}


int select_sqlite_stat(const char* query, double* value, char* err) {
    int result;
    sqlite3_stmt* stmt; 

    if(sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        sprintf(err, "Ошибка подготовки SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    result = sqlite3_step(stmt);
    
    if(result != SQLITE_ROW) {
        sprintf(err, "Ошибка выполнения SQL-запроса: %s", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }
    *value = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    return 0;
}