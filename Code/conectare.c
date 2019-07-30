#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <my_global.h>

static char* host="localhost";
static char* user="root";
static char* pass="12345678";
static char* dbase="PROIECT";

unsigned int p=3306;
static char* unix_socket =NULL;
unsigned int flag=0;
int main()
{ char query[300];
	MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;
    conn = mysql_init(NULL);
    if (conn == NULL) {
        printf("eroare\n");
        exit(1);
    }
    my_bool reconnect = 1;
int success = mysql_options(conn, MYSQL_OPT_RECONNECT, &reconnect);
assert(success == 0);

    if (!mysql_real_connect(conn, host, user, pass, dbase, p, unix_socket, flag)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
    }
    printf("Bine!\n");
    char*n2="dodu";
    char*p2="12345678";
    sprintf(query,"select tip from utilizatori where nume=('%s') and parola=('%s')",n2,p2);
    if (mysql_query(conn, query)) {
      fprintf(stderr, "%s\n", mysql_error(conn));
      exit(1);
   }
   res = mysql_use_result(conn);

   printf("MySQL Tables in mysql database:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("%s \n", row[0]);
   mysql_free_result(res);
    mysql_close(conn);
 //gcc -o conectare $(mysql_config --cflags) conectare.c $(mysql_config --libs)

}
