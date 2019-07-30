
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
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

#define PORT 2024

extern int errno;

void handler(int sig)
{
    int status;
    while(waitpid(-1, &status, WNOHANG) > 0){}
}

int scriere(char* mesaj,int client);
int citire(char* mesaj,int client,int marime);

char query[1000];
char nume[60];
char parola[60];

int main ()
{
  struct sockaddr_in server;
  struct sockaddr_in from;
  char msg[300];
  char msgrasp[300];
  char *ip;
  char site[200];
  int sd, sent, i;
  FILE *fp;
  int pid1;
  MYSQL *conn;
    conn = mysql_init(NULL);
    MYSQL_RES *res;
    MYSQL_ROW row;

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
    printf("BINE!\n");



  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }


  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));


    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }
  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  while (1)
    {
        i = 0;
      sent = 0;
      int client;
      int length = sizeof (from);

      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

      signal(SIGCHLD, handler);
      client = accept (sd, (struct sockaddr *) &from, &length);
	if ((pid1 = fork()) < 0)
      {
          perror("fork() 1");
          exit(1);
      }
      else if (pid1)
      {
          close(client);
      }
      else {
	  while(1)
	  {
      fflush(stdout);
      printf("---->\n");
      if(citire(msg,client,sizeof(msg))==-1)
      break;
      if(strcmp(msg,"logare")==0)
      {
        if(citire(nume,client,sizeof(nume))==-1)
        break;
        if(citire(parola,client,sizeof(parola))==-1)
        break;
        sprintf(query,"SELECT tip FROM utilizatori WHERE nume=('%s') AND parola=('%s')",nume,parola);
        if (mysql_query(conn, query)) {
          fprintf(stderr, "%s\n", mysql_error(conn));
          break;
       }
       res = mysql_use_result(conn);
       row = mysql_fetch_row(res);
       if(row==NULL)
       {
         if(scriere("nu",client)==-1)
         break;
       }
       else
       if(scriere(row[0],client)==-1)
       exit(1);
       mysql_free_result(res);
      }
        else
        if(strcmp(msg,"inscrie")==0)
        {
          int ds=0;
          while(ds==0){
            printf("===\n");
          if(citire(nume,client,sizeof(nume))==-1)
          break;
          if(strcmp(nume,"inapoi")==0)
          break;
          sprintf(query,"SELECT nume FROM utilizatori WHERE nume=('%s')",nume);
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            break;
         }
         res = mysql_use_result(conn);
         row = mysql_fetch_row(res);
         if(row==NULL)
         {
           if(scriere("ok",client)==-1)
           break;
           mysql_free_result(res);
           ds=1;
         }
         else
         {
           mysql_free_result(res);
           if(scriere("!ok",client)==-1)
           break;
         }

        }
        while(ds==1)
        {
          printf("Parola\n");
          if(citire(parola,client,sizeof(parola))==-1)
          break;
          if(strcmp(parola,"inapoi")==0)
          break;
          sprintf(query,"INSERT INTO utilizatori(nume,parola,vot)VALUES('%s','%s','1')",nume,parola);
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            break;
         }
         if(scriere("ok2",client)==-1)
         break;
         break;

        }

        }
        else
        if(strcmp(msg,"inserare_melodie")==0)
        {
          int ds=0;
          char melodie[60];
          char descriere[500];
          char genuri[300];
          char link[300];
          while(ds==0)
          {
            if(citire(melodie,client,sizeof(melodie))==-1)
            break;
            if(strcmp(melodie,"inapoi")==0)
            break;
            sprintf(query,"SELECT nume FROM melodii WHERE nume=('%s')",melodie);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              break;
           }
           res = mysql_use_result(conn);
           row = mysql_fetch_row(res);
           if(row==NULL)
           { printf("MEl valida\n");
             if(scriere("ok",client)==-1)
             break;
             mysql_free_result(res);
             ds=1;
           }
           else
           {
             mysql_free_result(res);
             if(scriere("!ok",client)==-1)
             exit(1);
           }
          }
          if(ds==1)
          {
          if(citire(descriere,client,sizeof(descriere))==-1)
          exit(1);
          if(strcmp(descriere,"inapoi")!=0)
          {if(scriere("ok",client)==-1)
          exit(1);
          if(citire(genuri,client,sizeof(genuri))==-1)
          exit(1);
          if(strcmp(genuri,"inapoi")!=0)
          {
            printf("ceva\n");
          if(scriere("ok",client)==-1)
          exit(1);
          printf("ceva\n");

          if(citire(link,client,sizeof(link))==-1)
          exit(1);
          if(strcmp(link,"inapoi")!=0)
          {if(scriere("ok",client)==-1)
          exit(1);
          printf("%s\n%s\n%s\n%s\n",melodie,descriere,genuri,link);
          sprintf(query,"INSERT INTO melodii(nume,descriere,genuri,link)VALUES('%s','%s','%s','%s')",melodie,descriere,genuri,link);
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            exit(1);
         }

         sprintf(query,"INSERT INTO top(nume,genuri)VALUES('%s','%s')",melodie,genuri);
         if (mysql_query(conn, query)) {
           fprintf(stderr, "%s\n", mysql_error(conn));
           exit(1);
        }
        if(scriere("melodie",client)==-1)
          exit(1);
        }}}
        }

        }
        else
        if(strcmp(msg,"vizualizare top")==0)
        {
          bzero(query,sizeof(query));
          sprintf(query,"SELECT nume,genuri,voturi FROM top order by voturi desc");
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            break;
         }
         res = mysql_use_result(conn);
         char linie[500];
         bzero(linie,sizeof(linie));
         int n_s=0;
         while(row = mysql_fetch_row(res))
         {
            sprintf(linie,"Nume melodie: %s | Genuri: %s | Voturi: %s\n",row[0],row[1],row[2]);
            printf("------ %s",linie);
            n_s+=strlen(linie);
         }
         char nm[10];
         sprintf(nm,"%d",n_s);
         if(scriere(nm,client)==-1)
         exit(1);
         mysql_free_result(res);
         int nr=atoi(nm);
         char rezultat[nr];
         bzero(rezultat,nr);
         if (mysql_query(conn, query)) {
           fprintf(stderr, "%s\n", mysql_error(conn));
           break;
        }
        res = mysql_use_result(conn);
        bzero(linie,sizeof(linie));
        while(row = mysql_fetch_row(res))
        {
           sprintf(linie,"Nume melodie: %s | Genuri: %s | Voturi: %s\n",row[0],row[1],row[2]);
           if(strlen(rezultat)==0)
           strcpy(rezultat,linie);
           else
           strcat(rezultat,linie);
        }
        printf("%s\n",rezultat);
        mysql_free_result(res);
        if(citire(msg,client,2)==-1)
        exit(1);
        rezultat[nr-1]='\0';
        if(scriere(rezultat,client)==-1)
        exit(1);
        }
        else
        if(strcmp(msg,"vizualizare top2")==0)
        {
          bzero(query,sizeof(query));
          char gen[10];
          bzero(gen,10);
          if(citire(gen,client,10)==-1)
          exit(1);
          sprintf(query,"SELECT nume,genuri,voturi FROM top where instr(genuri,'%s')>0 order by voturi desc",gen);
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            break;
         }
         res = mysql_use_result(conn);
         char linie[500];
         bzero(linie,sizeof(linie));
         int n_s=0;
         while(row = mysql_fetch_row(res))
         {
            sprintf(linie,"Nume melodie: %s | Genuri: %s | Voturi: %s\n",row[0],row[1],row[2]);
            printf("------ %s",linie);
            n_s+=strlen(linie);
         }
         char nm[10];
         sprintf(nm,"%d",n_s);
         if(scriere(nm,client)==-1)
         exit(1);
         mysql_free_result(res);
         int nr=atoi(nm);
         if(nr==0)
         break;
         char rezultat[nr];
         bzero(rezultat,nr);
         if (mysql_query(conn, query)) {
           fprintf(stderr, "%s\n", mysql_error(conn));
           break;
        }
        res = mysql_use_result(conn);
        bzero(linie,sizeof(linie));
        while(row = mysql_fetch_row(res))
        {
           sprintf(linie,"Nume melodie: %s | Genuri: %s | Voturi: %s\n",row[0],row[1],row[2]);
           if(strlen(rezultat)==0)
           strcpy(rezultat,linie);
           else
           strcat(rezultat,linie);
        }
        printf("%s\n",rezultat);
        mysql_free_result(res);
        if(citire(msg,client,2)==-1)
        exit(1);
        rezultat[nr-1]='\0';
        if(scriere(rezultat,client)==-1)
        exit(1);
        }
        else
        if(strcmp("votare melodie",msg)==0)
        {
          int ds=0;
          char melodie[60];
          char dc[2];
          int l=0;
          while(ds==0)
          {
            if(citire(melodie,client,sizeof(melodie))==-1)
            break;
            if(strcmp(melodie,"inapoi")==0)
            break;
            sprintf(query,"SELECT nume FROM melodii WHERE nume=('%s')",melodie);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              break;
           }
           res = mysql_use_result(conn);
           row = mysql_fetch_row(res);
           if(row!=NULL)
           { printf("MEl valida\n");
           fflush(stdout);
             if(scriere("ok",client)==-1)
             break;
             mysql_free_result(res);
             ds=1;
           }
           else
           {
             mysql_free_result(res);
             if(scriere("!ok",client)==-1)
             exit(1);
           }
          }

          if(ds==1)
          {

            sprintf(query,"SELECT vot FROM utilizatori WHERE nume=('%s') and vot='1'",nume);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              break;
           }
           res = mysql_use_result(conn);
           row = mysql_fetch_row(res);
           if(row==NULL)
           {
             if(scriere("!ok",client)==-1)
             exit(1);
             mysql_free_result(res);
           }
           else
           {
             mysql_free_result(res);
             sprintf(query,"SELECT utilizatori FROM top WHERE nume=('%s') and instr(utilizatori,'%s')>0",melodie,nume);
             if (mysql_query(conn, query)) {
               fprintf(stderr, "%s\n", mysql_error(conn));
               break;
            }
            res = mysql_use_result(conn);
            row = mysql_fetch_row(res);

            if(row!=NULL)
            {
              if(scriere("ko",client)==-1)
              exit(1);
              mysql_free_result(res);
            }
            else
            {
              mysql_free_result(res);
              sprintf(query,"update top set voturi=voturi+1 ,utilizatori=concat('%s',',',ifnull(utilizatori,' ')) where nume=('%s') ",nume,melodie);
              printf("query=%s\n",query);
              if (mysql_query(conn, query)) {
                fprintf(stderr, "%s\n", mysql_error(conn));
                break;
             }

              if(scriere("ok",client)==-1)
              exit(1);
            }
           }
          }
        }
        else
        if(strcmp(msg,"scrie comentariu")==0)
        {
          int ds=0;
          char melodie[60];
          char dc[2];
          int l=0;
          char v[8];
          bzero(v,8);
          while(ds==0)
          {
            if(citire(melodie,client,sizeof(melodie))==-1)
            break;
            if(strcmp(melodie,"inapoi")==0)
            break;
            sprintf(query,"SELECT nume FROM melodii WHERE nume=('%s')",melodie);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              break;
           }
           res = mysql_use_result(conn);
           row = mysql_fetch_row(res);
           if(row!=NULL)
           { printf("MEl valida\n");
           fflush(stdout);
             if(scriere("ok",client)==-1)
             exit(1);
             mysql_free_result(res);
             ds=1;
           }
           else
           {
             mysql_free_result(res);
             if(scriere("!ok",client)==-1)
             exit(1);
           }
          }
          char comentariu[300];
          bzero(comentariu,300);
          if(citire(comentariu,client,300)==-1)
          exit(1);
          sprintf(query,"insert into comentarii(melodie,utilizator,comentariu) values('%s','%s','%s')",melodie,nume,comentariu);
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            break;
         }

          if(scriere("ok",client)==-1)
          exit(1);

        }
        else
        if(strcmp(msg,"vezi informatii melodie")==0)
        {
          int ds=0;
          char melodie[60];
          char dc[2];
          int l=0;
          char v[8];
          bzero(v,8);
          while(ds==0)
          {
            if(citire(melodie,client,sizeof(melodie))==-1)
            break;
            if(strcmp(melodie,"inapoi")==0)
            break;
            sprintf(query,"SELECT nume FROM melodii WHERE nume=('%s')",melodie);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              break;
           }
           res = mysql_use_result(conn);
           row = mysql_fetch_row(res);
           if(row!=NULL)
           { printf("MEl valida\n");
           fflush(stdout);
             if(scriere("ok",client)==-1)
             exit(1);
             mysql_free_result(res);
             ds=1;
           }
           else
           {
             mysql_free_result(res);
             if(scriere("!ok",client)==-1)
             exit(1);
           }
          }
          int dimensiune_r=0;
          sprintf(query,"select a.nume , a.descriere , a.genuri , a.link , b.utilizator , b.comentariu from melodii as a left join comentarii as b on b.melodie = a.nume where a.nume='%s'",melodie);
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            break;
         }
         res = mysql_use_result(conn);
         int nr_1=0;
         char partial[1000];
         bzero(partial,1000);
         row = mysql_fetch_row(res);
         sprintf(partial," Nume melodie: %s\n Descriere: %s\n Genuri: %s\n Link: %s\n Comentarii: ",row[0],row[1],row[2],row[3]);
         if(row[4])
         sprintf(partial,"%s\n utilizator: %s\n",partial,row[4]);
         if(row[5])
         sprintf(partial,"%s\n---> comentariu: %s\n",partial,row[5]);
         printf("%s\n",partial);
         while(row = mysql_fetch_row(res))
         {

           dimensiune_r=strlen(row[4])+strlen(row[5])+30;

         }
         mysql_free_result(res);
         char rez_f[dimensiune_r+strlen(partial)];
         printf("nr=%d\n",sizeof(rez_f));
         bzero(rez_f,sizeof(rez_f));
         sprintf(query,"select a.nume , a.descriere , a.genuri , a.link , b.utilizator , b.comentariu from melodii as a left join comentarii as b on b.melodie = a.nume where a.nume='%s'",melodie);
         if (mysql_query(conn, query)) {
           fprintf(stderr, "%s\n", mysql_error(conn));
           break;
        }
        res = mysql_use_result(conn);
        row = mysql_fetch_row(res);
        strcpy(rez_f,partial);
        while(row = mysql_fetch_row(res))
        {
          if(row[4])
          sprintf(rez_f,"%s\n utilizator: %s\n",rez_f,row[4]);
          if(row[5])
          sprintf(rez_f,"%s\n---> comentariu: %s\n",rez_f,row[5]);
        }
         mysql_free_result(res);
         char vol[100];
         bzero(vol,100);
         sprintf(vol,"%d",strlen(rez_f)+5);
         if(scriere(vol,client)==-1)
         exit(1);
         if(citire(v,client,8)==-1)
         exit(1);
         if(scriere(rez_f,client)==-1)
         exit(1);

        }
        else
        if(strcmp(msg,"str_m")==0)
        {
          int ds=0;
          char melodie[60];
          char dc[2];
          int l=0;
          char v[8];
          bzero(v,8);
          while(ds==0)
          {
            if(citire(melodie,client,sizeof(melodie))==-1)
            break;
            if(strcmp(melodie,"inapoi")==0)
            break;
            sprintf(query,"SELECT nume FROM melodii WHERE nume=('%s')",melodie);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              break;
           }
           res = mysql_use_result(conn);
           row = mysql_fetch_row(res);
           if(row!=NULL)
           { printf("MEl valida\n");
           fflush(stdout);
             if(scriere("ok",client)==-1)
             exit(1);
             mysql_free_result(res);
             ds=1;
           }
           else
           {
             mysql_free_result(res);
             if(scriere("!ok",client)==-1)
             exit(1);
           }
          }
          if(ds==1)
          {if(citire(v,client,8)==-1)
          exit(1);
          sprintf(query,"delete from melodii where nume ='%s'",melodie);
          if (mysql_query(conn, query)) {
            fprintf(stderr, "%s\n", mysql_error(conn));
            break;
         }
         if(scriere("ok",client)==-1)
         exit(1);}
        }
        else
        if(strcmp(msg,"str_v")==0)
        {
          int ds=0;
          char num[60];
          char dc[2];
          int l=0;
          char v[8];
          bzero(v,8);
          while(ds==0)
          {
            if(citire(num,client,sizeof(num))==-1)
            break;
            if(strcmp(num,"inapoi")==0)
            break;
            sprintf(query,"SELECT nume FROM utilizatori WHERE nume=('%s') and tip='obisnuit' and vot='1'",num);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              break;
           }
           res = mysql_use_result(conn);
           row = mysql_fetch_row(res);
           if(row!=NULL)
           {
           fflush(stdout);
             if(scriere("ok",client)==-1)
             exit(1);
             mysql_free_result(res);
             ds=1;
           }
           else
           {
             mysql_free_result(res);
             if(scriere("!ok",client)==-1)
             exit(1);
           }
          }
          printf("vedem\n");
          if(ds==1)
          {
            sprintf(query,"update utilizatori set vot='0' WHERE nume=('%s')",num);
            if (mysql_query(conn, query)) {
              fprintf(stderr, "%s\n", mysql_error(conn));
              exit(1);
              if(scriere("ok",client)==-1)
              exit(1);
          }
        }}
        else
        if(strcmp(msg,"iesire")==0)
        break;

    }
          close (client);
          close(sd);
          exit(1);}

    }				/* while */
}				/* main */

int scriere(char* mesaj,int client)
{
	fflush (stdout);
	if (write (client, mesaj, strlen(mesaj)) <= 0)
    	{
      	perror ("[servev]Eroare la write() spre client.\n");
      	return -1;
    	}
printf("Mesajul a fost transmis cu succes .   %s\n",mesaj);
fflush (stdout);
return 0;
}
int citire(char* mesaj,int client,int marime)
{       printf("marime   %d\n",marime);
	bzero (mesaj, marime);
	if (read (client, mesaj, marime) <= 0)
    	{
      	perror ("[server]Eroare la read() de la clientr.\n");
      	return -1;
    	}
printf("S-a citit %s\n",mesaj);
fflush (stdout);
return 0;
}
