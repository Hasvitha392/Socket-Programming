#include <stdio.h>
#include <sys/socket.h>
#include<sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include<stdbool.h>
#define SA struct sockaddr 
#define PORT 8000
#define MAG  "\x1b[1;35m"


void sendfunc(int sockfd)
{
	char file[105]={0};
	size_t siz=1024;
	//char *token = (char*)malloc(10*sizeof(char));
	char *buff = (char *)malloc(siz*sizeof(char));
	int a,flag;
	flag=1;
	//recv(sockfd,token,sizeof(token),0);
	while(flag)
	{
		//char file[105]={0};
		a=recv(sockfd,file,100,0);

		//printf("%s\n",file);
		//char filedup[105];
		//strcpy(filedup,file);
		if(a==-1)
		{
			//printf("Error in receiving file name\n");
			exit(1);
		}
		if(strcmp(file,"exit")==0)
		{
			printf("%sExited from the server\n",MAG);
			flag=0;
			continue;
		}
		//printf("%s",filedup);
		int p=0;
		FILE *fd = fopen(file,"r");
		//memset(filedup,0,100);
		int checker = access(file,R_OK);
		memset(file,0,100);
		if(fd==NULL)
		{
			//printf("YES\n");
			sprintf(file,"not");
			send(sockfd,file,100,0);
			memset(file,0,100);
			continue;
		}
			if(checker==-1)
            {  
                sprintf(file,"not");
                send(sockfd,file,100,0);
                memset(file,0,100);
                continue;

            }
		fseek(fd,0,SEEK_END);
		long long int fdsize=ftell(fd);
		rewind(fd);
		sprintf(file,"ok %lld",fdsize);
		send(sockfd,file,100,0);
		while( (p=fread(buff,sizeof(char),siz,fd))  > 0)
        { 
        	int check=send(sockfd,buff,p,0);

            memset(buff,0,siz);
             if(check ==-1)
            {
             	perror("Error reg sending");
             	exit(1);
            }
                        
        }       
        fclose(fd);
        memset(file,0,100);
	}

}

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in serveraddr,cli; 

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	
	serveraddr.sin_family = AF_INET; 
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	serveraddr.sin_port = htons(PORT); 

	if ((bind(sockfd, (SA*)&serveraddr, sizeof(serveraddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 

	len = sizeof(cli); 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	
	sendfunc(connfd);
	close(connfd); 

	
} 
