//#include "pch.h"
#include <iostream>

#include <vector>
#include <algorithm>

#include <cmath>

const double eps = 1e-2;
const double PI = 3.14159265358979323846;

using namespace std;



// максимальная сепень двойки не меньше n
int max_pow(int n)
{
	if (n == 1) { return 1; }
	return 2 * max_pow((n + 1) / 2);
}


struct Complex
{
	Complex() = default;
	Complex(double x) : x(x), y(0)
	{	}
	Complex(double x, double y) : x(x), y(y)
	{	}
	explicit operator double() const
	{
		if (abs(y) > eps)
		{
			cout << "warning: accuracy loss when converting   Complex   to   double\n";
		}
		return x;
	}

	double x;
	double y;
};

Complex operator +(Complex a, Complex b)
{
	return Complex(a.x + b.x, a.y + b.y);
}
Complex operator -(Complex a, Complex b)
{
	return Complex(a.x - b.x, a.y - b.y);
}
Complex operator *(Complex a, Complex b)
{
	return Complex(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}


// корни из единицы
struct Units
{
	Units() = default;
	Units(int n, int k) : n(n), k(k)
	{	}

	explicit operator Complex() const
	{
		return Complex(cos(2*PI*k / n), 
					   sin(2*PI*k / n));
	}
	Units decrease() { return Units(n / 2, k % (n / 2)); }

	int n;
	int k;
};

Units operator *(Units w1, Units w2)
{
	if (w1.n != w2.n)
	{
		cout << "ERROR: can't multiply units\n";
		abort();
	}
	return Units(w1.n, (w1.k + w2.k) % w1.n);
}


// рекурсивное быстрое преобразование Фурье
vector<Complex> fourier(vector<Complex> & a, Units w0)
{
	int N2 = a.size();

	if (N2 == 1) { return move(a); }
	if (N2 != max_pow(N2))
	{
		cout << "Size error: func 'furier'\n";
		abort();
	}

	vector<Complex> a0(a.size() / 2, 0);
	vector<Complex> a1(a.size() / 2, 0);
	for (int i = 0; i < a.size(); i++)
	{
		if (i % 2 == 0) { a0[i / 2] = a[i]; }
		else { a1[i / 2] = a[i]; }
	}

	vector<Complex> y0 = fourier(a0, w0.decrease());
	vector<Complex> y1 = fourier(a1, w0.decrease());
	vector<Complex> y(N2, 0);
	Units w(N2, 0);
	for (int i = 0; i < N2 / 2; i++)
	{
		y[i]		  = y0[i] + (Complex)w * y1[i];
		y[i + N2 / 2] = y0[i] - (Complex)w * y1[i];
		w = w * w0;
	}
	return move(y);
}



struct Polynom
{
	Polynom() = default;
	Polynom(Polynom & other) = default;
	Polynom(Polynom && other) = default;
	Polynom & operator =(Polynom & other) = default;
	Polynom & operator =(Polynom && other) = default;

	// сначала младшие коэффициенты
	void get(int n)
	{
		this->n = n;
		this->n2 = max_pow(n);
		coef.assign(n2, 0);
		for_each(coef.begin(), coef.begin() + n, [](double& cur) { cin >> cur; });
	}
	double operator ()(double x0)
	{
		double rez = 0;
		for_each(coef.begin(), coef.begin() + n,
				[&rez, x0](double cur) { rez = x0 * rez + cur; });
		return rez;
	}
	void operator *=(double k)
	{
		for_each(coef.begin(), coef.begin() + n, [k](double& x) { x *= k; });
	}
	

	int n2, n; // коэффициенты [n; n2 - 1] должны быть нулями
	vector<double> coef;
};

ostream & operator <<(ostream & stream, Polynom f)
{
	bool first_not_null = true;
	for (int k = 0; k < f.n; k++)
	{
		if (abs(f.coef[k]) > eps)
		{
			if (!first_not_null) { stream << " + "; } // перед первым слагаемым плюс не пишется
			else { first_not_null = false; }

			if (k == 0) { stream << f.coef[k]; }
			else if (abs(f.coef[k] - 1) < eps) { stream << "x^" << k; }
			else { stream << f.coef[k] << "x^" << k; }
		}
	}
	return stream;
}

Polynom operator +(Polynom & f, Polynom & g)
{
	int N  = f.n > g.n ? f.n : g.n;
	int N2 = f.n > g.n ? f.n2 : g.n2;
	Polynom h;
	h.n = N, h.n2 = N2;
	h.coef.assign(N2, 0);
	for (int i = 0; i < N; i++)
	{
		h.coef[i] = f.coef[i] + g.coef[i];
	}
	return move(h);
}

// O(n log(n))
Polynom operator *(Polynom & f, Polynom & g)
{
	int N = f.n + g.n;
	int N2 = max_pow(N);
	vector<Complex> tmpf(N2); // fourier вызывается от vector<Complex>
	vector<Complex> tmpg(N2);
	for (int i = 0; i < N2; i++)
	{
		if (i < f.n2) { tmpf[i] = f.coef[i]; }
		else { tmpf[i] = 0; }
		if (i < g.n2) { tmpg[i] = g.coef[i]; }
		else { tmpg[i] = 0; }
	}
	vector<Complex> yf = fourier(tmpf, Units(N2, 1));
	vector<Complex> yg = fourier(tmpg, Units(N2, 1));
	tmpf.clear(); tmpf.shrink_to_fit();
	tmpg.clear(); tmpg.shrink_to_fit();

	vector<Complex> h(N2, 0);
	for (int i = 0; i < N2; i++) { h[i] = yf[i] * yg[i]; }
	h = fourier(h, Units(N2, N2 - 1)); // обратное преобразование Фурье
	vector<double> h1(N2, 0);
	for (int i = 0; i < N2; i++) { h1[i] = (double)h[i] / N2; }

	Polynom ans;
	ans.coef = h1;
	ans.n = f.n + g.n;
	ans.n2 = N2;
	return move(ans);
}


int main()
{
	int n;
//коэффициенты вводятся от младших к сташим

	cin >> n;
	Polynom f;
	f.get(n);

	cin >> n;
	Polynom g;
	g.get(n);

	Polynom h = f * g;
	cout << "\n\n( " << f <<  " ) * ( " << g << " ) =\n" << h;
	return EXIT_SUCCESS;
}