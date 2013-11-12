/* exp example */
#include <fcntl.h>
#include <resolv.h>

#include <stdio.h>      
#include <math.h>       
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <vector>
#include <pthread.h>
#include <netdb.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <random>

#include "omp.h"

//Module containing the network specifications.
using namespace std;
int FacultyCounter=-1;
int StudentCounter=1;


struct Course{
	string name;
	string dep;
	double frequency;
	bool isFloated;

	//Default constructors.
	//Course(): name(""),dep(""),frequency(0.0),is_floated(false) {};
	//Course(string s, string d) : name(s),dep(d),frequency(0.0),is_floated(false) {};
	Course(string s, string d, double f){
		name.assign(s);
		dep.assign(d);
		frequency = f;isFloated = false;
	}
};

struct Interest{
	string name;
	double pop;

	//Interest(): name(""),pop(0.0){};
	Interest(string s, double p){
		name.assign(s);
		pop = p;
	}
};

struct Faculty{
	string name;
	int uni_index;
	int dep_index;
	unsigned int empcode;
	string house;
	std::vector<string> interests;
	std::vector<Course> courses;
	int global_id;

};

struct Student{
	string name;
	int uni_index;
	int dep_index;
	unsigned int empcode;
	string hostel;
	std::vector<string> interests;
	std::vector<Course> courses;
	int global_id;
};

struct Department{
	string name;
	int faculty;
	int students;
	int numFloated;
	double coreCourses;
	double nonCoreCourses;
	std::vector<Course> courses;
	std::vector<Interest> interests;
	std::vector<Faculty> facList;
	std::vector<Student> stuList;

	Department(string n, int f, int s, double c, double o){
		name.assign(n);
		faculty = f;students = s;coreCourses = c;nonCoreCourses = o;
		numFloated=0;
	}
};



struct University{
	string name;
	std::vector<Department> deps;
	int uniStudents;
	int uniFaculty;
	std::vector<string> hostels;
	std::vector<string> houselocality;
	double frate;
	double openness;
	double friendliness;

	University():name(""){uniStudents=0;uniFaculty=0;};
};




class Network{
public:
	vector<University> university;
	double faculty_rand,student_rand,course_rand,friend_rand;

	void setEnvironment(char *filename){
		ifstream file(filename);
		string line = string();
		if(file.is_open()){
			getline(file,line);
			string token = string(strtok((char *)line.c_str()," "));
			while(!file.eof()){
				//char *cstr = new char[line.length() + 1];
				//strcpy(cstr, line.c_str());
				
				//delete [] cstr;
				//string token = getWord(line);
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
					getline(file,line);
					if(line.compare("")==0) {getline(file,line);	}
					//char *cstr = new char[line.length() + 1];
					//strcpy(cstr, line.c_str());
					token = string(strtok((char *)line.c_str()," "));
					//string token = getWord(line);
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
							cout<<utemp.openness<<"me";
						}
						else if (token.compare("FRIENDLINESS")==0) {
							utemp.friendliness = atof(strtok(NULL," "));
						}
						else {
							//cout<<"hiii \n";
						}
						getline(file,line);if(line.compare("")==0) {getline(file,line);	}
						//char *cstr = new char[line.length() + 1];
						//strcpy(cstr, line.c_str());
						token = string(strtok((char *)line.c_str()," \n"));
						//token = getWord(line);
					}
					university.push_back(utemp);
				}	
				//getline(file, line);
				//f(line.compare("")==0) {getline(file,line);	}
			}
		}file.close();
	}
	
};




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

int* courseListGenerator(int M, int N)
{
	int in, im;

im = 0;
int *vektor = new int[M];
for (in = 0; in < N && im < M; ++in) {
  int rn = N - in;
  int rm = M - im;
  if (rand() % rn < rm)    
    /* Take it */
    *(vektor+(im++)) = in + 1; /* +1 since your range begins from 1 */
}
return vektor;
}

void generateFaculty(vector<University> &u)
{	
	for (int iter=0;iter<u.size(); iter++)
{
	int numDept=u[iter].deps.size();
	int numHouse=u[iter].houselocality.size();
	
	int totalFaculty=0, numFacInDept=0, code=0,i,j,k;

	for (i=0;i<numDept;i++)
		{
		 totalFaculty+=u[iter].deps[i].faculty;
		}
	//struct Faculty [] facList = new Faculty[totalFaculty]; 


	for (i=0;i<numDept;i++)
		{ numFacInDept=u[iter].deps[i].faculty;
		int numInterest=u[iter].deps[i].interests.size();
			for (j=0; j<numFacInDept; j++)
			{   u[iter].uniFaculty+=1;
				struct Faculty temp;
				temp.empcode=code+1;	
			//	u[iter].deps[i].facList[code].empcode=code+1;
				temp.name=randomString(20,j);
			//	u[iter].deps[i].facList[code].name=randomString(20);
				int houseNum=(rand() % numHouse);
				temp.house=u[iter].houselocality[houseNum];
			//	u[iter].deps[i].facList[code].house=u[iter].houselocality[houseNum];
				temp.global_id=FacultyCounter;
			//	u[iter].deps[i].facList[code].global_id=FacultyCounter;
				FacultyCounter-=1;
				temp.uni_index=iter;
			//	u[iter].deps[i].facList[code].uni_index=iter;
				temp.dep_index=i;	
			//	u[iter].deps[i].facList[code].dep_index=i;
				for (k=0; k<numInterest; k++)
					{
						bool isInterest=(double)(rand() % 100)<=100.0*u[iter].deps[i].interests[k].pop;
						if (isInterest==1)
							temp.interests.push_back(u[iter].deps[i].interests[k].name); 
				//			u[iter].deps[i].facList[code].interests.push_back(u[iter].deps[i].interests[k].name); 

					}				
				code+=1;
				u[iter].deps[i].facList.push_back(temp);
			}
		}
}
}

void generateStudent(vector<University> &u)
{	
	for (int iter=0;iter<u.size(); iter++)
{
	int numDept=u[iter].deps.size();
	int numHostel=u[iter].hostels.size();
	
	int totalstudent=0, numStuInDept=0, code=0,i,j,k;
	//int totalstudent=0, numStuInDept=0,i,j,k;

	for (i=0;i<numDept;i++)
		{
		 totalstudent+=u[iter].deps[i].students;
		}
	//struct student [] stuList = new student[totalstudent]; 


	for (i=0;i<numDept;i++)
		{numStuInDept=u[iter].deps[i].students;
		int numInterest=u[iter].deps[i].interests.size();
			for (j=0; j<numStuInDept; j++)
			{   u[iter].uniStudents+=1;
				struct Student temp;
				temp.empcode=code+1;
				//u[iter].deps[i].stuList[code].empcode=code+1;
				temp.name=randomString(20, j);
				//cout<<temp.name<<"\n";
				//u[iter].deps[i].stuList[code].name=randomString(20);
				//u[iter].deps[i].stuList[code].dep=d[i];
				int hostelNum=(rand() % numHostel);
				temp.hostel=u[iter].hostels[hostelNum];
				//u[iter].deps[i].stuList[code].hostel=u[iter].hostels[hostelNum];
				temp.global_id=StudentCounter;
				//u[iter].deps[i].stuList[code].global_id=StudentCounter;
				StudentCounter+=1;
				temp.uni_index=iter;
				//u[iter].deps[i].facList[code].uni_index=iter;
				temp.dep_index=i;
				//u[iter].deps[i].facList[code].dep_index=i;
				for (k=0; k<numInterest; k++)
					{
						bool isInterest=(double)(rand() % 100)<=100*u[iter].deps[i].interests[k].pop;
						if (isInterest==1)
							temp.interests.push_back(u[iter].deps[i].interests[k].name); 
				//			u[iter].deps[i].stuList[code].interests.push_back(u[iter].deps[i].interests[k].name); 

					}				
				code+=1;
				u[iter].deps[i].stuList.push_back(temp);
			}
		}
}
}

void generateCourses(vector<University> &u)
{	
	for (int iter=0;iter<u.size(); iter++)
	{

	int i,j,k,l,m,p,flag, core, core1, nonCore, nonCore1, coreID, nonCoreID, core_real=0, nonCore_real=0,courseID;
	double core2, nonCore2;
	int numDepts=u[iter].deps.size();
	
	//floating courses and finding number of courses floated for every department
	for (j=0;j<numDepts;j++)

	{   u[iter].deps[j].numFloated=0;
		int numCourses=u[iter].deps[j].courses.size();
		
		for (i=0,p=0; i<numCourses; i++)
			{u[iter].deps[j].courses[i].isFloated=(double)(rand() % 100)<=(u[iter].deps[j].courses[i].frequency)*100.0/2.0;
			if (u[iter].	deps[j].courses[i].isFloated)
				{u[iter].deps[j].numFloated+=1;

			//alloting courses to faculty
					u[iter].deps[j].facList[p++].courses.push_back(u[iter].deps[j].courses[i]);
					//	cout<<u[iter].deps[j].facList[p-1].courses[0].name<<"cour"<<p<<" \n";
			}
			}



		int numStudents=u[iter].deps[j].stuList.size();

		core= u[iter].deps[j].coreCourses;
		core1=(int)(ceil(core));
		core2=(double)core1-core;
		nonCore=u[iter].deps[j].nonCoreCourses;
		nonCore1=(int)(ceil(nonCore));
		nonCore2=(double)nonCore1-nonCore;
		core_real = core1;
		nonCore_real = nonCore1;
		for (i=0; i<numStudents; i++)
		{	//find if (core1+1)th course is taken
			if ((double)(rand() % 100)<=(core2*100.0))
				core_real=core1+1;		//TODO:add core_real to Student struct


			//find if (nonCore+1)th course is taken
			if ((double)(rand() % 100)<=(nonCore2*100.0))
				nonCore_real=nonCore1+1; //TODO:add nonCore_real to Student struct


		//check if it's feasible to select such number of courses
		if (core_real>u[iter].deps[j].numFloated)
			core_real=u[iter].deps[j].numFloated;

		//select core1 objects from all courses
		int *availCoreList;
		availCoreList=courseListGenerator(core_real, u[iter].deps[j].numFloated);
		//cout<<core1<<" "<<core_real<<" "<<u[iter].deps[j].numFloated<<"\n";
		//cout<<*(availCoreList+0)<<" "<<*(availCoreList+1)<<" "<<*(availCoreList+2)<<"\n";
		for (coreID=0; coreID<core_real; coreID++)
			{	//cout<<u[iter].deps[j].courses[*(availCoreList+coreID)].name<<" "<<u[iter].deps[j].stuList[i].name<<"\n";
				u[iter].deps[j].stuList[i].courses.push_back(u[iter].deps[j].courses[*(availCoreList+coreID)]);
			}

		int totalNonCoreFloated=0;

		for (l=0; l<numDepts; l++)
			{	if (l!=j)
				{
				int numCourses=u[iter].deps[j].courses.size();

				for (m=0; m<numCourses; m++)
					if (u[iter].deps[j].courses[m].isFloated)
						totalNonCoreFloated+=1;
				}
			}

		int *availNonCoreList;
		availNonCoreList=courseListGenerator(nonCore1, totalNonCoreFloated);
		
		for (nonCoreID=0; nonCoreID<nonCore_real; nonCoreID++)
			{	courseID=0;
				for (l=0; l<numDepts; l++)
				{	if (l!=j)
					{
						int numCourses=u[iter].deps[j].courses.size();
						for (m=0; m<numCourses; m++)
							{	if (*(availNonCoreList+nonCoreID)==courseID)
									u[iter].deps[j].stuList[i].courses.push_back(u[iter].deps[l].courses[m]);
								courseID+=1;
							}
					}
				}
			}

		}
	}
  }
}


struct Student getStudentSender(University u, int localNum)
{	
	int numDepts=u.deps.size();
	//struct Student temp=u.deps[0].stuList[3];
	for (int i=0;i<numDepts;i++)
	{
		int numStuInDept=u.deps[i].stuList.size();
		for (int j=0; j<numStuInDept; j++)
		{	
			if (u.deps[i].stuList[j].empcode==localNum)
				{  
				return u.deps[i].stuList[j];
			}
		}
	}
	//return temp;
}

struct Student getStudentReceiver(vector<University> u, int iter,int localNum)
{	int counter=0;

		//struct Student temp=u[0].deps[0].stuList[3];
		//cout<<temp.name<<"asdfg \n";
	//cout<<"asdfg \n"; 
	for (int m=0;m<u.size(); m++)
	{	if (iter!=m)
		{
		int numDepts=u[m].deps.size();
	//	cout<<"m"<<m<<" numDepts:"<<numDepts<<"\n";
		for (int i=0;i<numDepts;i++)
		{//cout<<temp.name<<"asdfg"<<localNum<<" \n";
	//cout<<"qwertyuioo"<<i<<" \n";
			int numStuInDept=u[m].deps[i].stuList.size();
			//cout<<"mnbvcx \n";
			//cout<<counter<<"ball"<<numStuInDept<<"cat"<<localNum<<" \n";
			if (counter<=localNum)
			{
				for (int j=0; j<numStuInDept; j++)
				{//cout<<j<<"aditya"<<localNum<<"\n";
					if (counter==localNum)
						{ 
						//cout<<u[m].deps[i].stuList[j].empcode<<" yay!" <<m<<" ncl" <<i<<"kka" <<j<<"lkc \n";
						return u[m].deps[i].stuList[j];
					}
					counter+=1;
				}
			}
			//counter+=numStuInDept;
		}
	}
	}
	//return temp;

}

int getCourseFriend(University u, Student s)
{
	vector<int> possibleFriends;
	int numDepts=u.deps.size();
	for (int i=0;i<numDepts;i++)
	{	int numStuInDept=u.deps[i].stuList.size();
		for (int j=0; j<numStuInDept; j++)
		{	for (int k=0, flag=0; k<u.deps[i].stuList[j].courses.size() && flag!=1;k++)
			{
			struct vector<Course> v=s.courses;
			//sort(v.begin(),v.end());
			//if (u.deps[i].stuList[j].empcode!=s.empcode && std::find(v.begin(), v.end(),u.deps[i].stuList[j].courses[k]) != v.end())
			for (int l=0;l<v.size() && flag!=1; l++)

				{ if (u.deps[i].stuList[j].empcode!=s.empcode && v[l].name.compare(u.deps[i].stuList[j].courses[k].name)==0)
					{possibleFriends.push_back(u.deps[i].stuList[j].global_id);
					flag=1;
				}
				}
			}
		}
	}

	return possibleFriends[rand()%possibleFriends.size()];
}


int getDeptFriend(University u, Student s)
{
	vector<int> possibleFriends;

	int numStuInDept=u.deps[s.dep_index].stuList.size();
	for (int j=0; j<numStuInDept; j++)
	{
		if (u.deps[s.dep_index].stuList[j].empcode!=s.empcode) 
			possibleFriends.push_back(u.deps[s.dep_index].stuList[j].global_id);
	}
		
	return possibleFriends[rand()%possibleFriends.size()];
}

int getNbourFriend(University u, Student s)
{
	vector<int> possibleFriends;
	int numDepts=u.deps.size();
	for (int i=0;i<numDepts;i++)
	{	int numStuInDept=u.deps[i].stuList.size();
		for (int j=0; j<numStuInDept; j++)
		{	
			if (u.deps[i].stuList[j].empcode!=s.empcode && s.hostel.compare(u.deps[i].stuList[j].hostel)==0)
				possibleFriends.push_back(u.deps[s.dep_index].stuList[j].global_id);
		}
	}

	return possibleFriends[rand()%possibleFriends.size()];
}
int getInterestFriend(University u, Student s)
{
	vector<int> possibleFriends;
	int numStuInDept=u.deps[s.dep_index].stuList.size();
	for (int j=0; j<numStuInDept; j++)
		{	//cout<<"poi"<<numStuInDept<<" \n";
			for (int k=0, flag=0; k<u.deps[s.dep_index].stuList[j].interests.size() && flag!=1;k++)
			{//cout<<"poi2a"<< u.deps[s.dep_index].stuList[j].interests.size()<<"\n";
			struct vector<string> v=s.interests;
			//sort(v.begin(),v.end());
			//if (u.deps[i].stuList[j].empcode!=s.empcode && std::find(v.begin(), v.end(),u.deps[i].stuList[j].courses[k]) != v.end())
			for (int l=0;l<v.size() && flag!=1; l++)

				{ //cout<<"poi3a"<<v.size()<<" \n";
					if (u.deps[s.dep_index].stuList[j].empcode!=s.empcode && v[l].compare(u.deps[s.dep_index].stuList[j].interests[k])==0)
					{//cout<<"yay \n";
						possibleFriends.push_back(u.deps[s.dep_index].stuList[j].global_id);
					flag=1;
				}
				}
			}
		}
	//	cout<<possibleFriends.size()<<"size \n";
	return possibleFriends[rand()%possibleFriends.size()];
}

void generateFriends(vector<University> u, vector<bool> is_generate)
{	 cout<<"GENERATE FRIENDS IS CALLED! \n";
	int TotalStudents=StudentCounter-1;
	int TotalFaculty=-1*FacultyCounter-1;
	ofstream outfile;

	for (int iter=0;iter<u.size(); iter++)
	{	
  	if (is_generate[iter] == true)
  		{
  		int localNum=(rand() % u[iter].uniStudents)+1;		
  		int global_id1, global_id2, receiverUni;
  		struct Student frequest_senderS;
  		
  		frequest_senderS=getStudentSender(u[iter], localNum);
  		global_id1=frequest_senderS.global_id;
  		cout<<"GLOBAL ID 1: "<<global_id1<<"\n";
  		//now find whether it will be sent to someone inside or outside the university
  		if ((double)((rand()%100)+1)<=100.0*u[iter].openness)     //change was made here
  			//request to someone outside
  			{ //cout<<"inside\n";
  				int localNum= (rand() % (TotalStudents-u[iter].uniStudents))+1;
			  						  						
  				struct Student frequest_receiverS;
  				frequest_receiverS=getStudentReceiver(u, iter, localNum);
  				receiverUni=frequest_receiverS.uni_index;
  				global_id2=frequest_receiverS.global_id;
  					  		cout<<"GLOBAL ID 2: "<<global_id2<<"\n";

  	 					
  				char result[100]=""; // enough to hold all numbers up to 64-bits

  				if ((double)(rand() % 100) < 100.0*u[receiverUni].friendliness)
  				{	//write to file friendship request between friend_request_generator and receiver_id accepted
					sprintf(result, "%d %d %d %d 1", global_id1, iter, global_id2, receiverUni);
  				}
  				else
  				{
  					//write to file friendship request between friend_request_generator and receiver_id rejected
  					sprintf(result, "%d %d %d %d 0", global_id1, iter, global_id2, receiverUni);
  				}
  				
  				outfile.open("friend_log.txt", ios_base::app);
  				outfile << result; 
  				outfile<<"\n";
  				outfile.close();
  				
  			} 
  			
  		else
  			//request to someone inside
  			
  			{	//cout<<"outside \n";
  			int condNum=(rand()%100)+1, receiver_id=0;
  			//condNum=13;
  			//cout<<"condNum "<<condNum<<"\n";
  			char result[100]=""; // enough to hold all numbers up to 64-bits
  			if (condNum<=25)
  				{ //select random person sharing course
  				//		cout<<condNum<<"rew1 \n";
  					receiver_id=getCourseFriend(u[iter], frequest_senderS);
  				//		cout<<condNum<<"rew1 \n";
  				}
  				else if (condNum>25 && condNum<=50)
  				{ //select random person sharing department
  					receiver_id=getDeptFriend(u[iter],frequest_senderS);
  				}
  				else if (condNum>25 && condNum<=50)
  				{ // select random person sharing neighbourhood
  					receiver_id=getNbourFriend(u[iter], frequest_senderS);
  				}
  				else
  				{
  				//select random person sharing interest
  				//	cout<<condNum<<"rew4 \n";
  					receiver_id=getInterestFriend(u[iter], frequest_senderS);
  				//	cout<<condNum<<"rew4 \n";
  				}
  				

  				if ((double)(rand() % 100) < 100.0*u[iter].friendliness)
  				{	//write to file friendship request between friend_request_generator and receiver_id accepted
					sprintf(result, "%d %d %d %d 1", global_id1, iter, receiver_id, iter);
  				}
  				else
  				{
  					//write to file friendship request between friend_request_generator and receiver_id rejected
  					sprintf(result, "%d %d %d %d 0", global_id1, iter, receiver_id, iter);
  				}
  			//	cout<<result<<"final result";
  				outfile.open("friend_log.txt", ios_base::app);
  				outfile << result; 
  			//	cout<<"2 written to file"<<result<<"\n";
  				outfile<<"\n";
  				outfile.close();
  			
  			}
  		
		}
	
	}
	//outfile.close();
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
	{	int numStuInDept=u[iter].deps[i].stuList.size();cout << "LALALALA 714"<<endl;
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

void printGraph(vector<University> u){ 
	cout<<"print graph is running yayyyyyyyyyyyyyyy	yyyyyyyyyyyyyyyyyyyyy\n";
	int TotalStudents=StudentCounter-1;
	int TotalFaculty=-1*FacultyCounter-1;
	ifstream readfile;	cout << "LALALALA 735"<<endl;
	readfile.open("friend_log.txt"); 	cout << "LALALALA 736"<<endl;
	FILE *f = fopen("defaultOutput.graphml","wt"); 	cout << "LALALALA 737"<<endl;
	//Opening a file.
	const char *header = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"  \n xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"  \n  xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns \n http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">";
	fprintf(f, "%s\n",header); 	cout << "LALALALA 740"<<endl;
	//Here we will set the color settings of the graph.
	/* Red nodes -> Faculty
	Green nodes -> Students
	Blue nodes -> Courses*/
	fprintf(f,"<key id=\"d0\" for=\"node\" attr.name=\"color\" attr.type=\"string\"/>\n");
	fprintf(f,"<key id=\"d1\" for=\"node\" attr.name=\"name\" attr.type=\"string\"/>\n");
	static int edge_iter=0;

	fprintf(f,"<graph id=\"G\" edgefault=\"undirected\">\n");cout << "LALALALA 749"<<endl;
	string Identifier="blah";cout << "LALALALA 750"<<endl;
	for (int i=1;i<=TotalFaculty; i++)
	{	
		fprintf(f,"\t<node id=\"fac%d\"/>\n", i);
	}cout << "LALALALA 754"<<endl;
	for (int j=1;j<=TotalStudents; j++)
	{	//Identifier=getIdentifier(u, j);
		uniID temp = getIdentifier(u,j);
		fprintf(f,"\t<node id=\"%d%s\"/>\n",temp.ID,temp.uni.c_str());
	}cout << "LALALALA 758"<<endl;
	//Printed the opening lines of the graph.
	cout << "LALALALA 760"<<endl;
	while(!readfile.eof()){
		string line;
		getline(readfile,line); cout << "LALALALA 763"<<endl;
		int id1 = atoi(getWord(line).c_str());
		int uni1= atoi(getWord(line).c_str());
		int id2 = atoi(getWord(line).c_str());
		int uni2= atoi(getWord(line).c_str());
		cout<<"University 1: "<<uni1<< " University 2:"<<uni2<<"\n";
		bool friends = atoi(getWord(line).c_str());
		//fprintf(f,"\t<node id=\"f%d\"/>\n",id1);
		//fprintf(f,"\t<node id=\"f%d\"/>\n",id2);
		if(friends){
			uniID temp1 = getIdentifier(u,id1);
			uniID temp2 = getIdentifier(u,id2);
			fprintf(f,"\t\t<edge id=\"e%d\" source=\"%d%s\" target=\"%d%s\"/>\n",edge_iter++,temp1.ID,temp1.uni.c_str(), temp2.ID, temp2.uni.c_str());
		}
	}

	fprintf(f,"</graph>\n</graphml>");
	//Printed the concluding lines of the graph.
	fclose(f);
	cout << "LALALALA"<<endl;
}
int main(int argv, char** argc) {
		Network obj;
		
		obj.setEnvironment((char *)"SocialNetworkEnv.inp");
		/*
		srand(obj.faculty_rand);
		generateFaculty(obj.university);
		srand(obj.student_rand);
		generateStudent(obj.university);
		srand(obj.course_rand);
		generateCourses(obj.university);
		
		//cout<<obj.university[0].deps[0].facList[0].courses.size()<<" course\n";
		//cout<<obj.university[0].deps[0].facList[1].name<<"\n";
		//cout<<obj.university[0].deps[0].facList[2].name<<"\n";
		srand(obj.friend_rand);
		for(int i=0;i<10000;i++) {
        generateFriends(obj.university,i);
			}
		return 0;
		*/
		int id;
		int tot_time = 100;  //default time of simulation
		omp_lock_t lck;
		omp_init_lock(&lck);
		
		
		
		
		#pragma omp parallel num_threads(4) private(id)
	{
			id = omp_get_thread_num();
			
			if (id == 0){
			
			srand(obj.faculty_rand);
			generateFaculty(obj.university);
			sleep(1);
			
			
			
			int host_port= 1101;
			char* host_name="127.0.0.1";

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
				//goto FINISH;
			}
			
			p_int = (int*)malloc(sizeof(int));
			*p_int = 1;
				
			if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
				(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
				printf("Error setting options %d\n",errno);
				free(p_int);
				//goto FINISH;
			}
			free(p_int);

			my_addr.sin_family = AF_INET ;
			my_addr.sin_port = htons(host_port);
			
			memset(&(my_addr.sin_zero), 0, 8);
			my_addr.sin_addr.s_addr = inet_addr(host_name);

			if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
				if((err = errno) != EINPROGRESS){
					fprintf(stderr, "Error connecting socket %d\n", errno);
					//goto FINISH;
				}
			}

			//Now lets do the client related stuff
	
				
				buffer_len = 1024;
				memset(buffer, '\0', buffer_len);
				
				strcpy(buffer,"-1 0");
				
				if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
				fprintf(stderr, "Error sending data %d\n", errno);
				//goto FINISH;
				}
				printf("Sent bytes %d\n", bytecount);

				if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
					fprintf(stderr, "Error receiving data %d\n", errno);
					//goto FINISH;
				}
				printf("---%d---Recieved bytes %d\nReceived string \"%s\"\n",id, bytecount, buffer);

				close(hsock);
				//FINISH:
				//;
			}

			
			else if (id == 1){
			
			int start_time = 1;
			//int code = 1;
			
			while (start_time <= tot_time){
			
			
				int host_port= 1101;
				char* host_name="127.0.0.1";

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
					//goto FINISH;
				}
				
				p_int = (int*)malloc(sizeof(int));
				*p_int = 1;
					
				if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
					(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
					printf("Error setting options %d\n",errno);
					free(p_int);
					//goto FINISH;
				}
				free(p_int);

				my_addr.sin_family = AF_INET ;
				my_addr.sin_port = htons(host_port);
				
				memset(&(my_addr.sin_zero), 0, 8);
				my_addr.sin_addr.s_addr = inet_addr(host_name);

				if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
					if((err = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
						//goto FINISH;
					}
				}

				//Now lets do the client related stuff
		
					buffer_len = 1024;
					memset(buffer, '\0', buffer_len);
					
					string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
					strcpy(buffer,(time_str + " 1").c_str());
					
					//strcpy(buffer,"1 1");
					
					if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
					//goto FINISH;
					}
					printf("Sent bytes %d %s \n", bytecount,buffer);

					if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
						//goto FINISH;
					}
					printf("---%d---Recieved bytes %d\nReceived string \"%s\"\n",id, bytecount, buffer);
					if (strcmp(buffer,"ALARM")==0) {
						srand(obj.student_rand);
						omp_set_lock(&lck);
						generateStudent(obj.university);
						omp_unset_lock(&lck);
						//code++;
						start_time += 1*365*24*60; //printf("advasdvasdcvac \n");
					}
					
					close(hsock);
					
					if (start_time == 1+1*365*24*60) {
						
						hsock = socket(AF_INET, SOCK_STREAM, 0);
						if(hsock == -1){
							printf("Error initializing socket %d\n",errno);
							//goto FINISH;
						}
						
						p_int = (int*)malloc(sizeof(int));
						*p_int = 1;
							
						if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
							(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
							printf("Error setting options %d\n",errno);
							free(p_int);
							//goto FINISH;
						}
						free(p_int);

						my_addr.sin_family = AF_INET ;
						my_addr.sin_port = htons(host_port);
						
						memset(&(my_addr.sin_zero), 0, 8);
						my_addr.sin_addr.s_addr = inet_addr(host_name);
								
						
						if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
						if((err = errno) != EINPROGRESS){
							fprintf(stderr, "Error connecting socket %d\n", errno);
							//goto FINISH;
						}
					}

					//Now lets do the client related stuff
						char buffer1[1024];
						buffer_len = 1024;
						memset(buffer1, '\0', buffer_len);
						//string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
						//strcpy(buffer,(time_str + " 3").c_str());
						
						strcpy(buffer1,"-1 1");
						
						if( (bytecount=send(hsock, buffer1, strlen(buffer1),0))== -1){
						fprintf(stderr, "Error sending data %d\n", errno);
						//goto FINISH;
						}
						printf("Sent bytes %d\n", bytecount);

						if((bytecount = recv(hsock, buffer1, buffer_len, 0))== -1){
							fprintf(stderr, "Error receiving data %d\n", errno);
							//goto FINISH;
						}
						printf("---%d---Recieved bytes %d\nReceived string \"%s\"\n",id, bytecount, buffer1);
						close(hsock);
					}
					
					//FINISH:
					//;
				}
				
			}
				
			else if (id == 2){
			
			int start_time = 2;
			
			while (start_time <= tot_time){
			
				int host_port= 1101;
				char* host_name="127.0.0.1";

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
					//goto FINISH;
				}
				
				p_int = (int*)malloc(sizeof(int));
				*p_int = 1;
					
				if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
					(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
					printf("Error setting options %d\n",errno);
					free(p_int);
					//goto FINISH;
				}
				free(p_int);

				my_addr.sin_family = AF_INET ;
				my_addr.sin_port = htons(host_port);
				
				memset(&(my_addr.sin_zero), 0, 8);
				my_addr.sin_addr.s_addr = inet_addr(host_name);

				if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
					if((err = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
						//goto FINISH;
					}
				}

				//Now lets do the client related stuff
					buffer_len = 1024;
					memset(buffer, '\0', buffer_len);
					
					string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
					strcpy(buffer,(time_str + " 2").c_str());
					
					//strcpy(buffer,"1 1");
					
					if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
					//goto FINISH;
					}
					printf("Sent bytes %d %s \n", bytecount,buffer);

					if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
						//goto FINISH;
					}
					printf("---%d---Recieved bytes %d\nReceived string \"%s\"\n",id, bytecount, buffer);
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
							//goto FINISH;
						}
						
						p_int = (int*)malloc(sizeof(int));
						*p_int = 1;
							
						if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
							(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
							printf("Error setting options %d\n",errno);
							free(p_int);
							//goto FINISH;
						}
						free(p_int);

						my_addr.sin_family = AF_INET ;
						my_addr.sin_port = htons(host_port);
						
						memset(&(my_addr.sin_zero), 0, 8);
						my_addr.sin_addr.s_addr = inet_addr(host_name);
						
						if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
						if((err = errno) != EINPROGRESS){
							fprintf(stderr, "Error connecting socket %d\n", errno);
							//goto FINISH;
						}
					}

					//Now lets do the client related stuff
			
						char buffer1[1024];
						buffer_len = 1024;
						memset(buffer1, '\0', buffer_len);
						//string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
						//strcpy(buffer,(time_str + " 3").c_str());
						
						strcpy(buffer1,"-1 2");
						
						if( (bytecount=send(hsock, buffer1, strlen(buffer1),0))== -1){
						fprintf(stderr, "Error sending data %d\n", errno);
						//goto FINISH;
						}
						printf("Sent bytes %d\n", bytecount);

						if((bytecount = recv(hsock, buffer1, buffer_len, 0))== -1){
							fprintf(stderr, "Error receiving data %d\n", errno);
							//goto FINISH;
						}
						printf("---%d---Recieved bytes %d\nReceived string \"%s\"\n",id, bytecount, buffer1);
						close(hsock);
					}
					
					//FINISH:
					//;
				}
				
			}
			
			else if (id == 3){
			
			int start_time = 3;
			
			
				default_random_engine generator(obj.friend_rand);
				std::vector<bool> is_generate (obj.university.size(),false);
				std::vector<int> next_time(obj.university.size(),3);
				
				
				
				int host_port= 1101;
				char* host_name="127.0.0.1";

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
					//goto FINISH;
				}
				
				p_int = (int*)malloc(sizeof(int));
				*p_int = 1;
					
				if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
					(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
					printf("Error setting options %d\n",errno);
					free(p_int);
					//goto FINISH;
				}
				free(p_int);

				my_addr.sin_family = AF_INET ;
				my_addr.sin_port = htons(host_port);
				
				memset(&(my_addr.sin_zero), 0, 8);
				my_addr.sin_addr.s_addr = inet_addr(host_name);

				if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
					if((err = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
						//goto FINISH;
					}
				}

				//Now lets do the client related stuff
					buffer_len = 1024;
					memset(buffer, '\0', buffer_len);
					
					string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
					strcpy(buffer,(time_str + " 3").c_str());
					
					//strcpy(buffer,"1 1");
					
					if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
					//goto FINISH;
					}
					printf("Sent bytes %d %s \n", bytecount,buffer);
			
			while (start_time <= tot_time){
			
					int is_call = 0;

					if((bytecount = recv(hsock, buffer, buffer_len, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
						//goto FINISH;
					}
					printf("---%d---Recieved bytes %d\nReceived string \"%s\"  %d \n",id, bytecount, buffer, start_time);
					if (strcmp(buffer,"ALARM")==0) { 
						for (int iter=0; iter<obj.university.size();iter++) {
							is_generate[iter] = false;
						}
						is_call = 0; //cout<<"KASVGBJSDVBSDCVSDVKVKVKVLVLSVLVLSDVL"<<obj.university.size()<< "\n";
						for (int iter=0; iter<obj.university.size();iter++)
						{
							if(next_time[iter] == start_time) {
								is_generate[iter] = true;
								is_call = 1;
								exponential_distribution<float> distribution(obj.university[iter].frate);
								double to_add = distribution(generator);
								printf("KASVGBJSDVBSDCVSDVKVKVKVLVLSVLVLSDVL %f \n",to_add);
								next_time[iter] = start_time + (int)(ceil(to_add));
								
							}
				
						}
					
						if(is_call == 1) {
							omp_set_lock(&lck);
							cout<<"YES! YES!";
							generateFriends(obj.university,is_generate); //cout<<"harsh \n";
							cout<<"jaaaaaaaaaaaaaaaaaveeeeeeeeeeeeeddddddddd \n";
							omp_unset_lock(&lck);
							
							
						}
						//srand(obj.friend_rand);
						//omp_set_lock(&lck);
						//generateFriends(obj.university,(start_time )); //cout<<"harsh \n";
						//omp_unset_lock(&lck);
						start_time += 1;
					}
					
					close(hsock);
					
					//-------------------------------------
					

					hsock = socket(AF_INET, SOCK_STREAM, 0);
					if(hsock == -1){
						printf("Error initializing socket %d\n",errno);
						//goto FINISH;
					}
					
					p_int = (int*)malloc(sizeof(int));
					*p_int = 1;
						
					if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
						(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
						printf("Error setting options %d\n",errno);
						free(p_int);
						//goto FINISH;
					}
					free(p_int);

					my_addr.sin_family = AF_INET ;
					my_addr.sin_port = htons(host_port);
					
					memset(&(my_addr.sin_zero), 0, 8);
					my_addr.sin_addr.s_addr = inet_addr(host_name);

					if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
						if((err = errno) != EINPROGRESS){
							fprintf(stderr, "Error connecting socket %d\n", errno);
							//goto FINISH;
						}
					}

					//Now lets do the client related stuff
						buffer_len = 1024;
						memset(buffer, '\0', buffer_len);
						
						string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
						strcpy(buffer,(time_str + " 3").c_str());
						
						//strcpy(buffer,"1 1");
						
						if( (bytecount=send(hsock, buffer, strlen(buffer),0))== -1){
						fprintf(stderr, "Error sending data %d\n", errno);
						//goto FINISH;
						}
						printf("Sent bytes %d %s \n", bytecount,buffer);
					
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
							//goto FINISH;
						}
						
						p_int1 = (int*)malloc(sizeof(int));
						*p_int1 = 1;
							
						if( (setsockopt(hsock1, SOL_SOCKET, SO_REUSEADDR, (char*)p_int1, sizeof(int)) == -1 )||
							(setsockopt(hsock1, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int1, sizeof(int)) == -1 ) ){
							printf("Error setting options %d\n",errno);
							free(p_int1);
							//goto FINISH;
						}
						free(p_int1);

						my_addr1.sin_family = AF_INET ;
						my_addr1.sin_port = htons(host_port);
						
						memset(&(my_addr1.sin_zero), 0, 8);
						my_addr1.sin_addr.s_addr = inet_addr(host_name);
					
					if( connect( hsock1, (struct sockaddr*)&my_addr1, sizeof(my_addr1)) == -1 ){
					if((err1 = errno) != EINPROGRESS){
						fprintf(stderr, "Error connecting socket %d\n", errno);
						//goto FINISH;
					}
				}

				//Now lets do the client related stuff
		
					//char buffer1[1024];
					buffer_len1 = 1024;
					memset(buffer1, '\0', buffer_len1);
					//string time_str = static_cast<ostringstream*>( &(ostringstream() << start_time) )->str();
					//strcpy(buffer,(time_str + " 3").c_str());
					
					strcpy(buffer1,"-1 3");
					
					if( (bytecount1=send(hsock1, buffer1, strlen(buffer1),0))== -1){
					fprintf(stderr, "Error sending data %d\n", errno);
					//goto FINISH;
					}
					printf("Sent bytes %d\n", bytecount1);

					if((bytecount1 = recv(hsock1, buffer1, buffer_len1, 0))== -1){
						fprintf(stderr, "Error receiving data %d\n", errno);
						//goto FINISH;
					}
					printf("---%d---Recieved bytes %d\nReceived string \"%s\"\n",id, bytecount1, buffer1);
					
					close(hsock1);
					//FINISH:
					//;
					
				}
				
			}

	}
	
	omp_destroy_lock(&lck);
	printGraph(obj.university);
	}