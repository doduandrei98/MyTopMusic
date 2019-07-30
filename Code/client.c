
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

extern int errno;

int port;
int scriere(char* mesaj,int sd);
int citire(char* mesaj,int sd,int marime);
char nume[65];
char parola[65];
char tip[8];
int ok;

int main (int argc, char *argv[])
{
  int sd;
  struct sockaddr_in server;
  char msg[300];


  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }


  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }


  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
    while(ok!=-1){
      while(ok==0)
      {
        printf("Trebuie sa va logati sau inscrie intai!\nApasati 1 pt inscriere 2 pt logare si 3 pt iesire\ncomanda->");
        bzero(msg,sizeof(msg));
        fgets(msg,5,stdin);
        msg[strlen(msg)-1]='\0';
        if(strcmp(msg,"1")==0)
        {
          if(scriere("inscrie",sd)==-1)
          {
            close(sd);
            break;}
            ok=inscriere(&nume,parola,sd);
            if(ok==2)
            printf("Bine ati venit %s!\n",nume);
        }
          else
          if(strcmp(msg,"2")==0)
          {
            if(scriere("logare",sd)==-1)
            {
              close(sd);
              break;}
              ok=logare(&nume,parola,sd);
              if(ok==0)
              printf("nume sau parola gresita sau cont inexistent\n");
              else
              printf("Bine ati venit %s\n",nume);

          }
            else
            if(strcmp(msg,"3")==0)
            {ok=-1;
              if(scriere("iesire",sd)==-1)
              {
                close(sd);
                break;
              }
            }

      }
      while(ok==1)
      {
        printf("Apasati tasta pentru a utiliza o comanda!\nApasati:\n 1 pt vizualizare top\n 2 pt vizualizare top dupa gen\n 3 pt inserare melodie\n 4 votare melodie\n 5 pt comentare la o melodie\n 6 vezi comentarii la o melodie\n 7 sterge o melodie\n 8 elimina drepturile de vot a unui utilizator anonim \n 9 iesire\ncomanda->");
        bzero(msg,sizeof(msg));
        fgets(msg,5,stdin);
        msg[strlen(msg)-1]='\0';
        if(strcmp("7",msg)==0)
        {
          if(scriere("str_m",sd)==-1)
          {
            close(sd);
            exit(1);
          }
          char melodie[65];
          int ds=0;
          char v[8];
          while(ds==0)
          {
            printf("Nume Melodie\n--->)");
            bzero(melodie,sizeof(melodie));
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              exit(1);
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie inexistenta\n");
          }
          if(ds==1)
          {
            if(scriere("ok",sd)==-1)
            exit(1);
            if(citire(v,sd,8)==-1)
            exit(1);
            if(strcmp("ok",v)==0)
            printf("Ati sters cu succes  ok=%s!\n",v);
          }
        }
        else
        if(strcmp("8",msg)==0)
        {
          if(scriere("str_v",sd)==-1)
          {
            close(sd);
            exit(1);
          }
          char melodie[65];
          int ds=0;
          char v[8];
          while(ds==0)
          {
            printf("Nume utilizator\n--->)");
            bzero(melodie,sizeof(melodie));
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              exit(1);
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Vom lua dreptul acestui utilizator \n");
            }
            else
            printf("Utilizator inexistent sau de tip admin sau deja fara drepturi\n");
          }
          if(ds==1)
          {
            if(scriere("ok",sd)==-1)
            exit(1);
            if(strcmp(v,"ok")==0)
            printf("Am luat dreptul lui %s\n",melodie);

          }
        }
        else
        if(strcmp(msg,"9")==0)
        {ok=-1;
          if(scriere("iesire",sd)==-1)
          {
            close(sd);
            break;
          }
        }
        else
        if(strcmp(msg,"1")==0)
        {
          if(scriere("vizualizare top",sd)==-1)
          {
            close(sd);
            ok=-1;
            break;
          }
          char nm[10];
          bzero(nm,10);
          if(citire(nm,sd,10)==-1)
          exit(1);
          if(scriere("ok",sd)==-1)
          exit(1);
          printf("nm=%s\n",nm );
          int nr=atoi(nm);
          printf("nr=%d\n",nr);
          char rezultat[nr];
          bzero(rezultat,nr);
          if(citire(rezultat,sd,nr)==-1)
          exit(1);
          printf("Top\n%s\n",rezultat);
        }
        else
        if(strcmp(msg,"2")==0)
        {
          if(scriere("vizualizare top2",sd)==-1)
          {
            close(sd);
            ok=-1;
            break;
          }
          char gen[10];
          bzero(gen,10);
          while(1)
          {
            printf("Selectati gen:\n1->rock\n2->hip-hop\n3->dance\n4->opera\n");
            bzero(msg,sizeof(msg));
            fgets(msg,5,stdin);
            msg[strlen(msg)-1]='\0';
            if(strcmp(msg,"1")==0)
            {
              strcpy(gen,"rock");
              break;
            }
            else
            if(strcmp(msg,"2")==0)
            {
              strcpy(gen,"hip-hop");
              break;
            }
            else
            if(strcmp(msg,"3")==0)
            {
              strcpy(gen,"dance");
              break;
            }
            else
            if(strcmp(msg,"4")==0)
            {
              strcpy(gen,"opera");
              break;
            }
            else
            printf("Comanda invalida\n");

          }
          if(scriere(gen,sd)==-1)
          exit(1);
          char nm[10];
          bzero(nm,10);
          if(citire(nm,sd,10)==-1)
          exit(1);
          if(scriere("ok",sd)==-1)
          exit(1);
          printf("nm=%s\n",nm );
          int nr=atoi(nm);
          printf("nr=%d\n",nr);
          if(nr==0)
          {
            printf("Nici o melodie cu acest gen\n");
            break;
          }
          else{
          char rezultat[nr];
          bzero(rezultat,nr);
          if(citire(rezultat,sd,nr)==-1)
          exit(1);
          printf("Top\n%s\n",rezultat);
        }
        }
        else
        if(strcmp(msg,"3")==0)
        {
          char melodie[65];
          char descriere[505];
          char genuri[300];
          char link[303];
          int ds=0;
          char v[8];
          if(scriere("inserare_melodie",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie(maxim 60 caractere si nu trebuie sa fie gol:\nscrie inapoi sa te intorci la meniul utilizatorului\n--->)");
            bzero(melodie,sizeof(melodie));
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(strlen(melodie)==0||strlen(melodie)>60)
            printf("Numar prea mare de caractere sau spatiu gol\n");
            else
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie existenta deja\n");
          }
          while(ds==1)
          {
              printf("inapoi pt meniu.\nScrie descrierea(maxim 500 caractere.\n)");
              bzero(descriere,sizeof(descriere));
              fgets(descriere,505,stdin);
              descriere[strlen(descriere)-1]='\0';
              if(strlen(descriere)>500)
              printf("Numar prea mare de caractere!\n" );
              else
              if(strcmp(descriere,"inapoi")==0)
              {
                if(scriere(descriere,sd)==-1)
                exit(1);
                break;
              }else
              if(scriere(descriere,sd)==-1)
              break;
              if(citire(v,sd,8)==-1)
              break;
              if(strcmp(v,"ok")==0)
              ds=2;
              else
              printf("S-a pierdut ceva la transport\n");

          }
          while(ds==2)
          {
            printf("inapoi pt meniu.\n");
            int k=1;
            char v[8];
            bzero(v,sizeof(v));
            bzero(genuri,sizeof(genuri));
            while(1)
            {
              printf("Genuri selectate:%s\n",genuri);
              printf("apasati:\n1 pentru rock\n2 pentru hip-hop\n3 pentru dance \n4 pentru opera\n5 gata \n----->");
              bzero(v,sizeof(v));
              fgets(v,8,stdin);
              v[strlen(v)-1]='\0';
              printf("v=%s\n",v);
              if(strcmp(v,"1")==0)
              { if(strstr(genuri,"rock")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"rock,");
                else
                strcat(genuri,"rock,");
                else
                printf("Deja selectat\n");
              }else
              if(strcmp(v,"2")==0)
              {if(strstr(genuri,"hip-hop")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"hip-hop,");
                else
                strcat(genuri,"hip-hop,");
                else
                printf("Deja selectat\n");}else
              if(strcmp(v,"3")==0)
              {if(strstr(genuri,"dance")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"dance,");
                else
                strcat(genuri,"dance,");
                else
                printf("Deja selectat\n");}else
              if(strcmp(v,"4")==0)
              {if(strstr(genuri,"opera")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"opera,");
                else
                strcat(genuri,"opera,");
                else
                printf("Deja selectat\n");}else
              if(strcmp(v,"5")==0)
              {
                printf("gata\ndaca nu ati selectat nici un gen va intoarceti la meniu\n");
                if(strlen(genuri)!=0)
                genuri[strlen(genuri)-1]='\0';
                break;
              }
              else{printf("comanda invalida\n");}

            }
            if(strlen(genuri)==0)
            {if(scriere("inapoi",sd)==-1)
            break;
          break;}
            else
            if(scriere(genuri,sd)==-1)
            break;
            if(citire(v,sd,2)==-1)
            break;
            if(strcmp(v,"ok")==0)
            ds=3;
            else
            break;

          }
          while (ds==3) {
            printf("inapoi pentru meniu.\nScrie link-ul(maxim 300 caractere.\n)");
            bzero(link,sizeof(link));
            fgets(link,303,stdin);
            link[strlen(link)-1]='\0';
            if(scriere(link,sd)==-1)
            break;
            if(strcmp("inapoi",link)==0)
            {
              printf("revenim la meniu\n");
              if(scriere("inapoi",sd)==-1)
              exit(1);
              break;
            }
            else
            if(citire(v,sd,2)==-1)
            exit(1);
            if(strcmp("ok",v)==0)
          {
            ds=4;
            printf("ajunge\n");
            if(citire(v,sd,7)==-1)
            exit(1);
            printf("ajunge\n");
            if(strcmp(v,"melodie")==0)
            printf("melodie inserata cu succes\n");

          }
          }
        }
        else
        if(strcmp("4",msg)==0)
        {int ds=0;
          char melodie[60];
          char v[8];
          if(scriere("votare melodie",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie\n---->");
            bzero(melodie,65);
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            fflush(stdout);
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie inexistenta\n");
          }
          if(scriere("drept",sd)==-1)
          exit(1);
          if(citire(v,sd,8)==-1)
          exit(1);
          if(strcmp(v,"ok")==0)
          printf("Ati votat cu succes\n");
          else
          if(strcmp(v,"ko")==0)
          printf("Ati votat deja!\n");
          else
          printf("Nu aveti drept de vot!\n");

        }
        else
        if(strcmp("5",msg)==0)
        {
          int ds=0;
            char melodie[60];
            char v[8];
          if(scriere("scrie comentariu",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie\n---->");
            bzero(melodie,65);
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            fflush(stdout);
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie inexistenta\n");
          }
          char comentariu[305];
          while(1)
          {

            bzero(comentariu,305);
            fgets(comentariu,305,stdin);
            comentariu[strlen(comentariu)-1]='\0';
            if(strlen(comentariu)>300)
            {
              printf("Comentariu prea mare\n");
            }
            else
            {
              break;
          }


          }
          if(scriere(comentariu,sd)==-1)
          exit(1);
          if(citire(v,sd,8)==-1)
          exit(1);
          if(strcmp(v,"ok")==0)
          printf("Ati postat comentariul\n");
        }
        else
        if(strcmp("6",msg)==0)
        {
          int ds=0;
            char melodie[60];
            char v[8];
          if(scriere("vezi informatii melodie",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie\n---->");
            bzero(melodie,65);
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            fflush(stdout);
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie inexistenta\n");
          }
          if(citire(v,sd,10)==-1)
          exit(1);
          int n_r=atoi(v);
          char rez_f[n_r];
          bzero(rez_f,n_r);
          if(scriere("ok",sd)==-1)
          exit(1);
          if(citire(rez_f,sd,n_r)==-1)
          exit(1);
          printf("Informatii:\n%s\n",rez_f);
        }
        else
        printf("Comanda invalida\n");
      }

      while(ok==2)
      {
        printf("Apasati tasta pentru a utiliza o comanda!\nApasati:\n 1 pt vizualizare top\n 2 pt vizualizare top dupa gen\n 3 pt inserare melodie\n 4 votare melodie\n 5 pt comentare la o melodie\n 6 vezi comentarii la o melodie\n 7 iesire\ncomanda->");
        bzero(msg,sizeof(msg));
        fgets(msg,5,stdin);
        msg[strlen(msg)-1]='\0';
        if(strcmp(msg,"7")==0)
        {ok=-1;
          if(scriere("iesire",sd)==-1)
          {
            close(sd);
            break;
          }
        }
        else
        if(strcmp(msg,"1")==0)
        {
          if(scriere("vizualizare top",sd)==-1)
          {
            close(sd);
            ok=-1;
            break;
          }
          char nm[10];
          bzero(nm,10);
          if(citire(nm,sd,10)==-1)
          exit(1);
          if(scriere("ok",sd)==-1)
          exit(1);
          printf("nm=%s\n",nm );
          int nr=atoi(nm);
          printf("nr=%d\n",nr);
          char rezultat[nr];
          bzero(rezultat,nr);
          if(citire(rezultat,sd,nr)==-1)
          exit(1);
          printf("Top\n%s\n",rezultat);
        }
        else
        if(strcmp(msg,"2")==0)
        {
          if(scriere("vizualizare top2",sd)==-1)
          {
            close(sd);
            ok=-1;
            break;
          }
          char gen[10];
          bzero(gen,10);
          while(1)
          {
            printf("Selectati gen:\n1->rock\n2->hip-hop\n3->dance\n4->opera\n");
            bzero(msg,sizeof(msg));
            fgets(msg,5,stdin);
            msg[strlen(msg)-1]='\0';
            if(strcmp(msg,"1")==0)
            {
              strcpy(gen,"rock");
              break;
            }
            else
            if(strcmp(msg,"2")==0)
            {
              strcpy(gen,"hip-hop");
              break;
            }
            else
            if(strcmp(msg,"3")==0)
            {
              strcpy(gen,"dance");
              break;
            }
            else
            if(strcmp(msg,"4")==0)
            {
              strcpy(gen,"opera");
              break;
            }
            else
            printf("Comanda invalida\n");

          }
          if(scriere(gen,sd)==-1)
          exit(1);
          char nm[10];
          bzero(nm,10);
          if(citire(nm,sd,10)==-1)
          exit(1);
          if(scriere("ok",sd)==-1)
          exit(1);
          printf("nm=%s\n",nm );
          int nr=atoi(nm);
          printf("nr=%d\n",nr);
          if(nr==0)
          {
            printf("Nici o melodie cu acest gen\n");
            break;
          }
          else{
          char rezultat[nr];
          bzero(rezultat,nr);
          if(citire(rezultat,sd,nr)==-1)
          exit(1);
          printf("Top\n%s\n",rezultat);
        }
        }
        else
        if(strcmp(msg,"3")==0)
        {
          char melodie[65];
          char descriere[505];
          char genuri[300];
          char link[303];
          int ds=0;
          char v[8];
          if(scriere("inserare_melodie",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie(maxim 60 caractere si nu trebuie sa fie gol:\nscrie inapoi sa te intorci la meniul utilizatorului\n--->)");
            bzero(melodie,sizeof(melodie));
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(strlen(melodie)==0||strlen(melodie)>60)
            printf("Numar prea mare de caractere sau spatiu gol\n");
            else
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie existenta deja\n");
          }
          while(ds==1)
          {
              printf("inapoi pt meniu.\nScrie descrierea(maxim 500 caractere.\n)");
              bzero(descriere,sizeof(descriere));
              fgets(descriere,505,stdin);
              descriere[strlen(descriere)-1]='\0';
              if(strlen(descriere)>500)
              printf("Numar prea mare de caractere!\n" );
              else
              if(strcmp(descriere,"inapoi")==0)
              {
                if(scriere(descriere,sd)==-1)
                exit(1);
                break;
              }else
              if(scriere(descriere,sd)==-1)
              break;
              if(citire(v,sd,8)==-1)
              break;
              if(strcmp(v,"ok")==0)
              ds=2;
              else
              printf("S-a pierdut ceva la transport\n");

          }
          while(ds==2)
          {
            printf("inapoi pt meniu.\n");
            int k=1;
            char v[8];
            bzero(v,sizeof(v));
            bzero(genuri,sizeof(genuri));
            while(1)
            {
              printf("Genuri selectate:%s\n",genuri);
              printf("apasati:\n1 pentru rock\n2 pentru hip-hop\n3 pentru dance \n4 pentru opera\n5 gata \n----->");
              bzero(v,sizeof(v));
              fgets(v,8,stdin);
              v[strlen(v)-1]='\0';
              printf("v=%s\n",v);
              if(strcmp(v,"1")==0)
              { if(strstr(genuri,"rock")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"rock,");
                else
                strcat(genuri,"rock,");
                else
                printf("Deja selectat\n");
              }else
              if(strcmp(v,"2")==0)
              {if(strstr(genuri,"hip-hop")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"hip-hop,");
                else
                strcat(genuri,"hip-hop,");
                else
                printf("Deja selectat\n");}else
              if(strcmp(v,"3")==0)
              {if(strstr(genuri,"dance")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"dance,");
                else
                strcat(genuri,"dance,");
                else
                printf("Deja selectat\n");}else
              if(strcmp(v,"4")==0)
              {if(strstr(genuri,"opera")==NULL)
                if(strlen(genuri)==0)
                strcpy(genuri,"opera,");
                else
                strcat(genuri,"opera,");
                else
                printf("Deja selectat\n");}else
              if(strcmp(v,"5")==0)
              {
                printf("gata\ndaca nu ati selectat nici un gen va intoarceti la meniu\n");
                if(strlen(genuri)!=0)
                genuri[strlen(genuri)-1]='\0';
                break;
              }
              else{printf("comanda invalida\n");}

            }
            if(strlen(genuri)==0)
            {if(scriere("inapoi",sd)==-1)
            break;
          break;}
            else
            if(scriere(genuri,sd)==-1)
            break;
            if(citire(v,sd,2)==-1)
            break;
            if(strcmp(v,"ok")==0)
            ds=3;
            else
            break;

          }
          while (ds==3) {
            printf("inapoi pentru meniu.\nScrie link-ul(maxim 300 caractere.\n)");
            bzero(link,sizeof(link));
            fgets(link,303,stdin);
            link[strlen(link)-1]='\0';
            if(scriere(link,sd)==-1)
            break;
            if(strcmp("inapoi",link)==0)
            {
              printf("revenim la meniu\n");
              if(scriere("inapoi",sd)==-1)
              exit(1);
              break;
            }
            else
            if(citire(v,sd,2)==-1)
            exit(1);
            if(strcmp("ok",v)==0)
          {
            ds=4;
            printf("ajunge\n");
            if(citire(v,sd,7)==-1)
            exit(1);
            printf("ajunge\n");
            if(strcmp(v,"melodie")==0)
            printf("melodie inserata cu succes\n");

          }
          }
        }
        else
        if(strcmp("4",msg)==0)
        {int ds=0;
          char melodie[60];
          char v[8];
          if(scriere("votare melodie",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie\n---->");
            bzero(melodie,65);
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            fflush(stdout);
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie inexistenta\n");
          }
          if(scriere("drept",sd)==-1)
          exit(1);
          if(citire(v,sd,8)==-1)
          exit(1);
          if(strcmp(v,"ok")==0)
          printf("Ati votat cu succes\n");
          else
          if(strcmp(v,"ko")==0)
          printf("Ati votat deja!\n");
          else
          printf("Nu aveti drept de vot!\n");

        }
        else
        if(strcmp("5",msg)==0)
        {
          int ds=0;
            char melodie[60];
            char v[8];
          if(scriere("scrie comentariu",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie\n---->");
            bzero(melodie,65);
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            fflush(stdout);
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie inexistenta\n");
          }
          char comentariu[305];
          while(1)
          {

            bzero(comentariu,305);
            fgets(comentariu,305,stdin);
            comentariu[strlen(comentariu)-1]='\0';
            if(strlen(comentariu)>300)
            {
              printf("Comentariu prea mare\n");
            }
            else
            {
              break;
          }


          }
          if(scriere(comentariu,sd)==-1)
          exit(1);
          if(citire(v,sd,8)==-1)
          exit(1);
          if(strcmp(v,"ok")==0)
          printf("Ati postat comentariul\n");
        }
        else
        if(strcmp("6",msg)==0)
        {
          int ds=0;
            char melodie[60];
            char v[8];
          if(scriere("vezi informatii melodie",sd)==-1)
          {
            close(sd);
            break;
          }
          while(ds==0)
          {
            printf("Nume Melodie\n---->");
            bzero(melodie,65);
            fgets(melodie,65,stdin);
            melodie[strlen(melodie)-1]='\0';
            if(scriere(melodie,sd)==-1)
            {
              close(sd);
              break;
            }
            if(strcmp("inapoi",melodie)==0)
            break;
            if(citire(v,sd,sizeof(v))==-1)
            break;
            fflush(stdout);
            if(strcmp(v,"ok")==0)
            {ds=1;
             printf("Numele melodiei este valid\n");
            }
            else
            printf("Melodie inexistenta\n");
          }
          if(citire(v,sd,10)==-1)
          exit(1);
          int n_r=atoi(v);
          char rez_f[n_r];
          bzero(rez_f,n_r);
          if(scriere("ok",sd)==-1)
          exit(1);
          if(citire(rez_f,sd,n_r)==-1)
          exit(1);
          printf("Informatii:\n%s\n",rez_f);
        }
        else
        printf("Comanda invalida\n");
      }
}
  close (sd);
  return 0;
}
int scriere(char* mesaj,int sd)
{
	if (write (sd, mesaj, strlen(mesaj)) <= 0)
    	{
      	perror ("[client]Eroare la write() spre server.\n");
      	return -1;
    	}
      printf("S-a trimis:    %s\n",mesaj);
      return 0;
}
int citire(char* mesaj,int sd,int marime)
{
  bzero(mesaj,marime);
  printf("marime %d\n",marime);
	if (read (sd, mesaj, marime) <= 0)
    	{
      	perror ("[client]Eroare la read() de la server.\n");
      	return -1;
    	}
    return 0;
}
int logare(char* nume,char* parola,int sd)
{

  char v[8];
  bzero(nume,65);
  printf("nume: \n");
  fgets(nume,60,stdin);
  nume[strlen(nume)-1]='\0';
  scriere(nume,sd);
  bzero(parola,65);
  printf("parola\n");
  fgets(parola,60,stdin);
  parola[strlen(parola)-1]='\0';
  scriere(parola,sd);
  citire(v,sd,8);
  printf("Tip: %s\n",v);
  if(strcmp(v,"admin")==0)
  return 1;
  if(strcmp(v,"obisnuit")==0)
  return 2;
  return 0;
}
int inscriere(char* nume,char*parola,int sd)
{
  char v[8];
  int d=0;
  while(1)
  {
    printf("introdu numele care va fi de maxim 60 de caractere.\nDaca scrii inaopi te intorci la meniu.\n");
    while(d==0)
    {
      printf("---->");
      bzero(nume,65);
      fgets(nume,60,stdin);
      nume[strlen(nume)-1]='\0';
      if(strlen(nume)>60||strlen(nume)==0)
      printf("Numar prea mare de caractere in nume sau blank\n.Scrie inapoi pentru a te intoarce la meniu sau introdu un alt nume.\n");
      else
      if(strcmp(nume,"inapoi")==0)
      {
        scriere("inapoi",sd);
        return 0;
      }
      else
      {
        scriere(nume,sd);
        bzero(v,8);
        citire(v,sd,8);
        if(strcmp("ok",v)==0)
        d=1;
        else
        printf("Utilizator existent.\n");
      }

    }
    while(d==1)
    {
      printf("Introdu parola de maxim 60 de caractere.\nDaca scrii inapoi te intorci la meniu.\n");
      bzero(parola,65);
      fgets(parola,60,stdin);
      parola[strlen(parola)-1]='\0';
      if(strlen(parola)>60||strlen(parola)==0)
      printf("Parola prea mare sau blank.\n");
      else
      if(strcmp(parola,"inapoi")==0)
      {
        scriere("inapoi",sd);
        return 0;}
      else
      {
        scriere(parola,sd);
        bzero(v,8);
        citire(v,sd,8);
        if(strcmp(v,"ok2")==0)
        return 2;
        else
        printf("Ceva nu e i regula.\n");
      }

    }

  }
}
