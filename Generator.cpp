/*! \file Generator.cpp
A file containing necessary code for running the Generator process.
*/
#include <fcntl.h>
#include <resolv.h>
#include <stdio.h>      
#include <math.h>       
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <vector>
#include <pthread.h>
#include <netdb.h>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <random>

#include "omp.h"

#include "Generator.h"
#include "Network.h"
#include "generateFaculty.h"
#include "generateStudent.h"
#include "generateCourses.h"
#include "generateFriends.h"

using namespace std;


char *randomString( int maxLen, int seed ) {
char sourceChars[] = "abcdefghijklmnopqrstuvwxyz";
srand( seed );

int scLen = strlen( sourceChars );
int len = rand() % maxLen + 1;
char *retVal = (char *) malloc( len + 1 );
int i;

for (i=0; i<len; ++i) 
retVal[i] = sourceChars[ rand() % scLen ];

retVal[len] = '\0';
return retVal;
}

string getWord(string &source){
	string word = source.substr(0, source.find(" "));
	source = source.substr(source.find(" ")+1);
	return word;
}

struct uniID{
	string uni;
	int ID;
};

uniID getIdentifier(vector<University> u, int globalNum)
{	uniID out;
for (int iter=0; iter<u.size(); iter++)
{	
	int numDepts=u[iter].deps.size();
	for (int i=0;i<numDepts;i++)
	{	int numStuInDept=u[iter].deps[i].stuList.size();
		for (int j=0; j<numStuInDept; j++)
		{	if (u[iter].deps[i].stuList[j].global_id==globalNum)
			{
				int code=u[iter].deps[i].stuList[j].empcode;
				string Uni=u[iter].name;
				out.uni=Uni;
				out.ID=code;
			}
		}
	}
}
	return out;
}

/*! \fn void printGraph(vector<University> u,int StudentCounter,int FacultyCounter)
\param u The vector of universities belonging to the social network.
\param StudentCounter The global counter for total number of students in the social network.
\param FacultyCounter The global counter for total number of faculty in the social network.

printGraph writes the graph in GraphML format in the file defaultOutput.graphml.
*/
void printGraph(vector<University> u,int StudentCounter,int FacultyCounter){ 
	
	int TotalStudents=StudentCounter-1;
	int TotalFaculty=-1*FacultyCounter-1;
	ifstream readfile;
	readfile.open("friend_log.txt"); 
	FILE *f = fopen("defaultOutput.graphml","wt"); 	
	//Opening a file.
	const char *header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"  \n xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"  \n  xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns \n http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">";
	fprintf(f, "%s\n",header); 	
	
	fprintf(f,"<key id=\"d0\" for=\"node\" attr.name=\"color\" attr.type=\"string\"/>\n");
	fprintf(f,"<key id=\"d1\" for=\"node\" attr.name=\"name\" attr.type=\"string\"/>\n");
	static int edge_iter=0;

	fprintf(f,"<graph id=\"G\" edgefault=\"undirected\">\n");
	string Identifier="blah";
	for (int i=1;i<=TotalFaculty; i++)
	{	
		fprintf(f,"\t<node id=\"fac%d\"/>\n", i);
	}
	for (int j=1;j<=TotalStudents; j++)
	{	
		uniID temp = getIdentifier(u,j);
		fprintf(f,"\t<node id=\"%d%s\"/>\n",temp.ID,temp.uni.c_str());
	}
	//Printed the opening lines of the graph.
	
	while(!readfile.eof()){
		string line;
		getline(readfile,line); 
		int id1 = atoi(getWord(line).c_str());
		int uni1= atoi(getWord(line).c_str());
		int id2 = atoi(getWord(line).c_str());
		int uni2= atoi(getWord(line).c_str());
		bool friends = atoi(getWord(line).c_str());

		if(friends){
			uniID temp1 = getIdentifier(u,id1);
			uniID temp2 = getIdentifier(u,id2);
			fprintf(f,"\t\t<edge id=\"e%d\" source=\"%d%s\" target=\"%d%s\"/>\n",edge_iter++,temp1.ID,temp1.uni.c_str(), temp2.ID, temp2.uni.c_str());
		}
	}

	fprintf(f,"</graph>\n</graphml>");
	//Printed the concluding lines of the graph.
	fclose(f);

}

/*! \fn
The main function for the Generator that runs the different threads in parallel and communicates with the TimeKeeper.
*/
int main(int argc, char **argv) {
	
		int FacultyCounter=-1;
		int StudentCounter=1;
		
		ofstream myfile;
		myfile.open ("Generator.log");
		
		Network obj;
		
		obj.setEnvironment((char *)"SocialNetworkEnv.inp");
		
		int id;
		int tot_time = 100;  //default time of simulation
		omp_lock_t lck;
		omp_init_lock(&lck);
		
		// argc should be 3 for correct execution
		if ( argc == 3 ) {
			if (strcmp(argv[1],"-d") == 0) {
				tot_time = atoi(argv[2]) * 24 * 60;
				
			}
   
		}
		
		
		#pragma omp parallel num_threads(4) private(id)
	{
			id = omp_get_thread_num();
			
			if (id == 0){
			
			srand(obj.faculty_rand);
			generateFaculty(obj.university,FacultyCounter);
			sleep(1);
			
			
			
			int host_port= 1101;
			string host = "127.0.0.1";
			const char* host_name = host.c_str();

			struct sockaddr_in my_addr;

			char buffer[1024];
			int bytecount;
			int buffer_len=0;

			int hsock;
			int * p_int;
			int err;

			hsock = socket(AF_INET, SOCK_STREAM, 0);
			if(hsock == -1){
				printf("Error initializing socket %d\n",errno);
			}
			
			p_int = (int*)malloc(sizeof(int));
			*p_int = 1;
				
			if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
				(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
				printf("Error setting options %d\n",errno);
				free(p_int);
			}
			free(p_int);

			my_addr.sin_family = AF_INET ;
			my_addr.sin_port = htons(host_port);
			
			memset(&(my_addr.sin_zero), 0, 8);
			my_addr.sin_addr.s_addr = inet_addr(host_name);

			if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
				if((err = errno) != EINPROGRESS){
					fprintf(stderr, "Error connecting socket %d\n", errno);
				}
			}

			//Now lets do the client related stuff

				buffer_len = 1024;
				memset(buffer, '\0', buffer_len);
				
				strcpy(buffer,"-1 0");
				
				if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
				fprintf(stderr, "Error sending data %d\n", errno);
				
				}
				myfile << "Sent bytes "<<bytecount<<"\n";

				if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
					fprintf(stderr, "Error receiving data %d\n", errno);
					
				}
				myfile << "---"<<id<<"---Recieved bytes "<<bytecount<<"\nReceived string \""<<buffer<<"\"\n";

				close(hsock);
				
			}

			
			else if (id == 1){
			
			
			std::vector<int> codes(obj.university.size(),0);
			int start_time = 1;
			
			
			while (start_time <= tot_time){
			
			
				
			
				int host_port= 1101;
				string host = "127.0.0.1";
				const char* host_name = host.c_str();

				struct sockaddr_in my_addr;

				char buffer[1024];
				int bytecount;
				int buffer_len=0;

				int hsock;
				int * p_int;
				int err;

				hsock = socket(AF_INET, SOCK_STREAM, 0);
				if(hsock == -1){
					printf("Error initializing socket %d\n",errno);
				}
				
				p_int = (int*)malloc(sizeof(int));
				*p_int = 1;
					
				if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
					(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
					printf("Error setting options %d\n",errno);
					free(p_int);
				
				}
				free(p_int);

				my_addr.sin_family = AF_INET ;
				my_addr.sin_port = htons(host_port);
				
				memset(&(my_addr.sin_zero), 0, 8);
				my_addr.sin_addr.s_addr = inet_addr(host_name);

				if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
					if((err = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
						
					}
				}

				//Now lets do the client related stuff
		
					buffer_len = 1024;
					memset(buffer, '\0', buffer_len);
					
					string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
					strcpy(buffer,(time_str + " 1").c_str());
					
					if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
				
					}
					myfile << "Sent bytes "<<bytecount<<" "<<buffer;

					if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
				
					}
					myfile << "---"<<id<<"---Recieved bytes "<<bytecount<<"\nReceived string \""<<buffer<<"\"\n";
					if (strcmp(buffer,"ALARM")==0) {
						srand(obj.student_rand);
						omp_set_lock(&lck);
						generateStudent(obj.university, codes, StudentCounter);
						omp_unset_lock(&lck);
						start_time += 1*365*24*60;
					}
					
					close(hsock);
					
					if (start_time == 1+1*365*24*60) {
						
						hsock = socket(AF_INET, SOCK_STREAM, 0);
						if(hsock == -1){
							printf("Error initializing socket %d\n",errno);
						}
						
						p_int = (int*)malloc(sizeof(int));
						*p_int = 1;
							
						if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
							(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
							printf("Error setting options %d\n",errno);
							free(p_int);
						}
						free(p_int);

						my_addr.sin_family = AF_INET ;
						my_addr.sin_port = htons(host_port);
						
						memset(&(my_addr.sin_zero), 0, 8);
						my_addr.sin_addr.s_addr = inet_addr(host_name);
								
						
						if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
						if((err = errno) != EINPROGRESS){
							fprintf(stderr, "Error connecting socket %d\n", errno);
						}
					}

					//Now lets do the client related stuff
						char buffer1[1024];
						buffer_len = 1024;
						memset(buffer1, '\0', buffer_len);
						
						strcpy(buffer1,"-1 1");
						
						if( (bytecount=send(hsock, buffer1, strlen(buffer1),0))== -1){
						fprintf(stderr, "Error sending data %d\n", errno);
						
						}
						myfile << "Sent bytes "<<bytecount<<"\n";

						if((bytecount = recv(hsock, buffer1, buffer_len, 0))== -1){
							fprintf(stderr, "Error receiving data %d\n", errno);
							
						}
						myfile << "---"<<id<<"---Recieved bytes "<<bytecount<<"\nReceived string \""<<buffer1<<"\"\n";
						close(hsock);
					}

				}
				
			}
				
			else if (id == 2){
			
			int start_time = 2;
			
			while (start_time <= tot_time){
			
				int host_port= 1101;
				string host = "127.0.0.1";
				const char* host_name = host.c_str();

				struct sockaddr_in my_addr;

				char buffer[1024];
				int bytecount;
				int buffer_len=0;

				int hsock;
				int * p_int;
				int err;

				hsock = socket(AF_INET, SOCK_STREAM, 0);
				if(hsock == -1){
					printf("Error initializing socket %d\n",errno);
				}
				
				p_int = (int*)malloc(sizeof(int));
				*p_int = 1;
					
				if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
					(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
					printf("Error setting options %d\n",errno);
					free(p_int);
				}
				free(p_int);

				my_addr.sin_family = AF_INET ;
				my_addr.sin_port = htons(host_port);
				
				memset(&(my_addr.sin_zero), 0, 8);
				my_addr.sin_addr.s_addr = inet_addr(host_name);

				if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
					if((err = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
					}
				}

				//Now lets do the client related stuff
					buffer_len = 1024;
					memset(buffer, '\0', buffer_len);
					
					string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
					strcpy(buffer,(time_str + " 2").c_str());
					
					if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
					
					}
					myfile << "Sent bytes "<<bytecount<<" "<<buffer<<"\n";

					if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
						
					}
					myfile << "---"<<id<<"---Recieved bytes "<<bytecount<<"\nReceived string \""<<buffer<<"\"\n";
					if (strcmp(buffer,"ALARM")==0) {
						srand(obj.course_rand);
						omp_set_lock(&lck);
						generateCourses(obj.university);
						omp_unset_lock(&lck);
						start_time += 0.5*365*24*60;
					}
					
					close(hsock);
					if (start_time == 2+0.5*365*24*60) {
						
						hsock = socket(AF_INET, SOCK_STREAM, 0);
						if(hsock == -1){
							printf("Error initializing socket %d\n",errno);
						}
						
						p_int = (int*)malloc(sizeof(int));
						*p_int = 1;
							
						if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
							(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
							printf("Error setting options %d\n",errno);
							free(p_int);
						}
						free(p_int);

						my_addr.sin_family = AF_INET ;
						my_addr.sin_port = htons(host_port);
						
						memset(&(my_addr.sin_zero), 0, 8);
						my_addr.sin_addr.s_addr = inet_addr(host_name);
						
						if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
						if((err = errno) != EINPROGRESS){
							fprintf(stderr, "Error connecting socket %d\n", errno);
						}
					}

					//Now lets do the client related stuff
			
						char buffer1[1024];
						buffer_len = 1024;
						memset(buffer1, '\0', buffer_len);
						
						strcpy(buffer1,"-1 2");
						
						if( (bytecount=send(hsock, buffer1, strlen(buffer1),0))== -1){
						fprintf(stderr, "Error sending data %d\n", errno);
						
						}
						myfile << "Sent bytes "<<bytecount<<"\n";

						if((bytecount = recv(hsock, buffer1, buffer_len, 0))== -1){
							fprintf(stderr, "Error receiving data %d\n", errno);
					
						}
						myfile << "---"<<id<<"---Recieved bytes "<<bytecount<<"\nReceived string \""<<buffer1<<"\"\n";
						close(hsock);
					}
					
				}
				
			}
			
			else if (id == 3){
			
			int start_time = 3;
			
			
				default_random_engine generator(obj.friend_rand);
				std::vector<bool> is_generate (obj.university.size(),false);
				std::vector<int> next_time(obj.university.size(),3);
				
				
				
				int host_port= 1101;
				string host = "127.0.0.1";
				const char* host_name = host.c_str();
				
				struct sockaddr_in my_addr;

				char buffer[1024];
				int bytecount;
				int buffer_len=0;

				int hsock;
				int * p_int;
				int err;

				hsock = socket(AF_INET, SOCK_STREAM, 0);
				if(hsock == -1){
					printf("Error initializing socket %d\n",errno);
				}
				
				p_int = (int*)malloc(sizeof(int));
				*p_int = 1;
					
				if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
					(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
					printf("Error setting options %d\n",errno);
					free(p_int);
				}
				free(p_int);

				my_addr.sin_family = AF_INET ;
				my_addr.sin_port = htons(host_port);
				
				memset(&(my_addr.sin_zero), 0, 8);
				my_addr.sin_addr.s_addr = inet_addr(host_name);

				if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
					if((err = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
						
					}
				}

				//Now lets do the client related stuff
					buffer_len = 1024;
					memset(buffer, '\0', buffer_len);
					
					string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
					strcpy(buffer,(time_str + " 3").c_str());
					
					if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
					
					}
					myfile << "Sent bytes "<<bytecount<<" "<<buffer<<"\n";
			
			while (start_time <= tot_time){
			
					int is_call = 0;

					if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
						
					}
					myfile << "---"<<id<<"---Recieved bytes "<<bytecount<<"\nReceived string \""<<buffer<<"\""<<start_time<<"\n";
					if (strcmp(buffer,"ALARM")==0) { 
						for (int iter=0; iter<obj.university.size();iter++) {
							is_generate[iter] = false;
						}
						is_call = 0;
						for (int iter=0; iter<obj.university.size();iter++)
						{
							if(next_time[iter] == start_time) {
								is_generate[iter] = true;
								is_call = 1;
								exponential_distribution<float> distribution(obj.university[iter].frate);
								double to_add = distribution(generator);
								next_time[iter] = start_time + (int)(ceil(to_add));
								
							}
				
						}
					
						if(is_call == 1) {
							omp_set_lock(&lck);
							generateFriends(obj.university,is_generate,StudentCounter);
							omp_unset_lock(&lck);
							
							
						}
						start_time += 1;
					}
					
					close(hsock);
					
					//-------------------------------------
					

					hsock = socket(AF_INET, SOCK_STREAM, 0);
					if(hsock == -1){
						printf("Error initializing socket %d\n",errno);

					}
					
					p_int = (int*)malloc(sizeof(int));
					*p_int = 1;
						
					if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
						(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
						printf("Error setting options %d\n",errno);
						free(p_int);
				
					}
					free(p_int);

					my_addr.sin_family = AF_INET ;
					my_addr.sin_port = htons(host_port);
					
					memset(&(my_addr.sin_zero), 0, 8);
					my_addr.sin_addr.s_addr = inet_addr(host_name);

					if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
						if((err = errno) != EINPROGRESS){
							fprintf(stderr, "Error connecting socket %d\n", errno);
					
						}
					}

					//Now lets do the client related stuff
						buffer_len = 1024;
						memset(buffer, '\0', buffer_len);
						
						string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
						strcpy(buffer,(time_str + " 3").c_str());
						
						if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
						fprintf(stderr, "Error sending data %d\n", errno);
			
						}
						myfile << "Sent bytes "<<bytecount<<" "<<buffer<<"\n";
					
					//---------------------------------------
					
					struct sockaddr_in my_addr1;

					char buffer1[1024];
					int bytecount1;
					int buffer_len1=0;

					int hsock1;
					int * p_int1;
					int err1;
					hsock1 = socket(AF_INET, SOCK_STREAM, 0);
						if(hsock1 == -1){
							printf("Error initializing socket %d\n",errno);
							
						}
						
						p_int1 = (int*)malloc(sizeof(int));
						*p_int1 = 1;
							
						if( (setsockopt(hsock1, SOL_SOCKET, SO_REUSEADDR, (char*)p_int1, sizeof(int)) == -1 )||
							(setsockopt(hsock1, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int1, sizeof(int)) == -1 ) ){
							printf("Error setting options %d\n",errno);
							free(p_int1);
							
						}
						free(p_int1);

						my_addr1.sin_family = AF_INET ;
						my_addr1.sin_port = htons(host_port);
						
						memset(&(my_addr1.sin_zero), 0, 8);
						my_addr1.sin_addr.s_addr = inet_addr(host_name);
					
					if( connect( hsock1, (struct sockaddr*)&my_addr1, sizeof(my_addr1)) == -1 ){
					if((err1 = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
					
					}
				}

				//Now lets do the client related stuff
		
					
					buffer_len1 = 1024;
					memset(buffer1, '\0', buffer_len1);
					
					strcpy(buffer1,"-1 3");
					
					if( (bytecount1=send(hsock1, buffer1, strlen(buffer1),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
					
					}
					myfile << "Sent bytes "<<bytecount1<<"\n";

					if((bytecount1 = recv(hsock1, buffer1, buffer_len1, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
						
					}
					myfile << "---"<<id<<"---Recieved bytes "<<bytecount1<<"\nReceived string \""<<buffer1<<"\"\n";
					
					close(hsock1);
				}
				
			}

	}
	
	omp_destroy_lock(&lck);
	printGraph(obj.university,StudentCounter,FacultyCounter);
	
	myfile.close();
	}
