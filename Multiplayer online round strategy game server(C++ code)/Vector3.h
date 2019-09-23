#ifndef _VECTOR3
#define _VECTOR3

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

class Vector3
{
public:
	Vector3();
	Vector3(float NewX, float NewY, float NewZ);
	Vector3(string Vector3String);
	Vector3& operator=(Vector3 &V);
	~Vector3();

	float x = 0;
	float y = 0;
	float z = 0;

private:

};

Vector3::Vector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

inline Vector3::Vector3(float NewX, float NewY, float NewZ)
{
	this->x = NewX;
	this->y = NewY;
	this->z = NewZ;
}

inline Vector3::Vector3(string Vector3String)
{
	istringstream iss(Vector3String);
	string temp;

	getline(iss, temp, ' ');
	this->x = stof(temp);
	getline(iss, temp, ' ');
	this->y = stof(temp);
	getline(iss, temp, ' ');
	this->z = stof(temp);
}

inline Vector3 & Vector3::operator=(Vector3 & V)
{
	this->x = V.x;
	this->y = V.y;
	this->z = V.z;
	return *this;
}

Vector3::~Vector3()
{
}

#endif