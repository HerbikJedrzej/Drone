#pragma once
#include <utility>

template<unsigned int x, unsigned int y>
class Matrix{
    double val[x][y];
public:
    Matrix();
    Matrix(const double val[x][y]);
    Matrix(Matrix<x,y>&);
    Matrix(Matrix<x,y>&&) = default;
    Matrix& operator=(const Matrix&) = default;
    double get(unsigned int _x, unsigned int _y) const;
    double& operator()(unsigned int _x, unsigned int _y);
};

template<unsigned int x, unsigned int n, unsigned int y>
Matrix<x,y> multiplication(const Matrix<x,n>& m1, const Matrix<n,y>& m2){
    Matrix<x,y> toReturn;
    for(unsigned int _x = 0; _x < x; _x++)
        for(unsigned int _y = 0; _y < y; _y++)
            for(unsigned int _n = 0; _n < n; _n++)
                toReturn(_x, _y) += m1.get(_x, _n) * m2.get(_n, _y);
    return std::move(toReturn);
}

template<unsigned int x, unsigned int y>
Matrix<x,y> transformation(const Matrix<y,x>& m){
    Matrix<x,y> toReturn;
    for(unsigned int _x = 0; _x < x; _x++)
        for(unsigned int _y = 0; _y < y; _y++)
            toReturn(_x, _y) += m.get(_y, _x);
    return std::move(toReturn);
}

template<unsigned int x, unsigned int y>
Matrix<x,y> addition(const Matrix<x,y>& m1, const Matrix<x,y>& m2){
    Matrix<x,y> toReturn;
    for(unsigned int _x = 0; _x < x; _x++)
        for(unsigned int _y = 0; _y < y; _y++)
            toReturn(_x, _y) += m1.get(_x, _y) + m2.get(_x, _y);
    return std::move(toReturn);
}

template<unsigned int x, unsigned int y>
Matrix<x,y> subtraction(const Matrix<x,y>& m1, const Matrix<x,y>& m2){
    Matrix<x,y> toReturn;
    for(unsigned int _x = 0; _x < x; _x++)
        for(unsigned int _y = 0; _y < y; _y++)
            toReturn(_x, _y) += m1.get(_x, _y) - m2.get(_x, _y);
    return std::move(toReturn);
}

template<unsigned int x, unsigned int y>
Matrix<x,y>::Matrix(const double _val[x][y]){
    for(unsigned int _x = 0; _x < x; _x++)
        for(unsigned int _y = 0; _y < y; _y++)
            val[_x][_y] = _val[_x][_y];
}

template<unsigned int x, unsigned int y>
Matrix<x,y>::Matrix(){
    for(unsigned int _x = 0; _x < x; _x++)
        for(unsigned int _y = 0; _y < y; _y++)
            val[_x][_y] = 0;
}

template<unsigned int x, unsigned int y>
double& Matrix<x,y>::operator()(unsigned int _x, unsigned int _y){
    return val[_x][_y];
}

template<unsigned int x, unsigned int y>
double Matrix<x,y>::get(unsigned int _x, unsigned int _y) const{
    return val[_x][_y];
}

template<unsigned int x, unsigned int y>
Matrix<x,y>::Matrix(Matrix<x,y>& m){
    for(unsigned int _x = 0; _x < x; _x++)
        for(unsigned int _y = 0; _y < y; _y++)
            val[_x][_y] = m(_x, _y);
}
