#pragma once

#include <cmath>

class Vector{
public:
	double x;
	double y;

	Vector();
	Vector(double x, double y);
	Vector operator+(Vector v);
	Vector operator-(Vector v);
	Vector operator*(double n);
	double operator*(Vector v);
	Vector operator/(double n);
	double len();
	double angle();
};

double dist(Vector v1, Vector v2);