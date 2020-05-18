#include "pch.h"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cassert>
#include <algorithm>


template< int n, int m, class T >
class Matrix;
template< int n, int m, class T>
class MatrixSub;


template<int n, int m, class T >
class MatrixSub
{
	typedef Matrix<n, m, T> MatrTy;
public:
	MatrixSub(MatrTy &_origin, int r1, int r2, int c1, int c2) :
		origin(_origin),
		r1(r1), r2(r2), c1(c1), c2(c2)
	{
		assert(0 <= r1 && r1 <= r2 && r2 < n);
		assert(0 <= c1 && c1 <= c2 && c2 < m);
	}

	~MatrixSub() = default;

	template <int N, int M>
	MatrixSub operator= (const Matrix<N, M, T> &other);

	int r1, r2, c1, c2;
	MatrTy &origin;
};


template< int n, int m, class T >
class Matrix
{
	typedef std::vector<T> raw_ty;
	typedef std::vector< std::vector<T> > matrix_ty;
public:
	Matrix()
	{
		static_assert(std::is_arithmetic<T>::value, "non-arithmetic type");
		data = matrix_ty(n);
		for (auto &it : data)
		{
			it = raw_ty(m, 0);
		}
	}

	~Matrix()
	{
		for (auto &raw : data)
		{
			raw.clear();
			raw.shrink_to_fit();
		}
		data.clear();
		data.shrink_to_fit();
	}

	Matrix& operator= (const Matrix<n, m, T> &other) = default;


	const raw_ty &operator[] (int i) const
	{
		assert(i < n && i >= 0 && "wrong index");
		return data[i];
	}
	raw_ty &operator[] (int i)
	{
		assert(i < n && i >= 0 && "wrong index");
		return data[i];
	}


	void get(std::istream &is)
	{
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				is >> data[i][j];
			}
		}
	}
	void print(std::ostream &os) const
	{
		for (const auto &Iti : data)
		{
			os << "| ";
			for (const auto &Itj : Iti)
			{
				if (std::is_floating_point<T>::value)
				{
					os.precision(2);
				}
				os << Itj << "\t";
			}
			os << " |" << std::endl;
		}
		os << std::endl;
	}


	Matrix operator+ (const Matrix<n, m, T> &other)
	{
		Matrix<n, m, T> newMatr;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				newMatr[i][j] = data[i][j] + other[i][j];
			}
		}
		return std::move(newMatr);
	}

	template <int k>
	Matrix<n, k, T> operator* (const Matrix<m, k, T> &other)
	{
		Matrix<n, k, T> newMatr;
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < k; j++)
			{
				for (int s = 0; s < m; s++)
				{
					newMatr[i][j] += data[i][s] * other[s][j];
				}
			}
		}
		return std::move(newMatr);
	}


	template<int k>
	Matrix<n + k, m, T> operator& (const Matrix<k, m, T> &other)
	{
		Matrix<n + k, m, T> newMatr;
		std::copy(this->data.begin(), this->data.end(), newMatr.data.begin());
		std::copy(other.data.begin(), other.data.end(), newMatr.data.begin() + n);
		return std::move(newMatr);
	}

	template<int k>
	Matrix<n, m + k, T> operator| (const Matrix<n, k, T> &other)
	{
		Matrix<n, m + k, T> newMatr;
		for (int i = 0; i < n; i++)
		{
			std::copy(data[i].begin(), data[i].end(), newMatr[i].begin());
			std::copy(other[i].begin(), other[i].end(), newMatr[i].begin() + m);
		}
		return std::move(newMatr);
	}


	template<int N, int M>
	Matrix& operator= (const MatrixSub<N, M, T> &other)
	{
		assert(other.r2 - other.r1 + 1 == n);
		assert(other.c2 - other.c1 + 1 == m);
		for (int i = 0; i < n; i++)
		{
			std::copy(other.origin[i + other.r1].begin() + other.c1,
				other.origin[i + other.r1].begin() + other.c2 + 1,
				data[i].begin());
		}
		return *this;
	}

	MatrixSub<n, m, T> operator() (int r1, int r2, int c1, int c2)
	{
		return std::move(MatrixSub<n, m, T>(*this, r1, r2, c1, c2));
	}


	matrix_ty data;
};



template<int n, int m, class T>
template<int N, int M>
MatrixSub<n, m, T> 
MatrixSub<n, m, T>::operator= (const Matrix<N, M, T> &other)
{
	assert(r2 - r1 + 1 == N && c2 - c1 + 1 == M);
	for (int i = r1; i <= r2; i++)
	{
		std::copy(other[i - r1].begin(),
				  other[i - r1].end(),
				  origin[i].begin() + c1);
	}
	return *this;
}




int main()
{
	Matrix<3, 2, int> A;
	Matrix<3, 2, int> B;
	Matrix<2, 3, int> C;
	Matrix<2, 2, int> D;

	D[0][0] = 5; 
	D[1][1] = 6;
	C[1][1] = C[0][0] = 2;
	A[0][0] = 3;
	B = A;
	B[0][1] = 2;

	B = A + B;

	Matrix<3, 3, int> Prod = B * C;
	Matrix<3, 5, int> concCol = B | Prod;
	Matrix<5, 2, int> concRaw = B & D;
	Matrix<2, 2, int> tmp;
	tmp = Prod(0, 1, 1, 2);
	
	std::cout << "B\n";
	B.print(std::cout);

	std::cout << "product\n";
	Prod.print(std::cout);

	std::cout << "concat columns\n";
	concCol.print(std::cout);

	std::cout << "concat raws\n";
	concRaw.print(std::cout);

	std::cout << "assign submatrix\n";
	Prod(1, 2, 1, 2) = D;
	Prod.print(std::cout);

	std::cout << "extraction\n";
	tmp.print(std::cout);
	

    return EXIT_SUCCESS; 
}
