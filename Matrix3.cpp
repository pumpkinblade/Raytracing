#include "Matrix3.h"
#include <cstring>

Matrix3::Matrix3() : Matrix3(0.0)
{
}

Matrix3::Matrix3(double a)
{
    m[0][0] = a; m[0][1] = 0; m[0][2] = 0;
    m[1][0] = 0; m[1][1] = a; m[1][2] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = a;
}

Matrix3::Matrix3(double m00, double m01, double m02,
                 double m10, double m11, double m12,
                 double m20, double m21, double m22)
{
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

Matrix3::Matrix3(double mm[3][3])
{
    std::memcpy(m, mm, 9*sizeof(double));
}

Matrix3 Matrix3::Transpose(const Matrix3& m)
{
    return Matrix3(
        m(0,0), m(1,0), m(2,0),
        m(0,1), m(1,1), m(2,1),
        m(0,2), m(1,2), m(2,2)
    );
}

Matrix3 Matrix3::Inverse(const Matrix3& m)
{
    Matrix3 minv = m;
    bool hasp[3] = {false, false, false};
    int sw1[3], sw2[3];
    for(int l = 0; l < 3; l++)
    {
        // 寻找主元
        double pivot = 0.0;
        int posrow, poscol;
        for(int i = 0; i < 3; i++)
        {
            if(!hasp[i])
            {
                for(int j = 0; j < 3; j++)
                {
                    if(!hasp[j] && std::abs(minv(i, j)) > pivot)
                    {
                        pivot = std::abs(minv(i, j));
                        posrow = i;
                        poscol = j;
                    }
                }
            }
        }

        // 把主元移到对角线上
        if(posrow != poscol)
            for(int k = 0; k < 3; k++)
                std::swap(minv(posrow, k), minv(poscol, k));

        // 主元所在行除以主元
        double pivot_inv = 1.0 / minv(poscol, poscol);
        minv(poscol, poscol) = 1.0;
        for(int k = 0; k < 3; k++)
            minv(poscol, k) *= pivot_inv;
        
        // 行变换使得主元所在列其他元素为0
        for(int i = 0; i < 3; i++)
        {
            if(i != poscol)
            {
                double tmp = minv(i, poscol);
                minv(i, poscol) = 0.0;
                for(int k = 0; k < 3; k++)
                {
                    minv(i, k) -= tmp * minv(poscol, k);
                }
            }
        }

        sw1[l] = posrow;
        sw2[l] = poscol;
        hasp[poscol] = true;
    }

    // 调换列
    for(int l = 2; l >= 0; l--)
    {
        if(sw1[l] != sw2[l])
            for(int k = 0; k < 3; k++)
                std::swap(minv(k, sw1[l]), minv(k, sw2[l]));
    }
    return minv;
}

Matrix3 operator*(const Matrix3& m1, const Matrix3& m2)
{
    Matrix3 m3(0.0);
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            for(int k = 0; k < 3; k++)
                m3(i, j) += m1(i, k) * m2(k, j);
    return m3;
}

Vector3 operator*(const Matrix3& m, const Vector3& v)
{
    Vector3 vv(0.0, 0.0, 0.0);
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
                vv(i) += m(i, j) * v(j);
    return vv;
}
