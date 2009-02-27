#ifndef _MATRIX_H_
#define _MATRIX_H_

//
// originally implemented by Justin Legakis
//

#include <math.h>
#include <assert.h>

#include "vectors.h"

// ====================================================================
// ====================================================================

class Matrix {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Matrix() { Clear(); }
  Matrix(const Matrix& m);
  Matrix& operator=(const Matrix& m);
  ~Matrix() {}
  
  // ACCESSORS
  double Get(int row, int col) const { 
    assert (row >= 0 && row < 4);
    assert (col >= 0 && col < 4);
    return data[row+col*4]; }
  const float* glGet(void) {
    for (int i = 0; i < 16; i++) {
      float_data[i] = (float)data[i];
    }
    return float_data; }
  
  // MODIFIERS
  void Set(int row, int col, double v) {
    assert (row >= 0 && row < 4);
    assert (col >= 0 && col < 4);
    data[row+col*4] = v; }
  void SetToIdentity();
  void Clear();
  
  void Transpose(Matrix &m) const;
  void Transpose() { Transpose(*this); }

  int Inverse(Matrix &m, double epsilon = 1e-08) const;
  int Inverse(double epsilon = 1e-08) { return Inverse(*this,epsilon); }

  // OVERLOADED OPERATORS
  friend Matrix operator+(const Matrix &m1, const Matrix &m2);
  friend Matrix operator-(const Matrix &m1, const Matrix &m2);
  friend Matrix operator*(const Matrix &m1, const Matrix &m2);
  friend Matrix operator*(const Matrix &m1, double f);
  friend Matrix operator*(double f, const Matrix &m) { return m * f; }
  Matrix& operator+=(const Matrix& m) { *this = *this + m; return *this; }
  Matrix& operator-=(const Matrix& m) { *this = *this - m; return *this; }
  Matrix& operator*=(const double f)   { *this = *this * f; return *this; }
  Matrix& operator*=(const Matrix& m) { *this = *this * m; return *this; }

  // TRANSFORMATIONS
  static Matrix MakeTranslation(const Vec3f &v);
  static Matrix MakeScale(const Vec3f &v);
  static Matrix MakeScale(double s) { return MakeScale(Vec3f(s,s,s)); }
  static Matrix MakeXRotation(double theta);
  static Matrix MakeYRotation(double theta);
  static Matrix MakeZRotation(double theta);
  static Matrix MakeAxisRotation(const Vec3f &v, double theta);
  
  // Use to transform a point with a matrix
  // that may include translation
  void Transform(Vec4f &v) const;
  void Transform(Vec3f &v) const {
    Vec4f v2 = Vec4f(v.x(),v.y(),v.z(),1);
    Transform(v2);
    v.Set(v2.x()/v2.w(),v2.y()/v2.w(),v2.z()/v2.w()); }
  void Transform(Vec2f &v) const {
    Vec4f v2 = Vec4f(v.x(),v.y(),1,1);
    Transform(v2);
    v.Set(v2.x()/v2.w(),v2.y()/v2.w()); }

  // Use to transform the direction of the ray
  // (ignores any translation)
  void TransformDirection(Vec3f &v) const {
    Vec4f v2 = Vec4f(v.x(),v.y(),v.z(),0);
    Transform(v2);
    v.Set(v2.x(),v2.y(),v2.z()); }

  // INPUT / OUTPUT
  friend ostream& operator<<(ostream &ostr, const Matrix &m);
  friend istream& operator>>(istream &istr, Matrix &m);
  
  static double det4x4(double a1, double a2, double a3, double a4, 
		      double b1, double b2, double b3, double b4, 
		      double c1, double c2, double c3, double c4, 
		      double d1, double d2, double d3, double d4);
  static double det3x3(double a1,double a2,double a3,
		      double b1,double b2,double b3,
		      double c1,double c2,double c3);
  static double det2x2(double a, double b,
		      double c, double d);



private:

  // REPRESENTATION 
  // column-major order
  double data[16];
  float float_data[16];
};

// ====================================================================
// ====================================================================

#endif
