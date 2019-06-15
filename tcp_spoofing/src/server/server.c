#include <arpa/inet.h>
#include <unistd.h> 
#include "../../../inc/json_inc.h"
#include "../../../inc/serverClient_inc.h"

#define PORT			12345
#define MAXMSGLEN		1024
#define INET_ADDRSTRLEN 16
#define LISTEN_BACKLOG 	50
#define handle_error(msg)\
		do{ perror(msg); exit(EXIT_FAILURE); } while(0)


/************************************************************/
struct ListClientThread *ll = NULL;

/************************************************************/


int compare(struct ThreadClientInfo *lthread, struct ThreadClientInfo *thread)
{
	return(lthread->sockfd - thread->sockfd);
}

char *getIPfromSockAddress (struct sockaddr_in client_addr)
{
	int ipAddr = client_addr.sin_addr.s_addr;
	char *strIpAddr = (char *)malloc(sizeof(char) * INET_ADDRSTRLEN);
	inet_ntop( AF_INET, &ipAddr, strIpAddr, INET_ADDRSTRLEN );
	
	return strIpAddr;
}

void listInit(struct ListClientThread *ll)
{
	ll->head = ll->tail = NULL;
	ll->size =0;
}

int searchSocketUser (struct ListNode *listNode, char *userName)
{
	if(ll->head == NULL) return 0;
	struct ListNode *temp = listNode;
	
	while(temp)
	{
		if ( strcmp(temp->threadClient->clientName, userName) == 0)
		{
			return temp->threadClient->sockfd;
		}
		temp = temp->next;
	}
	return 0;
}

int listInsert(struct ListClientThread **ll, struct ThreadClientInfo *listNode)
{
	if ((*ll)->head == NULL)
	{
		(*ll)->head = (struct ListNode *)malloc(sizeof(struct ListClientThread *));
		(*ll)->head->threadClient = listNode;
		(*ll)->head->next = NULL;
		(*ll)->tail = (*ll)->head;
	}
	else 
	{
		(*ll)->tail->next = (struct ListNode *)malloc(sizeof(struct ListClientThread));
		(*ll)->tail = (*ll)->tail->next;
		(*ll)->tail->threadClient = listNode;
		(*ll)->tail->next = NULL;
	}
	(*ll)->size ++;
	return ((*ll)->size);
}

int listDelete(struct ListClientThread **ll, struct ThreadClientInfo *listNode)
{
	struct ListNode *temp, *head;
	if ((*ll) == NULL || (*ll)->head == NULL || (*ll)->size == 0) return -1;
	head = (*ll)->head;
	while ((*ll)->head)
	{
		if (compare((*ll)->head->threadClient, listNode) == 0)
		{
			temp = (*ll)->head;
			/*1 user in list*/
			if ((*ll)->head->next == NULL || (*ll)->size == 1)
			{
				(*ll)->tail = (*ll)->head;
				(*ll)->head = NULL;
				(*ll)->size--;
				return 1;
			}
			
			/*2 or more user in list*/
			(*ll)->head = (*ll)->head->next;
			if ((*ll)->head->next == NULL)
			{
				(*ll)->tail = (*ll)->head;
			}
			
			free(temp);
			(*ll)->size--;
			return 1;
		}
		(*ll)->head = (*ll)->head->next;
	}

	return 0;
}


/*List User Online*/
char **listUsrOnline(struct ListClientThread *ll)
{
	if (ll == NULL || ll->head == NULL || ll->size == 0) return;
	struct ListNode *temp = ll->head;
	int count=0;
	int size = ll->size;
	char **listUserOnline;
	printf("%d\n",(ll->size) );
	*listUserOnline = (char*) malloc(sizeof(char *) *ll->size);
	printf("%d\n",(ll->size) );
	while(count < size && temp != NULL)
	{
		listUserOnline[count] = temp->threadClient->clientName;
		printf("%s", temp->threadClient->clientName);
		temp = temp->next;
		count++;
	}
	return listUserOnline;
}


int codeOnMessage(char *clientMessage, char *contentMsg)
{
	char climsg[MAXMSGLEN];
	char *clicontent = (char *)malloc(MAXMSGLEN * sizeof (char));
	memset (climsg, '\0', MAXMSGLEN);
	memset (clicontent, '\0', MAXMSGLEN);

	strncpy(climsg, clientMessage, strlen(clientMessage));	
	if (climsg != NULL)
	{
		char *codeNumber = strtok(climsg, " ");
		clicontent = strtok(NULL, "\0");
		if (clicontent != NULL)
		{
			strncpy(contentMsg, clicontent, strlen(clicontent));
		}
		if (strcmp(codeNumber, "login") == 0)
		{ 
			return 1;/*login with name*/
		}
		else if (strncmp(codeNumber, "/exit", 5) == 0)
		{ 
			return 2;/*interrupt connection*/
		}
		else{
			
		}

	}
	return 4;
}

char *addUserInList(int sockfd)
{
	struct ThreadClientInfo *threadClientInfo = NULL;
	char buffer[MAXMSGLEN];
	int code = 0;
	char *contentMsg = (char *) malloc(MAXMSGLEN * sizeof(char));
	threadClientInfo = (struct ThreadClientInfo *)malloc(sizeof(struct ThreadClientInfo *));
	recv(sockfd, buffer, sizeof(char)*MAXMSGLEN, 0);
    code = codeOnMessage(buffer, contentMsg);
    if (code == 1)
    {
		printf ("%s connect successfully...\n", buffer);
		strcpy(contentMsg, "test");
		threadClientInfo->sockfd = sockfd;
		if (contentMsg)
		{	
			strncpy(threadClientInfo->clientName, contentMsg, strlen(contentMsg));
		}
		else
		{
			strcpy(threadClientInfo->clientName, "Anomymous");
		}
    	send(threadClientInfo->sockfd, "Connected...\n", MAXMSGLEN, 0);
    	listInsert(&ll, threadClientInfo);
    }
    return threadClientInfo->clientName;
}

void closeConnection(struct ThreadClientInfo *threadClientInfo)
{
	/* '<username> disconnect to server...'*/
		listDelete(&ll,threadClientInfo);
	    printf("%s disconnect successfully.\n", threadClientInfo->clientName);
	    close(threadClientInfo->sockfd);
}

int json_changeValue(JSON_OBJECT *json_object, char *key, char *new_value){
	
	int json_num = 0;
	if (key == NULL || new_value == NULL){
		return 0;
	}
	
	for (; json_num < 10; json_num ++){
		if (strncmp(json_object[json_num].key, key, strlen(key)) == 0){
			strcpy(json_object[json_num].value, new_value);
		}
	}

	return 1;
}

void changeMsgContent (char *msgChat)
{
	JSON_CONTENT json_buff;
	JSON_CONTENT json_buff_temp;
	JSON_OBJECT_CONTENT json_object;
	
	if (json_buff.buffer == NULL)
	{
		json_buff.buffer = (char *) malloc(sizeof(char) * strlen(msgChat) +1 );
	}
	for (int i=0; i<strlen(msgChat); i++){
		json_buff.buffer[i] = msgChat[i];
	}
	json_buff.buffer[strlen(msgChat) +1] = '\0';
	json_buff.size = strlen(msgChat);
	json_object.size = json_value_size(json_buff);
	json_object.json_obj = (JSON_OBJECT *)malloc(sizeof(JSON_OBJECT) * json_object.size);
	int num = json_parse(json_buff, &json_object);
	json_changeValue(json_object.json_obj, "age", "50");
	j2str(json_object, &json_buff_temp);
	memset(msgChat, '\0', json_buff.size);
	for (int i=0; i<json_buff.size; i++){
		msgChat[i] = json_buff_temp.buffer[i];
	}
}



void sendMessageChat(struct ListNode *node, char *clientName, char *msgChat)
{
	char *contentMsg = (char*)malloc(sizeof(char*) * MAXMSGLEN);
	if (msgChat == NULL || msgChat == "") return;
	/*[name1]: message content*/
	char msgContent[MAXMSGLEN] ;
	// strcpy(msgContent, "[");
	// strcat(msgContent, clientName);
	// strcat(msgContent, "]:");
	changeMsgContent(msgChat );
	strcat(msgContent, msgChat);
	while(node != NULL)
	{
		if (strcmp(clientName, node->threadClient->clientName) == 0)
		{ 
    		/*send message to client*/
    		send((int)node->threadClient->sockfd, msgContent, MAXMSGLEN, 0);
		}
		node = node->next;
	}
}

void *clientHandler(struct ThreadClientInfo *threadClientInfo)
{
    char buffer[MAXMSGLEN];
    char *contentMsg = (char*)malloc(sizeof(char*) * MAXMSGLEN);
    int code = 0;

    /*check userName from Client message*/
    int sockfd = threadClientInfo->sockfd;
    while(1)
    {
	    int msgLen = recv(threadClientInfo->sockfd, buffer, sizeof(char)*MAXMSGLEN, 0);
		if (!msgLen)
	    {
    		closeConnection(threadClientInfo);
	    	return;
	    }
	    if (buffer != NULL)
	    {
	    	code = codeOnMessage(buffer, contentMsg);
	    	if (code == 2 || !msgLen)
	    	{
	    		closeConnection(threadClientInfo);
	    		return;
		    }
		    else
		    {
				printf ("\n%s\n\n", buffer);
		    	sendMessageChat (ll->head, threadClientInfo->clientName, buffer);
		    }
		    
		    
	    }
    }
}

int main (int argc, char *argv[])
{
	int sock, sockfd, sockCli;
	struct sockaddr_in sockServer, sockClient;
	pthread_t threadClient;  /* thread variables */
	char buffer[MAXMSGLEN];
	socklen_t peer_addr_size;
	ll = (struct ListClientThread *)malloc(sizeof(struct ListClientThread));
	listInit(ll);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		handle_error("socket");
	
	memset (&sockServer, 0, sizeof(sockServer));
	sockServer.sin_family = AF_INET;
	sockServer.sin_addr.s_addr = INADDR_ANY;
	sockServer.sin_port = htons(PORT);
	
	if (bind(sock, (struct sockaddr *) &sockServer, sizeof(sockServer)) == -1)
		handle_error("bind");
	if (listen(sock, LISTEN_BACKLOG) == -1)
			handle_error("listen");
		
	while (1)
	{
		sockCli = sizeof(struct sockaddr_in);
		sockfd = accept(sock, (struct sockaddr *) &sockClient, (socklen_t *)&sockCli);
		if (sockfd == -1)
			handle_error("accept");
		
		/* create threads 1 for thread client */
		struct ThreadClientInfo *threadClient = (struct ThreadClientInfo *)malloc(sizeof(struct ThreadClientInfo));
		threadClient->sockfd = sockfd;
		strcpy(threadClient->ipAddr, getIPfromSockAddress(sockClient));
		char *userName = addUserInList(threadClient->sockfd);
		strcpy(threadClient->clientName, userName);
		
        if (pthread_create (&threadClient->threadClientID, NULL, 
        		(void *) &clientHandler, threadClient) < 0){
        	handle_error("create thread");
        }
	}
	
    return 0;
}