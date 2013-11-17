/*! \file Generator.cpp
A file containing necessary code for running the TimeKeeper process.
*/
#include <sys/types.h>
#include <fcntl.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/in.h>
#include <resolv.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <fstream>

#define BUFFSIZE 1024
#define ERROR "Incorrect time entered. Retry."
#define SUCCESS "Alarm request recorded."
#define ALARM "ALARM"

using namespace std;

static int currentTime=0;
static bool ispause=true;
ofstream myfile;
void SocketHandler(int*);
void AlarmHandler();

struct alarmlist{
    std::vector<int> time;
    std::vector<int> id;
    std::vector<int> sock;
    alarmlist(){};
};

static alarmlist alarms;

void setAlarm(int timel, int client, int sock){
    alarms.time.push_back(timel);
    alarms.id.push_back(client);
    alarms.sock.push_back(sock);
    myfile << "Alarm set for "<<client<<" at "<<timel<<"\n";
}

/*! \fn
The main function for the TimeKeeper which does all the server side socket programming.
*/
int main(int argv, char** argc){
	
	myfile.open ("Timekeeper.log");

	int host_port= 1101;

	struct sockaddr_in my_addr;

	int hsock;
	int * p_int ;
	int err;

	socklen_t addr_size = 0;
	int* csock;
	struct sockaddr_in sadr;
	pthread_t thread_id=0;
	pthread_t service_id=0;
    bool service_identifier=true;

	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		myfile << "Error initializing socket "<<errno<<"\n";
		goto FINISH;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
		
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		myfile << "Error setting options "<<errno<<"\n";
		free(p_int);
		goto FINISH;
	}
	free(p_int);

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	if( bind( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		myfile<<"Error binding to socket, make sure nothing else is listening on this port "<<errno<<"\n";
		goto FINISH;
	}
	if(listen( hsock, 10) == -1 ){
		myfile << "Error listening "<<errno<<"\n";
		goto FINISH;
	}

	addr_size = sizeof(struct sockaddr_in);
	
	while(1){
		printf("waiting for a connection\n");
		csock = (int*)malloc(sizeof(int));
        
        
		if((*csock = accept( hsock, (struct sockaddr*)&sadr, &addr_size))!= -1){
			myfile << "---------------------\nReceived connection from "<<inet_ntoa(sadr.sin_addr)<<"\n";
			SocketHandler(csock);
		}
		else{
			myfile << "Error accepting "<<errno<<"\n";
		}
	}
	
FINISH:
myfile.close();
}

/*! \fn void SocketHandler(int* lp)
\param lp The unique socket identifier for the current connection.

Reads all incoming requests and schedules them accordingly.
*/
void SocketHandler(int* lp){
    int *csock = (int*)lp;

	char in_buffer[BUFFSIZE];
	char out_buffer[BUFFSIZE];
	int buffer_len = 1024;
	int bytecount;

	memset(in_buffer, 0, buffer_len);
	if((bytecount = recv(*csock, in_buffer, buffer_len, 0))== -1){
		myfile <<"Error receiving data "<<errno<<"\n";
		goto FINISH;
	}
	myfile << "Received bytes "<<bytecount<<"\n Received string \""<<in_buffer<<"\"\n";
	strcat(in_buffer, " SERVER ECHO");
	
	
	/*handling the request*/

	int alarmTime, clientLabel;

    sscanf(strtok(in_buffer," "),"%d",&alarmTime);
    sscanf(strtok(NULL," "),"%d",&clientLabel);

    if(alarmTime==-1){
        //UN-Pause. 
        strcpy(out_buffer,"Time unpaused");
		ispause=false;
		
		AlarmHandler();
		
        if((bytecount = send(*csock, out_buffer, strlen(out_buffer), 0))== -1){
		myfile << "Error sending data "<<errno<<"\n";
		goto FINISH;
		}
		
    }
    else if(alarmTime<currentTime){
        strcpy(out_buffer,ERROR);
        if((bytecount = send(*csock, out_buffer, strlen(out_buffer), 0))== -1){
		myfile << "Error sending data "<<errno<<"\n";
		
		}
    }
    else{
        strcpy(out_buffer,SUCCESS);
        setAlarm(alarmTime,clientLabel,*csock);  	
    }
	myfile << "Sent bytes "<<bytecount<< out_buffer<<"\n";

FINISH:
	free(csock);
}

/*! \fn void AlarmHandler()
Completes all alarm requests scheduled made by the Socket Handler at appropriate times.
*/
void AlarmHandler(){
			if(!ispause){
				
				currentTime++;
				//printf("curr time = %d \n",currentTime);
				alarmlist temp;
				int i=0;
				while(alarms.id.size()!=0){
					int timel = alarms.time.back();
					alarms.time.pop_back();
					int client = alarms.id.back();
					alarms.id.pop_back();
					int sock = alarms.sock.back();
					alarms.sock.pop_back();
					if(timel==currentTime){
						
						ispause  = true;
						
						myfile << "Handling Alarm at "<<i++<<" time "<<currentTime<<"\n";
						char alarmBuffer[BUFFSIZE];
                
						strcpy(alarmBuffer,ALARM);
						
						int sv;
						if((sv = send(sock, alarmBuffer, strlen(alarmBuffer), 0))== -1){
							myfile << "Error sending alarm "<<errno<<"\n";
						}
						else{
							myfile << "Alarm request sent for client "<<client<<" at time "<<timel<<"\n";
						}
					}
					else{
						temp.id.push_back(client);
						temp.time.push_back(timel);
						temp.sock.push_back(sock);
					}
				}
				alarms=temp;
				
			}
}
