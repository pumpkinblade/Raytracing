#ifndef MATRIX_3_H
#define MATRIX_3_H

#include "rtbase.h"

struct Matrix3
{
    Matrix3();
    Matrix3(double d);
    Matrix3(double mm[3][3]);
    Matrix3(double m00, double m01, double m02,
            double m10, double m11, double m12,
            double m20, double m21, double m22);

    const double &operator()(int i, int j) const { return m[i][j]; }
    double &operator()(int i, int j) { return m[i][j]; }

    static Matrix3 Transpose(const Matrix3 &m);
    static Matrix3 Inverse(const Matrix3 &m);

    double m[3][3];
};

Matrix3 operator*(const Matrix3 &m1, const Matrix3 &m2);
Vector3 operator*(const Matrix3 &m, const Vector3 &v);

#endif