#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>


void converse (int sock) 
{
   int n;
   char msg[256];
 
   while(1)
   { 
     bzero(msg,256);
     n = read(sock,msg,255);
   
     if (n < 0) 
     {
      perror("ERROR reading from socket");
      exit(1);
     }
   
     printf("%s",msg);  //message from the client being displayed on server's terminal
     scanf("%s",msg);   //server's reply being taken
     n = write(sock,msg,18);
   
     if (n < 0) 
     {
      perror("ERROR in writing to socket");
      exit(1);
     }
   }
}

int main()   
{
   int sockfd, new_sockfd, portno, clientlen;
   char buffer[256];
   struct sockaddr_in server_addr, client_addr;
   int n, proc_id;
   
   //1. create a socket 
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   //2. Initialize socket 
   bzero((char *) &server_addr, sizeof(server_addr));
   portno = 5050;  //assuming portno=5050
   
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = INADDR_ANY;
   server_addr.sin_port = htons(portno);
   
   // 3. bind 
   if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
      perror("ERROR on binding");
      exit(1);
   }
   
   /* start listening for the clients, here
      process will go in sleep mode and will wait
      for the incoming connection
   */
   
   listen(sockfd,5);
   clientlen = sizeof(client_addr);
   
   while (1) 
   {
      new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clientlen);
		
      if (new_sockfd < 0) 
      {
         perror("ERROR in accepting");
         exit(1);
      }
      printf("connection accepted\n");
     
     // Create child process to handle another client
      proc_id = fork();
		
      if (proc_id < 0) 
      {  perror("ERROR in creating a new process");
         exit(1);
      }
      
      if (proc_id == 0) 
      {
         //client process
         close(sockfd);
         converse(new_sockfd);
         exit(0);
      }
      else
     {
      //parent process
        close(new_sockfd);
     }
		
   } 
}



