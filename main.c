#include <stdio.h>

/*******************************************************************************

  Ein TCP-Echo-Server als iterativer Server: Der Server schickt einfach die
  Daten, die der Client schickt, an den Client zurück.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFSIZE 1024 // Größe des Buffers
#define TRUE 1
#define ENDLOSSCHLEIFE 1
#define PORT 5678


int main() {

  int rfd; // Rendevouz-Descriptor
  int cfd; // Verbindungs-Descriptor

  struct sockaddr_in client; // Socketadresse eines Clients
  socklen_t client_len; // Länge der Client-Daten
  char in[BUFSIZE]; // Daten vom Client an den Server
  int bytes_read; // Anzahl der Bytes, die der Client geschickt hat


  // Socket erstellen
  rfd = socket(AF_INET, SOCK_STREAM, 0);
  if (rfd < 0 ){
    fprintf(stderr, "socket konnte nicht erstellt werden\n");
    exit(-1);
  }


  // Socket Optionen setzen für schnelles wiederholtes Binden der Adresse
  int option = 1;
  setsockopt(rfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &option, sizeof(int));


  // Socket binden
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(PORT);
  int brt = bind(rfd, (struct sockaddr *) &server, sizeof(server));
  if (brt < 0 ){
    fprintf(stderr, "socket konnte nicht gebunden werden\n");
    exit(-1);
  }


  // Socket lauschen lassen
  int lrt = listen(rfd, 5);
  if (lrt < 0 ){
    fprintf(stderr, "socket konnte nicht listen gesetzt werden\n");
    exit(-1);
  }

  while (ENDLOSSCHLEIFE) {

    char keys[20][20];
    char values[20][20];


    // Verbindung eines Clients wird entgegengenommen
    cfd = accept(rfd, (struct sockaddr *) 0, 0);

    int pid = fork();

    if (pid==0) {
      close(rfd);
      printf("test");

      // Lesen von Daten, die der Client schickt
      bytes_read = read(cfd, in, BUFSIZE);

      // Zurückschicken der Daten, solange der Client welche schickt (und kein Fehler passiert)
      while (bytes_read > 0) {
        printf("sending back the %d bytes I received...\n", bytes_read);
        char command[16], key[20], value[20];                                        //command key und value platzhalter für eingabe
        int n = sscanf(in, "%15s %19s %19[^\n]", command, key, value);  //teilt das gelesen in command, key und value auf

        if (strcmp(command, "put" )==0) {
          for (int i =0; i<20;i++) {
            if (strcmp(keys[i], key)==0 || keys[i][0]=='\0') {
              for (int j = 0; j<20; j++) {
                keys[i][j]=key[j];
                values[i][j]=value[j];
              }
              break;
            }
          }

        }
        if (strcmp(command, "get")==0) {
          for (int i =0; i<20;i++) {
            if (strcmp(keys[i], key)==0) {
              snprintf(in, BUFSIZE, "%s:%s\n", key, values[i]);
              write(cfd, in, strlen(in));
              break;
            }
          }
        }
        if (strcmp(command, "del")==0){
            for (int i =0; i<20;i++) {
              if (strcmp(keys[i], key)==0) {
                strcpy(keys[i], "");
                break;
              }
            }
        }
        if (strcmp(command, "quit")==0) exit(0);

        bytes_read = read(cfd, in, BUFSIZE);

      }
      printf("oh nein");
      close(cfd);
      exit(0);
    }
    if (pid>0) close(cfd);

  }

  // Rendevouz Descriptor schließen
  close(rfd);

}