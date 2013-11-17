/*! \file generateFaculty.cpp
A file containing necessary code for running the thread generateFaculty.cpp
*/
#include "generateFaculty.h"
#include <string.h>

using namespace std;

/*! \fn void generateFaculty(vector<University> &u, int &FacultyCounter)
\param u The vector of universities belonging to the social network.
\param FacultyCounter The global counter for total number of faculty in the social network.

Thread generateFaculty, creates faculty nodes. All faculty members are generated at time 0, at the beginning of 
simulation. Each generated faculty member has a sequentially increasing employee code, and a random 
name, department, house, and set of interests.
*/
void generateFaculty(vector<University> &u, int &FacultyCounter)
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

	for (i=0;i<numDept;i++)
		{ numFacInDept=u[iter].deps[i].faculty;
		int numInterest=u[iter].deps[i].interests.size();
			for (j=0; j<numFacInDept; j++)
			{   u[iter].uniFaculty+=1;
				struct Faculty temp;
				temp.empcode=code+1;	
			
				temp.name=randomString(20,j);
			
				int houseNum=(rand() % numHouse);
				temp.house=u[iter].houselocality[houseNum];
			
				temp.global_id=FacultyCounter;
			
				FacultyCounter-=1;
				temp.uni_index=iter;
			
				temp.dep_index=i;	
			
				for (k=0; k<numInterest; k++)
					{
						bool isInterest=(double)(rand() % 100)<=100.0*u[iter].deps[i].interests[k].pop;
						if (isInterest==1)
							temp.interests.push_back(u[iter].deps[i].interests[k].name); 

					}				
				code+=1;
				u[iter].deps[i].facList.push_back(temp);
			}
		}
}
}

