#include "Network.h"
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

void Network::setEnvironment(char *filename)
{
		ifstream file(filename);
		string line = string();
		
		ofstream myfile;
		myfile.open ("univ.log");
		
		if(file.is_open()){
			getline(file,line);
			string token = string(strtok((char *)line.c_str()," "));
			while(!file.eof()){
				
				//Getting the token and examining it.
				if(token.compare("RANDOMSEED")==0){ 
					
					faculty_rand = atof(strtok(NULL," "));
					student_rand = atof(strtok(NULL," "));
					course_rand = atof(strtok(NULL," "));
					friend_rand = atof(strtok(NULL," "));
					getline(file,line);if(line.compare("")==0) {getline(file,line);	}
					if(file.eof()) {break;}
					token = string(strtok((char *)line.c_str()," \n"));
				}
				else if(token.compare("UNIVERSITY")==0){
					University utemp;
					utemp.name.assign((string)strtok(NULL," "));
					myfile << utemp.name<<"\n";
					getline(file,line);
					if(line.compare("")==0) {
						getline(file,line);	
					}
			
					token = string(strtok((char *)line.c_str()," "));
					
					while(!(token.compare("UNIVERSITY")==0 || token.compare("RANDOMSEED")==0)){
						if (token.compare("DEPARTMENT")==0) {
							string n = (string)strtok(NULL," ");
							int f = atoi(strtok(NULL," "));
							int s = atoi(strtok(NULL," "));
							double c = atof(strtok(NULL," "));
							double o = atof(strtok(NULL," "));
							Department dtemp(n,f,s,c,o);
							utemp.deps.push_back(dtemp);
							
						}
						else if (token.compare("COURSE")==0) {
							string n = (string)strtok(NULL," ");
							string d = (string)strtok(NULL," ");
							double f = atoi(strtok(NULL," "));
							Course ctemp(n,d,f);
							utemp.deps.back().courses.push_back(ctemp);
						}
						else if (token.compare("INTEREST")==0) {
							string s = (string)strtok(NULL," ");
							double p = atof(strtok(NULL," "));
							Interest itemp(s,p);
							utemp.deps.back().interests.push_back(itemp);
						}
						else if (token.compare("HOSTEL")==0) {
							utemp.hostels.push_back((string)strtok(NULL," "));
						}
						else if (token.compare("HOUSELOCALITY")==0) {
							utemp.houselocality.push_back((string)strtok(NULL," "));
						}
						else if (token.compare("FRIENDSHIPRATE")==0) {
							utemp.frate = atof(strtok(NULL," "));
						}
						else if (token.compare("OPENNESS")==0) {
							utemp.openness = atof(strtok(NULL," "));
						}
						else if (token.compare("FRIENDLINESS")==0) {
							utemp.friendliness = atof(strtok(NULL," "));
						}
						else {
							//invalid entry - do nothing
						}
						getline(file,line);if(line.compare("")==0) {
							getline(file,line);	
						}
						
						token = string(strtok((char *)line.c_str()," \n"));
						
					}
					university.push_back(utemp);
				}	
			}
		}
		file.close();
		myfile.close();
	}

Network::Network(){faculty_rand=0;student_rand=0;course_rand=0;friend_rand=0;}

