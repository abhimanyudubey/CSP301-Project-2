/*! \file generateCourses.cpp
A file containing necessary code for running the thread generateCourses.cpp
*/
#include "generateCourses.h"
#include <string.h>
#include <math.h>


using namespace std;

/*! \fn int* courseListGenerator(int M, int N)
A function to select M distinct and random courses from N total courses.
*/
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

/*! \fn void generateStudent(vector<University> &u, vector<int> &codes, int &StudentCounter)
\param u The vector of universities belonging to the social network.

Thread generateCourses creates courses “once every semester.” It generates courses and registers 
students. In other words it edits students' course data. It does not create any new nodes. It also registers students in a course – each student is registered for 
“semester_dept_courses” courses of their department and “semester_nondept_courses” courses of some 
other (randomly chosen) departments.
*/
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
		
		for (coreID=0; coreID<core_real; coreID++)
			{	
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

