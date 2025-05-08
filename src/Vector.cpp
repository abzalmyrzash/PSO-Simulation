#include "Vector.hpp"

Vector::Vector(){

}

Vector::Vector(double x, double y){
	this->x = x;
	this->y = y;
}

Vector Vector::operator+(Vector v){
	return Vector(x + v.x, y + v.y);
}

Vector Vector::operator-(Vector v){	
	return Vector(x - v.x, y - v.y);
}

Vector Vector::operator*(double n){
	return Vector(x * n, y * n);
}

double Vector::operator*(Vector v){
	return x * v.x + y * v.y;
}

Vector Vector::operator/(double n){
	return Vector(x / n, y / n);
}

double Vector::len(){
	return sqrt(x*x + y*y);
}

double Vector::angle(){
	return (y > 0 ? 1 : -1) * acos(x / len());
}

double dist(Vector v1, Vector v2){
	return (v1 - v2).len();
}

