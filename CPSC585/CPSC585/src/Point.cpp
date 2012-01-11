/*
*  Point class, this class allows the formation of curves and surfaces in 2D and/or 3D space
*   each point can also be easily outputted or inputted from a file with the fstream functions
*   which will output or input all 3 coordinates to a text file that the user will specify
*
*  The fstream operator overload code was burrowed from the previous TA's program 2D Point Picking
*   located at http://pages.cpsc.ucalgary.ca/~slongay/pmwiki-2.2.1/pub/CPSC453/Picking.zip
*
*  The crossProduct and normal calculation functions were burrowed from the TA'S webite located at:
*   http://pages.cpsc.ucalgary.ca/~tfalders/CPSC453/2010-10-21/main.cpp
* 
*  Author: Jeffrey Lam
*  Email: jcmlam@ucalgary.ca
*/

#include <iostream>
#include <fstream>
#include "Point.h"

//using namespace std;

// handles output of data from a file
std::ofstream& operator<< (std::ofstream& output, const Point& p)
{
	output << p.x << ' ' << p.y << ' ' << p.z << "\n";
	return output;
}

// handles input of data from a file
std::ifstream& operator>>(std::ifstream& input, Point& p)
{
	input >> p.x >> p.y >> p.z;
	return input;
}

// default constructor that creates a point at (0, 0, 0)
Point::Point()
{
	x = 0.0;
	y = 0.0;
	z = 0.0;
}

// constructor for 2D points
Point::Point(double a, double b)
{
	x = a;
	y = b;
	z = 0.0;
}

// contructor for 3D points
Point::Point(double a, double b, double c)
{
	x = a;
	y = b;
	z = c;
}

// overload = operator, allows assigning points with other points
Point& Point::operator= (const Point &a)
{
	x = a.x;
	y = a.y;
	z = a.z;

	return *this;
}

// overload == operater, checks whether or not 2 points/vectors are the same.
bool Point::operator == (const Point &p) const {
        return (  (x == p.x)
               && (y == p.y)
               && (z == p.z)
               );
}

// overload + operator, allows adding points together
Point Point::operator+ (const Point &a) const
{
	return Point (x+a.x, y+a.y, z+a.z);
}

// overload - operator, allows subtracting points together
Point Point::operator- (const Point &a) const
{
	return Point (x-a.x, y-a.y, z-a.z);
}

// overload * operator, scaling operator
Point Point::operator* (double n) const
{
	return Point (x*n, y*n, z*n);
}

// overload / operator, also a scaling operator
Point Point::operator/ (double n) const
{
	return Point (x/n, y/n, z/n);
}

// Calculates the magnitude of a vector
double Point::mag()
{
        return sqrt (x*x + y*y + z*z);
}

// calculate the distance from this point to point a
double Point::dist(Point &a)
{
    double d = ((a.x - x)*(a.x - x)) + ((a.y - y)*(a.y - y)) + ((a.z - z)*(a.z - z));
    return sqrt(d);
}

// Calculates the cross product of 2 vectors
Point crossProduct(Point &v1, Point &v2) 
{

	Point result;
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	return result;
}

// compute the dot product of 2 vectors
double dotProduct(Point &v1, Point &v2)
{
    double result;
    
    result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    
    return result;
}

// computes and returns the angle of 2 vectors in degrees
double angle(Point &v1, Point &v2)
{
    Point nv1 = v1/v1.mag();
    Point nv2 = v2/v2.mag();
    double radian;

    radian = acos(dotProduct(nv1, nv2));

    return radian*180.0/PI;
}


