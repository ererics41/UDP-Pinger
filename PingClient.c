#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

void error(const char *);
int main(int argc, char *argv[]){
  struct timeval timeout={1,0};
  struct timeval current;
  int sock;
  unsigned int length;
  struct sockaddr_in server, from;
  struct hostent *hp;
  char buffer[256];

  //check to ensure the right number of argvs were passed in
  if (argc != 3) { 
    printf("Usage: server port\n");
    exit(1);
  }

  //create a socket, use SOCK_DRAM to indicate use of udp then perform basic error checking
  sock= socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) 
    error("socket");

  //set the family of the server address(server) to match the socket
  server.sin_family = AF_INET;

  //get the host ip address from argv
  hp = gethostbyname(argv[1]);
  if (hp==0) 
    error("Unknown host");

  //copy the host ip address into the server address(server)
  bcopy((char *)hp->h_addr, (char *)&server.sin_addr, hp->h_length);

  //set the server address(server)'s port number to the port number from argv
  server.sin_port = htons(atoi(argv[2]));

  length=sizeof(struct sockaddr_in);

  int n;
  double aver = 0;
  double max = 0;
  double min = 0;
  int numReceived = 0;
  char *seqNum;
  char *start;
  double startD;
  double time;

  hp = gethostbyname(argv[1]);

  for(int i = 0; i < 10; i++){
    //get current time and format buffer
    gettimeofday(&current, NULL);
    time = (double)(current.tv_usec)/1000000 + (double)current.tv_sec;
    sprintf(buffer, "PING %d %f", i, time);
    
    //send packet
    n=sendto(sock,buffer, strlen(buffer), 0, (const struct sockaddr *)&server, length);
    if (n < 0) 
      error("Sendto");
    bzero(buffer,256);
    
    //set the timer
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));

    /* Receive UDP message */
    n = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &length);
    if (n >= 0) {
      //Message Received
      
      //get current time
      gettimeofday(&current, NULL);
      
      //get sequence number
      seqNum = strtok(buffer, " ");
      seqNum = strtok (NULL, " ");
      
      //calculate rtt
      start = strtok(NULL, " ");
      sscanf(start, "%lf", &startD);
      time = 1000*((double)(current.tv_usec)/1000000 + (double)current.tv_sec - startD);
      numReceived++;
      printf("PING received from %s: seq#=%s time=%fms\n",hp->h_name, seqNum, time);

      //update average, min and max
      aver = aver + time;
      if(time > max){
        max = time;
      }
      if(min == 0){
        min = time;
      }
      if(min > time){
        min = time;
      }
    }
    else{
      //Message Receive Timeout 
      printf("Request Timeout\n");
    }
    //clear the buffer and wait 1 second
    bzero(buffer,256);
    sleep(1);
  }
  if(numReceived>0){
    aver = aver/numReceived;
  }
  printf("-- ping statistics --- 10 packets transmitted, %d received, %d%% packet loss rtt min/avg/max = %f %f %f ms"
    , numReceived, (10-numReceived)*10, min, aver, max);

  //close the socket
  close(sock);
  return 0;
}

void error(const char *msg)
{
  perror(msg);
  exit(0);
}
