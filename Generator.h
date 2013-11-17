#ifndef GENERATOR_H
#define GENERATOR_H


#include <stdio.h>           
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>



//Module containing the network specifications.
using namespace std;
//int FacultyCounter;
//int StudentCounter;


struct Course{
	string name;
	string dep;
	double frequency;
	bool isFloated;

	//Constructor
	Course(string s, string d, double f){
		name.assign(s);
		dep.assign(d);
		frequency = f;isFloated = false;
	}
};

struct Interest{
	string name;
	double pop;

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


char *randomString( int maxLen, int seed );

#endif
