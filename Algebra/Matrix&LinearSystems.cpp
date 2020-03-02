#include "pch.h"
#include <iostream>

using namespace std;

#define int long long

int GCD(int a, int b)
{
	if (b == 0)
		return a;
	return GCD(b, a%b);
}

struct rational
{
	rational(): nom(0), den(1)
	{	}
	rational(int n, int d = 1)
	{
		int tmp = GCD(n, d);
		nom = n / tmp;
		den = d / tmp;
		if (den < 0)
		{
			den = -den;
			nom = -nom;
		}
	}
	void get_int()
	{
		cin >> this->nom;
		this->den = 1;
	}

	bool operator==(const rational & other) const
	{
		return	(this->nom == other.nom && this->den == other.den) ||
				(this->nom == 0 && other.nom == 0);
	}
	bool operator==(int a) { return ((nom == a) && den == 1) || (nom == 0 && a == 0); }
	bool operator!=(const rational & other) { return !(*this == other);  }
	bool operator!=(int a) { return !(*this == a); }

	rational operator +(const rational & other) const { return rational(this->nom*other.den + other.nom*this->den, this->den*other.den); }
	rational operator+=(const rational & other)		 { return *this = *this + other; }
	rational operator -() { return rational(-this->nom, this->den); }
	rational operator -(rational & other) const { return *this + -other; }
	rational operator-=(rational & other)	   { return *this = *this - other; }
	rational operator *(const rational & other) const { return rational(this->nom*other.nom, this->den*other.den); }
	rational operator *(int a) { return rational(nom * a, den); }
	rational operator*=(const rational & other) { return *this = *this * other; }
	rational operator /(const rational & other) const
	{
		if (other.nom == 0)
			return 0;
		return rational (this->nom * other.den, this->den * other.nom);
	}
	rational operator/=(const rational & other) { return *this = *this / other; }

	void print()
	{
		if(den != 1)
			cout << nom << "/" << den;
		else
			cout << nom;
	}

	int nom;
	int den;
};


int digits(int n)
{
	if (n == 0)
		return 0;
	return digits(n / 10) + 1;
}
int len(rational a)
{
	if (a == 0)
		return 1;
	if (a.den == 1)
		return int(a.nom < 0) + digits(a.nom);
	return int(a.nom < 0) + digits(a.nom) + digits(a.den) + 1;
}


struct row
{
	row()
	{	}
	row(int M, bool equasion = false) : M(M), equasion(equasion)
	{
		R = new rational[M + int(equasion)];
		for (int i = 0; i < M; i++)
		{
			R[i].get_int();
		}
		if(equasion)
			R[M].get_int();
	}

	row(const row & other, int M)
	{
		if (other.equasion)
		{
			R = new rational[M + 1];
			this->equasion = true;
		}
		else
		{
			R = new rational[M];
			this->equasion = false;
		}
		for (int i = 0; i < M; i++)
			R[i] = other.R[i];
		if (equasion)
		{
			R[M] = other.R[M];
		}
	}

	void read(int M, bool equasion = false)
	{
		this->equasion = equasion;
		this->M = M;
		R = new rational[M + int(equasion)];
		for (int i = 0; i < M; i++)
		{
			R[i].get_int();
		}
		if (equasion)
			R[M].get_int();
	}

	rational & operator[](int i)
	{
		return R[i];
	}

	void operator=(const row & other)
	{
		for (int i = 0; i < M; i++)
		{
			R[i] = other.R[i];
		} 
	}

	void operator+=(const row & other)
	{
		for (int i = 0; i < M; i++)
		{
			R[i] += other.R[i];
		}
		if (equasion)
			R[M] += other.R[M];
	}
	void operator*=(rational a)
	{
		for (int i = 0; i < M; i++)
			R[i] *= a;
		if (equasion)
			R[M] *= a;
	}
	void operator/=(rational a)
	{
		*this *= (rational(1) / a);
	}

	void reduce(const row & other, rational k)
	{
		for (int i = 0; i < M; i++)
		{
			rational tmp = other.R[i] * k;
			R[i] -= tmp;
		}
		if (equasion)
		{
			rational tmp = other.R[M] * k;
			R[M] -= tmp;
		}
	}


	int M;
	rational* R;
	bool equasion;
};


struct Matrix
{
	Matrix(int N, int M, bool system = false): N(N), M(M), system(system)
	{
		matr = new row[N];
		if (system)
		{
			lens = new int[M + 1];
			lens[M] = 0;
		}
		else
			lens = new int[M];
		for (int i = 0; i < M; i++)
			lens[i] = 0;

		for (int i = 0; i < N; i++)
		{
			matr[i].read(M, true);
		}
		cout << endl;
	}

	void print()
	{
		for (int j = 0; j < M; j++)
		{
			lens[j] = 0;
			for (int i = 0; i < N; i++)
				if (len(matr[i][j]) > lens[j])
					lens[j] = len(matr[i][j]);
		}
		if (system)
		{
			lens[M] = 0;
			for (int i = 0; i < N; i++)
				if (len(matr[i][M]) > lens[M])
					lens[M] = len(matr[i][M]);
		}

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < M; j++)
			{
				matr[i][j].print();
				cout << " ";
				for (int k = 0; k < lens[j] - len(matr[i][j]) + 1; k++)
					cout << " ";
			}
			if (system)
			{
				cout << "| ";
				matr[i][M].print();
			}
			cout << endl;
		}
	}

	void swap(int i, int j)
	{
		rational* tmp = matr[i].R;
		matr[i].R = matr[j].R;
		matr[j].R = tmp;
 	}

	void solve()
	{
		int depth = 0;
		for (int j = 0; j < M - 1; j++)
		{
			int cur = depth;
			while (cur < N && matr[cur][j] == 0)
				cur++;
			if (cur == N)
			{
				continue;//!!!!!
			}
			if (cur != j)
			{
				swap(cur, depth);
			}

			for (int i = depth + 1; i < N; i++)
			{
				if (matr[i][j] != 0)
				{
					matr[i].reduce(matr[depth], matr[i][j]/matr[depth][j]);
				}
			}

			print();
			cout << endl;
			depth++;
		}

		for (int j = M - 1; j > 0; j--)
		{
			int cur = N - 1;
			bool changed = false;
			while (cur > 0 && matr[cur][j] == 0)
			{
				cur--;
			}
			if (cur == 0)
				continue;
			matr[cur] /= matr[cur][j];
			for (int i = cur - 1; i >= 0; i--)
			{

				if (matr[i][j] != 0)
				{
					changed = true;
					matr[i].reduce(matr[cur], matr[i][j] / matr[cur][j]);
				}
			}
			if (changed)
			{
				print();
				cout << endl;
			}
		}
	}

	int M;
	int N;
	row* matr;
	bool system;

	int* lens;
};


#undef int
int main()
{
#define int long long

	int x, y;
	rational tmp(1, 1);
	while (cin >> x >> y)
	{
		tmp *= rational(x, y);
		tmp.print();
		cout << endl;
	}

	int N, M;
	cin >> N >> M;
	Matrix a(N, M, true);
	a.solve();
	cout << len(a.matr[0][1]) << " " << a.lens[1];

    return EXIT_SUCCESS; 
}
