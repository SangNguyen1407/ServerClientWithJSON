#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "../../../inc/json_inc.h"
#include "../../../inc/serverClient_inc.h"

#define handle_error(msg)\
			do{perror(msg); exit(EXIT_FAILURE); }while(0)
			
/* client.c
 */	
int sockClient;
int closeConn = 0;

int readMsg (char *msgContent)
{
	if (msgContent)
	{
		JSON_CONTENT json_buff;
		json_buff = readFile("json.txt", READ_ONLY_MODE);
		printf ("\n%s\n", json_buff.buffer);
		strncpy(msgContent, json_buff.buffer, strlen(json_buff.buffer));
	}
	return 3;
}

int checkMsgCodeNumber (char *msgContent)
{
	if (msgContent)
	{
		printf ("\n%s\n", msgContent);
		char *codeNumber = strtok(msgContent, " ");
		if (strncmp(codeNumber, "/exit", 5) == 0)
		{
			return 2;
		}
		else
		{
			JSON_CONTENT json_buff;
			json_buff = readFile("json.txt", READ_ONLY_MODE);
			printf ("\n%s\n", json_buff.buffer);
			return 4;
		}
	}
	
	return 0;
}

void *recvMessage(int *resultId)
{
	while(1)
	{
		char buffer[1024];
		char YNQuestion[10];
		int msgLen = recv(sockClient, buffer, MAXMSGLEN, 0);
		if(!msgLen) 
		{
			printf("Server disconnect...\n");
			closeConn = 1;
			pthread_exit(0);
			return;
		}
		if(strncmp(buffer, "/exit", 5) == 0)
		{
			pthread_exit(0);
			return;
		}

		printf("%s", buffer); /*normal message chat*/
	}
}

int main (int argc, char * argv[])
{
	int lenMsg = 0, *resultId;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	char buffer[1024];
	char srvIpAddr[20];
	struct in_addr addr;
	struct ThreadClientInfo *threadClient;
	
	sockClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sockClient == -1)
		handle_error("sock");
	resultId = (int *) malloc(sizeof(int));
	memset (&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	if (argv[2] == NULL)
	{
		printf ("Please enter IP Server...\n");
		return 0;
	}
	strcpy(srvIpAddr, argv[2]);
	serverAddr.sin_addr.s_addr = inet_addr(srvIpAddr); //argv[2] IP address
	serverAddr.sin_port = htons(PORT);
	if (connect(sockClient, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
	{
		handle_error("connect");	
	}
	/*Code for starting chatting*/		
	printf ("Start chatting....\n");
	char *msgSend = (char*) malloc(sizeof(char)*MAXMSGLEN);
	/*Send user name to Server*/
	strcpy(msgSend, "login "); /*code 1 for username*/
	if (argv[1] == NULL || argv[1] == "")
	{
		printf("No Username\n");
		close(sockClient);
		return 0;
	}
	
	strcat(msgSend, argv[1]);  /* msg send Server is 'login <username>' for example
	                                'login test' */
	printf("Send username to Server: %s\n",msgSend);
	send(sockClient, msgSend, 200, 0);
	recv(sockClient, buffer, MAXMSGLEN, 0);
	printf("%s", buffer);
	/*Send and recive 'command line' to Server
		1- exit
		2- List of User is online.
	 */
	printf("------Content message--------\n");
	threadClient = (struct ThreadClientInfo *)malloc(sizeof(struct ThreadClientInfo));
	pthread_create (&threadClient->threadClientID, NULL, 
        		(void *) &recvMessage, (int*) &resultId);
    
	while(1)
	{
	    char commandline[1024];
		char *readFile;
	    fgets(commandline, sizeof(commandline), stdin);
		
		readFile = (char *) malloc (sizeof(char) * 2048);
		memset (readFile, '\0', 2048);
		int i = readMsg (readFile);
	//	printf("Content:\n %s\n", readFile );
	    
		send(sockClient, readFile, 200, 0);
		if ( i == 2)
		{
			close(sockClient);
			printf("Closed\n");
			break;
		}
		else
		{
			int msgLen = recv(sockClient, buffer, MAXMSGLEN, 0);
			printf("Send OK:\n%s\n",buffer);
		}
		
	}	
	return 0;
}