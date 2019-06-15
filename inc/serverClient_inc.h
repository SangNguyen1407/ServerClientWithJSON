#ifndef _SERVERCLIENT_INC_H_
#define _SERVERCLIENT_INC_H_

#include<sys/socket.h>
#include <sys/un.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <pthread.h>    /* POSIX Threads */
#include <arpa/inet.h>
#include <unistd.h> 

#define PORT			12345
#define MAXMSGLEN		1024
#define INET_ADDRSTRLEN 16
#define LISTEN_BACKLOG 	50

struct ThreadClientInfo {
    pthread_t threadClientID;
    int sockfd; 
    char clientName[MAXMSGLEN];
	char ipAddr[INET_ADDRSTRLEN];
} THREADCLIENTINFO;

struct ListNode {
	struct ThreadClientInfo *threadClient;
	struct ListNode *next;
} LISTNODE;

struct ListClientThread {
	struct ListNode *head, *tail;
	int size;
} LLIST;

#endif