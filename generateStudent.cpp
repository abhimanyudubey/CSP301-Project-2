/*! \file generateStudent.cpp
A file containing necessary code for running the thread generateStudent.cpp
*/
#include "generateStudent.h"
#include <string.h>

using namespace std;

/*! \fn void generateStudent(vector<University> &u, vector<int> &codes, int &StudentCounter)
\param u The vector of universities belonging to the social network.
\param codes The vector to keep track of the last assigned student codes for all universities.
\param StudentCounter The global counter for total number of students in the social network.

Thread generateStudent generates Student nodes, each with a sequential entry no and randomly 
generated name, department, hostel and a set of interests. 
*/
void generateStudent(vector<University> &u, vector<int> &codes, int &StudentCounter)
{	
	for (int iter=0;iter<u.size(); iter++)
{
	int numDept=u[iter].deps.size();
	int numHostel=u[iter].hostels.size();
	
	int totalstudent=0, numStuInDept=0,i,j,k;

	for (i=0;i<numDept;i++)
		{
		 totalstudent+=u[iter].deps[i].students;
		}

	for (i=0;i<numDept;i++)
		{numStuInDept=u[iter].deps[i].students;
		int numInterest=u[iter].deps[i].interests.size();
			for (j=0; j<numStuInDept; j++)
			{   u[iter].uniStudents+=1;
				struct Student temp;
				temp.empcode=codes[iter]+1;
				
				temp.name=randomString(20, j);

				int hostelNum=(rand() % numHostel);
				temp.hostel=u[iter].hostels[hostelNum];
				
				temp.global_id=StudentCounter;
				
				StudentCounter+=1;
				temp.uni_index=iter;
				
				temp.dep_index=i;
				
				for (k=0; k<numInterest; k++)
					{
						bool isInterest=(double)(rand() % 100)<=100*u[iter].deps[i].interests[k].pop;
						if (isInterest==1)
							temp.interests.push_back(u[iter].deps[i].interests[k].name); 

					}				
				codes[iter]+=1;
				u[iter].deps[i].stuList.push_back(temp);
			}
		}
}
}
