#include <math.h>
#include <iostream>

#define PI 3.14159265
#define POS_INFINITY 1e30
#define NEG_INFINITY -1e30

template <typename T> class Vector2D
{
private:
	T x;
	T y;

public:
	explicit Vector2D(const T& x = 0, const T& y = 0) : x(x), y(y) {}
	Vector2D(const Vector2D<T>& src) : x(src.x), y(src.y) {}
	virtual ~Vector2D() {}

	
	inline T X() const { return x; }
	inline T Y() const { return y; }
	inline T X(const T& x) { this->x = x; }
	inline T Y(const T& y) { this->y = y; }

	
	inline Vector2D<T> operator-() const
	{
		return Vector2D<T>(-x, -y);
	}

	inline Vector2D<T> operator+() const
	{
		return Vector2D<T>(+x, +y);
	}

	inline Vector2D<T> operator+(const Vector2D<T>& v) const
	{
		return Vector2D<T>(x + v.x, y + v.y);
	}

	inline Vector2D<T> operator-(const Vector2D<T>& v) const
	{
		return Vector2D<T>(x - v.x, y - v.y);
	}

	inline Vector2D<T> operator*(const T& s) const
	{
		return Vector2D<T>(x*s, y*s);
	}

	// Dot product
	inline T operator*(const Vector2D<T>& v) const
	{
		return x * v.x + y * v.y;
	}

	// Cross product
	static T cross(const Vector2D<T>& v1, const Vector2D<T>& v2)
	{
		return (v1.x * v2.y - v1.y * v2.x );
	}

	// l-2 norm
	inline T norm() const { return sqrt(x*x + y * y); }

	// inner angle (radians)
	static T angle(const Vector2D<T>& v1, const Vector2D<T>& v2)
	{	
		T angle = acos((v1 * v2) / (v1.norm() * v2.norm()));
		//not a number
		if (angle != angle) {
			return PI;
		}
		return angle;
	}
	// inner angle (radians)
	static T distance(const Vector2D<T>& v1, const Vector2D<T>& v2)
	{
		const double dx = v2.x - v1.x;
		const double dy = v2.y - v1.y;
		const double lsq = dx * dx + dy * dy;
		return std::sqrt(lsq);
		
	}
	
};