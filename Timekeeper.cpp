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
//#include <pthread.h>
#include <vector>
//#include <mutex>


#define BUFFSIZE 1024
#define ERROR "Incorrect time entered. Retry."
#define SUCCESS "Alarm request recorded."
#define ALARM "ALARM"

using namespace std;

static int currentTime=0;
static bool ispause=true;
void SocketHandler(int*);
void AlarmHandler();
//std::mutex mtx;

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
    printf("Alarm set for %d at %d\n",client,timel);
}

int main(int argv, char** argc){

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
		printf("Error initializing socket %d\n", errno);
		goto FINISH;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
		
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", errno);
		free(p_int);
		goto FINISH;
	}
	free(p_int);

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	if( bind( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		fprintf(stderr,"Error binding to socket, make sure nothing else is listening on this port %d\n",errno);
		goto FINISH;
	}
	if(listen( hsock, 10) == -1 ){
		fprintf(stderr, "Error listening %d\n",errno);
		goto FINISH;
	}

	//Now lets do the server stuff

	addr_size = sizeof(struct sockaddr_in);
	
	//pthread_create(&service_id,0,&AlarmHandler, (void*)service_identifier);
    //pthread_detach(service_id);
    //This thread handles the servicing of the alarm requests and proceeds time. 
	
	while(1){
		printf("waiting for a connection\n");
		csock = (int*)malloc(sizeof(int));
        
        
        
        
		if((*csock = accept( hsock, (struct sockaddr*)&sadr, &addr_size))!= -1){
			printf("---------------------\nReceived connection from %s\n",inet_ntoa(sadr.sin_addr));
			SocketHandler(csock);
			//pthread_create(&thread_id,0,&SocketHandler, (void*)csock );
			//pthread_detach(thread_id);
		}
		else{
			fprintf(stderr, "Error accepting %d\n", errno);
		}
        /*
        if(!ispause){
            currentTime++;
            AlarmHandler(csock);
        }
        */
	}
	
FINISH:
;
}

void SocketHandler(int* lp){
    int *csock = (int*)lp;

	char in_buffer[BUFFSIZE];
	char out_buffer[BUFFSIZE];
	int buffer_len = 1024;
	int bytecount;

	memset(in_buffer, 0, buffer_len);
	if((bytecount = recv(*csock, in_buffer, buffer_len, 0))== -1){
		fprintf(stderr, "Error receiving data %d\n", errno);
		goto FINISH;
	}
	printf("Received bytes %d\nReceived string \"%s\"\n", bytecount, in_buffer);
	strcat(in_buffer, " SERVER ECHO");
	/*my code*/

	int alarmTime, clientLabel;

    sscanf(strtok(in_buffer," "),"%d",&alarmTime);
    sscanf(strtok(NULL," "),"%d",&clientLabel);

    if(alarmTime==-1){
        //TODO:UN-Pause. 
        strcpy(out_buffer,"Time unpaused");
        //mtx.lock();
		ispause=false;
		//mtx.unlock();
		
		AlarmHandler();
		
        if((bytecount = send(*csock, out_buffer, strlen(out_buffer), 0))== -1){
		fprintf(stderr, "Error sending data %d\n", errno);
		goto FINISH;
		}
		
    }
    else if(alarmTime<currentTime){
        strcpy(out_buffer,ERROR);
        if((bytecount = send(*csock, out_buffer, strlen(out_buffer), 0))== -1){
		fprintf(stderr, "Error sending data %d\n", errno);
		
		}
    }
    else{
        strcpy(out_buffer,SUCCESS);
        setAlarm(alarmTime,clientLabel,*csock);
       	//if((bytecount = send(*csock, out_buffer, strlen(out_buffer), 0))== -1){
		//fprintf(stderr, "Error sending data %d\n", errno);
		//goto FINISH;
		//}
    }/*
	if((bytecount = send(*csock, in_buffer, strlen(in_buffer), 0))== -1){
		fprintf(stderr, "Error sending data %d\n", errno);
		goto FINISH;
		}
	*/
	printf("Sent bytes %d %s\n", bytecount,out_buffer);

FINISH:
	free(csock);
    //return 0;
}

void AlarmHandler(){
    //bool isService = (bool)args;
    //if(isService){
		//while (1) {
			//mtx.lock();
			if(!ispause){
				
				currentTime++;
				printf("curr time = %d \n",currentTime);
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
						//mtx.lock();
						ispause  = true;
						//mtx.unlock();
						printf("Handling Alarm at %d time %d \n",i++,currentTime);
						char alarmBuffer[BUFFSIZE];
                
						//string client_str = static_cast<ostringstream*>( &(ostringstream() << client) )->str();
						strcpy(alarmBuffer,ALARM);
						//strcpy(alarmBuffer,(ALARM + client_str).c_str());
						int sv;
						if((sv = send(sock, alarmBuffer, strlen(alarmBuffer), 0))== -1){
							fprintf(stderr, "Error sending alarm \n", errno);
						}
						else{
							printf("Alarm request sent for client %d at time %d\n",client,timel);
							//ispause  = true;
						}
					}
					else{
						temp.id.push_back(client);
						temp.time.push_back(timel);
						temp.sock.push_back(sock);
					}
				}
				alarms=temp;
				//mtx.unlock();
			}
			//mtx.unlock();
		//}
		
    //}
    //pthread_exit(0);
    //return 0;
}