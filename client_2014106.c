#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[])    //argv[1] = server's address , argv[2]=portno  e.g ./a.out 192.168.53.143 5050 
{
   int sockfd, portno, n;
   struct sockaddr_in server_addr;
   struct hostent *server;
   
   char msg[256];
   
   if (argc < 3) 
   {
      fprintf(stderr,"Please write the command as: %s hostname port\n", argv[0]);
      exit(0);
   }
	
   portno = atoi(argv[2]);  //to convert to integer
   
   // Create a socket 
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR in opening socket");
      exit(1);
   }
	
   server = gethostbyname(argv[1]);   // to convert ip address of server to its domain name
   
   if (server == NULL) 
   {
      fprintf(stderr,"No such host, Exiting....\n");
      exit(0);
   }
   
   bzero((char *) &server_addr, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
   server_addr.sin_port = htons(portno);
   
   /* connecting to server */
   if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
      perror("ERROR in connecting to the server");
      exit(1);
   }
   
   
  while(1)	
 {  //printf("Please enter the message: ");
   bzero(msg,256);
   fgets(msg,255,stdin);
   
   //Sending message to server 
   n = write(sockfd, msg, strlen(msg));
   
   if (n < 0) 
   {
      perror("ERROR in writing to socket");
      exit(1);
   }
   
   /* reading server's reply */
   bzero(msg,256);
   n = read(sockfd, msg, 255);
   
   if (n < 0) 
   {
      perror("Error in reading from socket");
      exit(1);
   }
	
   printf("%s\n",msg);   // displaying server's message on client's screen
 }
   return 0;
}
