/*! \file generateFriends.cpp
A file containing necessary code for running the thread generateFriends.cpp
*/

#include "generateFriends.h"
#include <string.h>
#include <fstream>

using namespace std;

/*! \fn struct Student getStudentSender(University u, int localNum)
\param u The university from which the student sender is selected.
\param localNum A temporary variable used for ascertaining the student in the university sending the friend request.
*/
struct Student getStudentSender(University u, int localNum)
{	
	int numDepts=u.deps.size();
	
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
}

/*! \fn struct Student getStudentReceiver(vector<University> u, int iter,int localNum)
\param u The vector of universities from which the student receiver is selected.
\param iter The university number from which th friend request is generated.
\param localNum A temporary variable used for ascertaining the student receiving the friend request.
*/
struct Student getStudentReceiver(vector<University> u, int iter,int localNum)
{	int counter=0;

	for (int m=0;m<u.size(); m++)
	{	if (iter!=m)
		{
		int numDepts=u[m].deps.size();
	
		for (int i=0;i<numDepts;i++)
		{
			int numStuInDept=u[m].deps[i].stuList.size();
			
			if (counter<=localNum)
			{
				for (int j=0; j<numStuInDept; j++)
				{
					if (counter==localNum)
						{ 
						
						return u[m].deps[i].stuList[j];
					}
					counter+=1;
				}
			}
			
		}
	}
	}

}

/*! \fn int getCourseFriend(University u, Student s)
\param u The university from where the friendship request is sent.
\param s The student who sent out the friend request.
*/
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
			
			for (int l=0;l<v.size() && flag!=1; l++)

				{ if (u.deps[i].stuList[j].empcode!=s.empcode && v[l].name.compare(u.deps[i].stuList[j].courses[k].name)==0)
					{possibleFriends.push_back(u.deps[i].stuList[j].global_id);
					flag=1;
				}
				}
			}
		}
	}

	if (possibleFriends.size() == 0) {
		return 0;
	}
	
	return possibleFriends[rand()%possibleFriends.size()];
}

/*! \fn int getDeptFriend(University u, Student s)
\param u The university from where the friendship request is sent.
\param s The student who sent out the friend request.
*/
int getDeptFriend(University u, Student s)
{
	vector<int> possibleFriends;

	int numStuInDept=u.deps[s.dep_index].stuList.size();
	for (int j=0; j<numStuInDept; j++)
	{
		if (u.deps[s.dep_index].stuList[j].empcode!=s.empcode) 
			possibleFriends.push_back(u.deps[s.dep_index].stuList[j].global_id);
	}
	
	if (possibleFriends.size() == 0) {
		return 0;
	}	
	return possibleFriends[rand()%possibleFriends.size()];
}

/*! \fn int getNbourFriend(University u, Student s)
\param u The university from where the friendship request is sent.
\param s The student who sent out the friend request.
*/
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
	if (possibleFriends.size() == 0) {
		return 0;
	}
	return possibleFriends[rand()%possibleFriends.size()];
}

/*! \fn int getInterestFriend(University u, Student s)
\param u The university from where the friendship request is sent.
\param s The student who sent out the friend request.
*/
int getInterestFriend(University u, Student s)
{
	vector<int> possibleFriends;
	int numStuInDept=u.deps[s.dep_index].stuList.size();
	for (int j=0; j<numStuInDept; j++)
		{	
			for (int k=0, flag=0; k<u.deps[s.dep_index].stuList[j].interests.size() && flag!=1;k++)
			{
			struct vector<string> v=s.interests;
			
			for (int l=0;l<v.size() && flag!=1; l++)

				{ 
					if (u.deps[s.dep_index].stuList[j].empcode!=s.empcode && v[l].compare(u.deps[s.dep_index].stuList[j].interests[k])==0)
					{
						possibleFriends.push_back(u.deps[s.dep_index].stuList[j].global_id);
					flag=1;
				}
				}
			}
		}

	if (possibleFriends.size() == 0) {
		return 0;
	}
	return possibleFriends[rand()%possibleFriends.size()];
}

/*! \fn void generateFriends(vector<University> u, vector<bool> is_generate, int &StudentCounter)
\param u The vector of universities belonging to the social network.
\param is_generate The boolean vector indicating whether a friendship request is generated within a university.
\param StudentCounter The global counter for total number of students in the social network.

Thread generateFriends generates friending activities based on an exponential distribution with rate 
req_per_minute, r. It also logs all friendship requests and whether they were accepted.
*/
void generateFriends(vector<University> u, vector<bool> is_generate, int StudentCounter)
{	 
	int TotalStudents=StudentCounter-1;
	//int TotalFaculty=-1*FacultyCounter-1;
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
  		
  		if ((double)((rand()%100)+1)<=100.0*u[iter].openness)     
  			//request to someone outside
  			{ 
  				int localNum= (rand() % (TotalStudents-u[iter].uniStudents))+1;
			  						  						
  				struct Student frequest_receiverS;
  				frequest_receiverS=getStudentReceiver(u, iter, localNum);
  				receiverUni=frequest_receiverS.uni_index;
  				global_id2=frequest_receiverS.global_id;
  						
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
  			
  			{	
  			int condNum=(rand()%100)+1, receiver_id=0;
  			
  			char result[100]=""; // enough to hold all numbers up to 64-bits
  			if (condNum<=25)
  				{ //select random person sharing course
  					
  					receiver_id=getCourseFriend(u[iter], frequest_senderS);
  				
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
  				
  					receiver_id=getInterestFriend(u[iter], frequest_senderS);
  				
  				}
  				
  				if (receiver_id != 0) {

  				if ((double)(rand() % 100) < 100.0*u[iter].friendliness)
  				{	//write to file friendship request between friend_request_generator and receiver_id accepted
					sprintf(result, "%d %d %d %d 1", global_id1, iter, receiver_id, iter);
  				}
  				else
  				{
  					//write to file friendship request between friend_request_generator and receiver_id rejected
  					sprintf(result, "%d %d %d %d 0", global_id1, iter, receiver_id, iter);
  				}
  			
  				outfile.open("friend_log.txt", ios_base::app);
  				outfile << result; 
  			
  				outfile<<"\n";
  				outfile.close();
  			}
  			}
  		
		}
	
	}
	
}
