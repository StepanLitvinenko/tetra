/*
 * complex.h
 *
 *  Created on: 29.12.2017
 *      Author: dblagov
 */

#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <math.h>
#include <iostream>

template< typename StoredType > struct Complex {
	StoredType re, im ;
	Complex() : re(), im() {}
	Complex( StoredType re, StoredType im ) : re( re ), im( im ) {}

	template< typename InputType > Complex< StoredType >& operator=( const Complex< InputType >& val ) {
		re = static_cast< StoredType >( val.re );
		im = static_cast< StoredType >( val.im );
		return *this;
	}
	
	inline void conj()	
	{
		im = -im;
	}
	
	inline float abs() const
	{
		return sqrt(re * re + im * im);
	}
	
	inline float sqrAbs() const
	{
		return re * re + im * im;
	}
	
	inline float arg() const
	{
		return (im == 0.0f) ? 0.0f : atan2(im, re);
	}
	
	inline float argDeg() const
	{
		return arg() * 180.0 / M_PI;
	}
};

template<typename T> inline Complex<T> conj(const Complex<T>& val)
{
	return Complex<T>(val.re, -val.im);
}

template <typename T> inline Complex<T> operator+(const Complex<T>& l, const Complex<T>& r)
{
	return Complex<T>(l.re + r.re, l.im + r.im);
}

template <typename T> inline Complex<T> operator-(const Complex<T>& l, const Complex<T>& r)
{
	return Complex<T>(l.re - r.re, l.im - r.im);
} 

template <typename T> inline Complex<T> operator*(const Complex<T>& l, const Complex<T>& r)
{
	return Complex<T>(l.re * r.re - l.im * r.im, l.re * r.im + l.im * r.re);
}

template <typename T, typename InputType> inline Complex<T> operator / (const Complex<T>& l, InputType val)
{
	return Complex<T>(l.re/val, l.im/val);
}

template <typename T> inline Complex<T> operator / (const Complex<T>& l, const Complex<T>& r)
{
	return (l * conj(r)) / r.abs();
}

template< typename T > std::ostream& operator << (std::ostream& stream, Complex< T > value) {
	stream << "{" << value.re << "; " << value.im << "}";
	return stream ;
}
#endif /* COMPLEX_H_ */
