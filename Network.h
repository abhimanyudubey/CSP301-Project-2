#ifndef NETWORK_H
#define NETWORK_H


#include <stdio.h>           
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include "Generator.h"

class Network{
public:
	vector<University> university;
	double faculty_rand,student_rand,course_rand,friend_rand;

	void setEnvironment(char *filename);
	
	Network();
	
	
	
};

#endif
