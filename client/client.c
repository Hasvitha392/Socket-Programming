#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SA struct sockaddr 
#define ll long long int
#define PORT 8000
#define RED  "\x1b[1;31m"
#define GREEN  "\x1b[1;32m"
#define BLUE "\x1b[1;34m"
#define CYAN  "\x1b[1;36m"
#define YELLOW  "\x1b[1;33m"
#define MAG  "\x1b[1;35m"
#define RESET  "\x1b[0m"


void recvfunc(int sockfd)
{
    size_t siz2=100;
    char file[100]={0};
    size_t siz=1024;
    int flag,n,k;
    char *chunk[100],*chunk2[100];
    char *token = (char*)malloc(siz2*sizeof(char));
    char *buff = (char*)malloc(siz*sizeof(char));
    //char *token2 = (char*)malloc(siz2*sizeof(char));
    flag=1;k=0;
    while(flag)
    {
      printf(CYAN"client>"RESET);
      //free(token);
      //free(token2);
      //free(buff);
      memset(buff,0,siz);
      memset(token,0,siz2);
      getline(&token,&siz,stdin);
      int c=0;
      char *separate = strtok(token," \n");
      while(separate!=NULL)
      {
        chunk[c]=strdup(separate);
        separate = strtok(NULL," \n");
        c++;
      }
      chunk[c]=NULL;
      n=c;
      if(!(strcmp(chunk[0],"exit")))
      {
        flag=0;
        send(sockfd,file,100,0);
        printf("%sExited from the client\n",MAG);
        continue;
      }
      if(strcmp(chunk[0],"get")!=0)
      {
        printf("%sInvalid input\n",RED);
        continue;
      }
      n--;
      int i=0;
      int k=0;
      int c1=0;
      int cur=0;int total=0;
      for(i=0;i<n;i++)
      {
        //int total=0;
        memset(file,0,siz2);
        strncpy(file,chunk[i+1],strlen(chunk[i+1]));
       // int sender = send(sockfd,file,100,0);
        if((send(sockfd,file,100,0))==-1)
        {
          printf("%sError in sending\n",RED);
          exit(1);
        }
        recv(sockfd,file,100,0);
        int c1=0;
        char *separate1 = strtok(file," \n");
        while(separate1!=NULL)
        {
          chunk2[c1]=strdup(separate1);
          separate1 = strtok(NULL," \n");
          c1++;
        }
        chunk2[c1]=NULL;
        k=c1;
        if(strcmp(chunk2[0],"not")==0)
        {
          printf("\n%sFile-%s doesn't exist/not readable on server side\n \n",YELLOW,chunk[i+1]);
          continue;
        }
        if(strcmp(chunk2[0],"ok")==0)
        {
          int p=0;
          printf("%sFile-%s exists on server side\n",GREEN,chunk[i+1]);
          FILE *fd2=fopen(chunk[i+1],"w");
          //printf("YES1\n");
          //int cur=0;
          cur=0;total=atoi(chunk2[1]);
          while((p=recv(sockfd,buff,siz,0))>0)
          {
            // printf("YES2\n");
            cur=cur+p;
            memset(file,0,siz2);
            // printf("YES3\n");
             //printf("YES4\n");
             //printf("YES5\n");
            if(fwrite(buff,sizeof(char),p,fd2)!=p)
            {
              printf("%sError in writing the file contents\n",RED);
              exit(1);
            }
            long double percentage = ((long double)(cur) / (long double) total) * 100;
            // printf("YES6\n");
            sprintf(file,"\r%sPercentage of file downloaded is %.2Lf",BLUE,percentage);
           // printf("YES7\n");
            write(1,file,strlen(file));
            if(percentage==100.00) break;
            if(p!=siz) break;
            if(p<=0) break;
          }
          fclose(fd2);
          printf("\n");
        }
        else
        {
          printf("%sError in checking of file-%s \n",RED,chunk[i+1]);
          continue;
        }
        memset(file,0,siz2);

      }
    }

}

int main() 
{ 
  	int sockfd;
  	struct sockaddr_in serveraddr, cli; 

  	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  	if (sockfd == -1) { 
  		printf("socket creation failed...\n"); 
  		exit(0); 
  	} 
  
    memset(&serveraddr, '0', sizeof(serveraddr));
  	serveraddr.sin_family = AF_INET; 
  	
    if(inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    serveraddr.sin_port = htons(PORT); 

  	if (connect(sockfd, (SA*)&serveraddr, sizeof(serveraddr)) != 0) { 
  		printf("connection with the server failed...\n"); 
  		exit(0); 
  	} 

    recvfunc(sockfd);

    return 0;
} 
