#ifndef _VECTORS_H_
#define _VECTORS_H_

//
// originally implemented by Justin Legakis
//

#include <iostream>
using namespace std;

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

class Matrix;

// ====================================================================
// ====================================================================

class Vec2f {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Vec2f() { data[0] = data[1] = 0; }
  Vec2f(const Vec2f &V) {
    data[0] = V.data[0];
    data[1] = V.data[1]; }
  Vec2f(double d0, double d1) {
    data[0] = d0;
    data[1] = d1; }
  Vec2f(const Vec2f &V1, const Vec2f &V2) {
    data[0] = V1.data[0] - V2.data[0];
    data[1] = V1.data[1] - V2.data[1]; }
  ~Vec2f() { }

  // ACCESSORS
  void Get(double &d0, double &d1) const {
    d0 = data[0];
    d1 = data[1]; }
  double operator[](int i) const { 
    assert (i >= 0 && i < 2); 
    return data[i]; }
  double x() const { return data[0]; }
  double y() const { return data[1]; }
  double Length() const {
    return (double)sqrt( data[0] * data[0] + 
			data[1] * data[1] ); }

  // MODIFIERS
  void Set(double d0, double d1) {
    data[0] = d0;
    data[1] = d1; }
  void Scale(double d0, double d1) {
    data[0] *= d0;
    data[1] *= d1; }
  void Divide(double d0, double d1) {
    data[0] /= d0;
    data[1] /= d1; }
  void Negate() {
    data[0] = -data[0];
    data[1] = -data[1]; }

  // OVERLOADED OPERATORS
  Vec2f& operator=(const Vec2f &V) {
    data[0] = V.data[0];
    data[1] = V.data[1];
    return *this; }
  int operator==(const Vec2f &V) const {
    return ((data[0] == V.data[0]) &&
	    (data[1] == V.data[1])); }
  int operator!=(const Vec2f &V) {
    return ((data[0] != V.data[0]) ||
	    (data[1] != V.data[1])); }
  Vec2f& operator+=(const Vec2f &V) {
    data[0] += V.data[0];
    data[1] += V.data[1];
    return *this; }
  Vec2f& operator-=(const Vec2f &V) {
    data[0] -= V.data[0];
    data[1] -= V.data[1];
    return *this; }
  Vec2f& operator*=(double f) {
    data[0] *= f;
    data[1] *= f;
    return *this; }
  Vec2f& operator/=(double f) {
    data[0] /= f;
    data[1] /= f;
    return *this; }
  
  // OPERATIONS
  double Dot2(const Vec2f &V) const {
    return data[0] * V.data[0] + data[1] * V.data[1] ; }

  // STATIC OPERATIONS
  static void Add(Vec2f &a, const Vec2f &b, const Vec2f &c ) {
    a.data[0] = b.data[0] + c.data[0];
    a.data[1] = b.data[1] + c.data[1]; }
  static void Sub(Vec2f &a, const Vec2f &b, const Vec2f &c ) {
    a.data[0] = b.data[0] - c.data[0];
    a.data[1] = b.data[1] - c.data[1]; }
  static void CopyScale(Vec2f &a, const Vec2f &b, double c ) {
    a.data[0] = b.data[0] * c;
    a.data[1] = b.data[1] * c; }
  static void AddScale(Vec2f &a, const Vec2f &b, const Vec2f &c, double d ) {
    a.data[0] = b.data[0] + c.data[0] * d;
    a.data[1] = b.data[1] + c.data[1] * d; }
  static void Average(Vec2f &a, const Vec2f &b, const Vec2f &c ) {
    a.data[0] = (b.data[0] + c.data[0]) * 0.5f;
    a.data[1] = (b.data[1] + c.data[1]) * 0.5f; }
  static void WeightedSum(Vec2f &a, const Vec2f &b, double c, const Vec2f &d, double e ) {
    a.data[0] = b.data[0] * c + d.data[0] * e;
    a.data[1] = b.data[1] * c + d.data[1] * e; }

  // INPUT / OUTPUT
  friend ostream& operator<< (ostream &ostr, const Vec2f &v) {
    ostr << v.data[0] << " " << v.data[1] << endl;
    return ostr; }

private:

  // REPRESENTATION
  double		data[2];
  
};
// ====================================================================
// ====================================================================

class Vec3f {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Vec3f() { data[0] = data[1] = data[2] = 0; }
  Vec3f(const Vec3f &V) {
    data[0] = V.data[0];
    data[1] = V.data[1];
    data[2] = V.data[2]; }
  Vec3f(double d0, double d1, double d2) {
    data[0] = d0;
    data[1] = d1;
    data[2] = d2; }
  Vec3f(const Vec3f &V1, const Vec3f &V2) {
    data[0] = V1.data[0] - V2.data[0];
    data[1] = V1.data[1] - V2.data[1];
    data[2] = V1.data[2] - V2.data[2]; }
  ~Vec3f() { }

  // ACCESSORS
  void Get(double &d0, double &d1, double &d2) const {
    d0 = data[0];
    d1 = data[1];
    d2 = data[2]; }
  double operator[](int i) const { 
    assert (i >= 0 && i < 3); 
    return data[i]; }
  double x() const { return data[0]; }
  double y() const { return data[1]; }
  double z() const { return data[2]; }
  double r() const { return data[0]; }
  double g() const { return data[1]; }
  double b() const { return data[2]; }
  void setx(double x) { data[0]=x; }
  void sety(double y) { data[1]=y; }
  void setz(double z) { data[2]=z; }
  double Length() const {
    return (double)sqrt( data[0] * data[0] +
			data[1] * data[1] +
			data[2] * data[2] ); }

  // MODIFIERS
  void Set(double d0, double d1, double d2) {
    data[0] = d0;
    data[1] = d1;
    data[2] = d2; }
  void Scale(double d0, double d1, double d2) {
    data[0] *= d0;
    data[1] *= d1;
    data[2] *= d2; }
  void Divide(double d0, double d1, double d2) {
    data[0] /= d0;
    data[1] /= d1;
    data[2] /= d2; }
  void Normalize() {
    double l = Length();
    if (l > 0) {
      data[0] /= l;
      data[1] /= l;
      data[2] /= l; }}
  void Negate() {
    data[0] = -data[0];
    data[1] = -data[1];
    data[2] = -data[2]; }
  void Clamp(double low = 0, double high = 1) {
    if (data[0] < low) data[0] = low;  if (data[0] > high) data[0] = high;
    if (data[1] < low) data[1] = low;  if (data[1] > high) data[1] = high;
    if (data[2] < low) data[2] = low;  if (data[2] > high) data[2] = high; }

  // OVERLOADED OPERATORS
  Vec3f& operator=(const Vec3f &V) {
    data[0] = V.data[0];
    data[1] = V.data[1];
    data[2] = V.data[2];
    return *this; }
  int operator==(const Vec3f &V) {
    return ((data[0] == V.data[0]) &&
	    (data[1] == V.data[1]) &&
	    (data[2] == V.data[2])); }
  int operator!=(const Vec3f &V) {
    return ((data[0] != V.data[0]) ||
	    (data[1] != V.data[1]) ||
	    (data[2] != V.data[2])); }
  Vec3f& operator+=(const Vec3f &V) {
    data[0] += V.data[0];
    data[1] += V.data[1];
    data[2] += V.data[2];
    return *this; }
  Vec3f& operator-=(const Vec3f &V) {
    data[0] -= V.data[0];
    data[1] -= V.data[1];
    data[2] -= V.data[2];
    return *this; }
  Vec3f& operator*=(int i) {
    data[0] = double(data[0] * i);
    data[1] = double(data[1] * i);
    data[2] = double(data[2] * i);
    return *this; }
  Vec3f& operator*=(double f) {
    data[0] *= f;
    data[1] *= f;
    data[2] *= f;
    return *this; }
  Vec3f& operator/=(int i) {
    data[0] = double(data[0] / i);
    data[1] = double(data[1] / i);
    data[2] = double(data[2] / i);
    return *this; }
  Vec3f& operator/=(double f) {
    data[0] /= f;
    data[1] /= f;
    data[2] /= f;
    return *this; }
  
  friend Vec3f operator+(const Vec3f &v1, const Vec3f &v2) { 
    Vec3f v3; Add(v3,v1,v2); return v3; } 
  friend Vec3f operator-(const Vec3f &v1) {
    Vec3f v3 = v1; v3.Negate(); return v3; }
  friend Vec3f operator-(const Vec3f &v1, const Vec3f &v2) {
    Vec3f v3; Sub(v3,v1,v2); return v3; } 
  friend Vec3f operator*(const Vec3f &v1, double f) {
    Vec3f v2; CopyScale(v2,v1,f); return v2; } 
  friend Vec3f operator*(double f, const Vec3f &v1) {
    Vec3f v2; CopyScale(v2,v1,f); return v2; } 
  friend Vec3f operator*(const Vec3f &v1, const Vec3f &v2) {
    Vec3f v3; Mult(v3,v1,v2); return v3; }
         
  // OPERATIONS
  double Dot3(const Vec3f &V) const {
    return data[0] * V.data[0] +
      data[1] * V.data[1] +
      data[2] * V.data[2] ; }

  // STATIC OPERATIONS
  static void Add(Vec3f &a, const Vec3f &b, const Vec3f &c ) {
    a.data[0] = b.data[0] + c.data[0];
    a.data[1] = b.data[1] + c.data[1];
    a.data[2] = b.data[2] + c.data[2]; }
  static void Sub(Vec3f &a, const Vec3f &b, const Vec3f &c ) {
    a.data[0] = b.data[0] - c.data[0];
    a.data[1] = b.data[1] - c.data[1];
    a.data[2] = b.data[2] - c.data[2]; }
  static void Mult(Vec3f &a, const Vec3f &b, const Vec3f &c ) {
    a.data[0] = b.data[0] * c.data[0];
    a.data[1] = b.data[1] * c.data[1];
    a.data[2] = b.data[2] * c.data[2]; }
  static void CopyScale(Vec3f &a, const Vec3f &b, double c ) {
    a.data[0] = b.data[0] * c;
    a.data[1] = b.data[1] * c;
    a.data[2] = b.data[2] * c; }
  static void AddScale(Vec3f &a, const Vec3f &b, const Vec3f &c, double d ) {
    a.data[0] = b.data[0] + c.data[0] * d;
    a.data[1] = b.data[1] + c.data[1] * d;
    a.data[2] = b.data[2] + c.data[2] * d; }
  static void Average(Vec3f &a, const Vec3f &b, const Vec3f &c ) {
    a.data[0] = (b.data[0] + c.data[0]) * 0.5f;
    a.data[1] = (b.data[1] + c.data[1]) * 0.5f;
    a.data[2] = (b.data[2] + c.data[2]) * 0.5f; }
  static void WeightedSum(Vec3f &a, const Vec3f &b, double c, const Vec3f &d, double e ) {
    a.data[0] = b.data[0] * c + d.data[0] * e;
    a.data[1] = b.data[1] * c + d.data[1] * e;
    a.data[2] = b.data[2] * c + d.data[2] * e; }
  static void Cross3(Vec3f &c, const Vec3f &v1, const Vec3f &v2) {
    double x = v1.data[1]*v2.data[2] - v1.data[2]*v2.data[1];
    double y = v1.data[2]*v2.data[0] - v1.data[0]*v2.data[2];
    double z = v1.data[0]*v2.data[1] - v1.data[1]*v2.data[0];
    c.data[0] = x; c.data[1] = y; c.data[2] = z; }

  static void Min(Vec3f &a, const Vec3f &b, const Vec3f &c ) {
    a.data[0] = (b.data[0] < c.data[0]) ? b.data[0] : c.data[0];
    a.data[1] = (b.data[1] < c.data[1]) ? b.data[1] : c.data[1];
    a.data[2] = (b.data[2] < c.data[2]) ? b.data[2] : c.data[2]; }
  static void Max(Vec3f &a, const Vec3f &b, const Vec3f &c ) {
    a.data[0] = (b.data[0] > c.data[0]) ? b.data[0] : c.data[0];
    a.data[1] = (b.data[1] > c.data[1]) ? b.data[1] : c.data[1];
    a.data[2] = (b.data[2] > c.data[2]) ? b.data[2] : c.data[2]; }
  
  // INPUT / OUTPUT
  friend ostream& operator<< (ostream &ostr, const Vec3f &v) {
    ostr << v.data[0] << " " << v.data[1] << " " << v.data[2] << endl; 
    return ostr; }
  friend istream& operator>> (istream &istr, Vec3f &v) {
    istr >> v.data[0] >> v.data[1] >> v.data[2];
    return istr; }
  
private:

  friend class Matrix;

  // REPRESENTATION
  double		data[3];
  
};

// ====================================================================
// ====================================================================

class Vec4f {
  
public:

  // CONSTRUCTORS & DESTRUCTOR
  Vec4f() { data[0] = data[1] = data[2] = data[3] = 0; }
  Vec4f(const Vec4f &V) {
    data[0] = V.data[0];
    data[1] = V.data[1];
    data[2] = V.data[2];
    data[3] = V.data[3]; }
  Vec4f(double d0, double d1, double d2, double d3) {
    data[0] = d0;
    data[1] = d1;
    data[2] = d2;
    data[3] = d3; }
  Vec4f(const Vec3f &V, double w) {
    data[0] = V.x();
    data[1] = V.y();
    data[2] = V.z();
    data[3] = w; }
  Vec4f(const Vec4f &V1, const Vec4f &V2) {
    data[0] = V1.data[0] - V2.data[0];
    data[1] = V1.data[1] - V2.data[1];
    data[2] = V1.data[2] - V2.data[2];
    data[3] = V1.data[3] - V2.data[3]; }
  ~Vec4f() { }

  // ACCESSORS
  void Get(double &d0, double &d1, double &d2, double &d3) const {
    d0 = data[0];
    d1 = data[1];
    d2 = data[2];
    d3 = data[3]; }
  double operator[](int i) const { 
    assert (i >= 0 && i < 4); 
    return data[i]; }
  double x() const { return data[0]; }
  double y() const { return data[1]; }
  double z() const { return data[2]; }
  double w() const { return data[3]; }
  double r() const { return data[0]; }
  double g() const { return data[1]; }
  double b() const { return data[2]; }
  double a() const { return data[3]; }
  double Length() const {
    return (double)sqrt( data[0] * data[0] +
			data[1] * data[1] +
			data[2] * data[2] +
			data[3] * data[3] ); }

  // MODIFIERS
  void Set(double d0, double d1, double d2, double d3) {
    data[0] = d0;
    data[1] = d1;
    data[2] = d2;
    data[3] = d3; }
  void Scale(double d0, double d1, double d2, double d3) {
    data[0] *= d0;
    data[1] *= d1;
    data[2] *= d2;
    data[3] *= d3; }
  void Divide(double d0, double d1, double d2, double d3) {
    data[0] /= d0;
    data[1] /= d1;
    data[2] /= d2;
    data[3] /= d3; }
  void Negate() {
    data[0] = -data[0];
    data[1] = -data[1];
    data[2] = -data[2];
    data[3] = -data[3]; }
  void Normalize() {
    double l = Length();
    if (l > 0) {
      data[0] /= l;
      data[1] /= l;
      data[2] /= l; }}
  void DivideByW() {
    if (data[3] != 0) {
      data[0] /= data[3];
      data[1] /= data[3];
      data[2] /= data[3];
    } else {
      data[0] = data[1] = data[2] = 0; }
    data[3] = 1;}

  // OVERLOADED OPERATORS
  Vec4f& operator=(const Vec4f &V) {
    data[0] = V.data[0];
    data[1] = V.data[1];
    data[2] = V.data[2];
    data[3] = V.data[3];
    return *this; }
  int operator==(const Vec4f &V) const {
    return ((data[0] == V.data[0]) &&
	    (data[1] == V.data[1]) &&
	    (data[2] == V.data[2]) &&
	    (data[3] == V.data[3])); }
  int operator!=(const Vec4f &V) const {
    return ((data[0] != V.data[0]) ||
	    (data[1] != V.data[1]) ||
	    (data[2] != V.data[2]) ||
	    (data[3] != V.data[3])); }
  Vec4f& operator+=(const Vec4f &V) {
    data[0] += V.data[0];
    data[1] += V.data[1];
    data[2] += V.data[2];
    data[3] += V.data[3];
    return *this; }
  Vec4f& operator-=(const Vec4f &V) {
    data[0] -= V.data[0];
    data[1] -= V.data[1];
    data[2] -= V.data[2];
    data[3] -= V.data[3];
    return *this; }
  Vec4f& operator*=(double f) {
    data[0] *= f;
    data[1] *= f;
    data[2] *= f;
    data[3] *= f;
    return *this; }
  Vec4f& operator/=(double f) {
    data[0] /= f;
    data[1] /= f;
    data[2] /= f;
    data[3] /= f;
    return *this; }

  // OPERATIONS
  double Dot2(const Vec4f &V) const {
    return data[0] * V.data[0] +
      data[1] * V.data[1]; }
  double Dot3(const Vec4f &V) const {
    return data[0] * V.data[0] +
      data[1] * V.data[1] +
      data[2] * V.data[2]; }
  double Dot4(const Vec4f &V) const {
    return data[0] * V.data[0] +
      data[1] * V.data[1] +
      data[2] * V.data[2] +
      data[3] * V.data[3]; }
  
  // STATIC OPERATIONS
  static void Add(Vec4f &a, const Vec4f &b, const Vec4f &c ) {
    a.data[0] = b.data[0] + c.data[0];
    a.data[1] = b.data[1] + c.data[1];
    a.data[2] = b.data[2] + c.data[2];
    a.data[3] = b.data[3] + c.data[3]; }
  static void Sub(Vec4f &a, const Vec4f &b, const Vec4f &c ) {
    a.data[0] = b.data[0] - c.data[0];
    a.data[1] = b.data[1] - c.data[1];
    a.data[2] = b.data[2] - c.data[2];
    a.data[3] = b.data[3] - c.data[3]; }
  static void CopyScale(Vec4f &a, const Vec4f &b, double c ) {
    a.data[0] = b.data[0] * c;
    a.data[1] = b.data[1] * c;
    a.data[2] = b.data[2] * c;
    a.data[3] = b.data[3] * c; }
  static void AddScale(Vec4f &a, const Vec4f &b, const Vec4f &c, double d ) {
    a.data[0] = b.data[0] + c.data[0] * d;
    a.data[1] = b.data[1] + c.data[1] * d;
    a.data[2] = b.data[2] + c.data[2] * d;
    a.data[3] = b.data[3] + c.data[3] * d; }
  static void Average(Vec4f &a, const Vec4f &b, const Vec4f &c ) {
    a.data[0] = (b.data[0] + c.data[0]) * 0.5f;
    a.data[1] = (b.data[1] + c.data[1]) * 0.5f;
    a.data[2] = (b.data[2] + c.data[2]) * 0.5f;
    a.data[3] = (b.data[3] + c.data[3]) * 0.5f; }
  static void WeightedSum(Vec4f &a, const Vec4f &b, double c, const Vec4f &d, double e ) {
    a.data[0] = b.data[0] * c + d.data[0] * e;
    a.data[1] = b.data[1] * c + d.data[1] * e;
    a.data[2] = b.data[2] * c + d.data[2] * e;
    a.data[3] = b.data[3] * c + d.data[3] * e; }
  static void Cross3(Vec4f &c, const Vec4f &v1, const Vec4f &v2) {
    double x = v1.data[1]*v2.data[2] - v1.data[2]*v2.data[1];
    double y = v1.data[2]*v2.data[0] - v1.data[0]*v2.data[2];
    double z = v1.data[0]*v2.data[1] - v1.data[1]*v2.data[0];
    c.data[0] = x; c.data[1] = y; c.data[2] = z; }

  // INPUT / OUTPUT
  friend ostream& operator<< (ostream &ostr, const Vec4f &v) {
    ostr << v.data[0] << " " << v.data[1] << " " << v.data[2] << " " << v.data[3] << endl; 
    return ostr; }

private:

  friend class Matrix;

  // REPRESENTATION
  double		data[4];
  
};

// ====================================================================
// ====================================================================

#endif

