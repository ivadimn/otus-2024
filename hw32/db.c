#include "db.h"
#include "log.h"
#include "lite.h"
#include "psql.h"


static engine_t db_engine;
static char sql[SQL_LEN];
static char error_message[ERR_MSG_LEN];


static char* sql_stat = "SELECT %s(%s) FROM %s; ";
static char* sql_disp = "SELECT AVG((%s - %f) * (%s - %f)) FROM %s; ";


void open_db(conninfo_t* conninfo) {

    if (strcmp(conninfo->dbms, "SQLITE") == 0) {
        db_engine = SQLITE;
        open_sqlite(conninfo->dbname);
    } else if (strcmp(conninfo->dbms, "POSTGRESQL") == 0) {
        db_engine = POSTGRESQL;
        open_psql(conninfo->hostaddr, conninfo->dbname, 
                      conninfo->user, conninfo->password);
    } else {
        err_quit("Неизвестный тип СУБД '%s'", conninfo->dbms);
    }
    
}

void close_db(void) {
    switch (db_engine)  {
        case SQLITE:
            close_sqlite();
            break;
        case POSTGRESQL:
            close_psql();
            break;
        default:
            break;
    }
}

char* get_err_msg() {
    return error_message;
}


int is_valid_param(const char* table, const char* column) {

    switch (db_engine)  {
        case SQLITE:
            return is_valid_sqlite_param(table, column, error_message);
        case POSTGRESQL:
            return is_valid_psql_param(table, column, error_message);
        default:
            return INVALID_PARAM;
    }
    
}

static int select_stat(const char *fun_name, const char* table, const char* column, double* result) {
    
    double r_avg;
    if (strcmp(fun_name, "DISP") == 0) {
        avg(table, column, &r_avg);
        sprintf(sql, sql_disp, column, r_avg, column, r_avg, table);    
    } else {
        sprintf(sql, sql_stat, fun_name, column, table);
    }
   
    switch (db_engine)
    {
        case SQLITE:
            return select_sqlite_stat(sql, result, error_message);
        case POSTGRESQL:
            return select_psql_stat(sql, result, error_message);
        default:
            return -1;
    }
}


int avg(const char* table, const char* column, double* result) {
    return select_stat("AVG", table, column, result);
}

int min(const char* table, const char* column, double* result) {
    return select_stat("MIN", table, column, result);
}

int max(const char* table, const char* column, double* result) {
    return select_stat("MAX", table, column, result);
}

int sum(const char* table, const char* column, double* result) {
    return select_stat("SUM", table, column, result);
}

int disp(const char* table, const char* column, double* result) {
    return select_stat("DISP", table, column, result);
}
