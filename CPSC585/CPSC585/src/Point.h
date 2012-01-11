/*
*  Point class header file
*
*  Author: Jeffrey Lam
*  Email: jcmlam@ucalgary.ca
*/

#ifndef POINT_H
#define POINT_H

#include <math.h>
#include <fstream>
#include <iostream>

#define PI 3.14159265

//using namespace std;

class Point
{
private:
    friend std::ofstream& operator<<(std::ofstream&, const Point& p);
    friend std::ifstream& operator>>(std::ifstream&, Point& p);

public:
	double x, y, z;

	Point();
	Point(double a, double b);
	Point(double a, double b, double c);
	virtual ~Point(){};

	Point& operator= (const Point &a);

	bool operator == (const Point &p) const;

	Point operator+ (const Point &a) const;

	Point operator- (const Point &a) const;

	Point operator* (double n) const;

	Point operator/ (double n) const;

	double mag();
	double dist(Point &a);
};

Point crossProduct (Point &v1, Point &v2);

double dotProduct (Point &v1, Point &v2);

double angle(Point &v1, Point &v2);

#endif
