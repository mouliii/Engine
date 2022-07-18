#pragma once
#include <iostream>



template<typename T>
struct vec2
{
	T x, y;

	vec2(T x, T y)
		: x(x), y(y) {}

	vec2()
		: x(0), y(0) {}

	vec2(const vec2<T>& other)
		:
		x(other.x), y(other.y)
	{}


	T lenght()
	{
		return sqrt(this->x * this->x + this->y * this->y);
	}

	void normalize()
	{
		this->operator/=(this->lenght());
	}

	float dot(const vec2<T>& other)
	{
		return this->x * other.x + this->y * other.y;
	}


	vec2<T>& operator=(const vec2<T>& other)
	{
		this->x = other.x;
		this->y = other.y;
		return *this;
	}

	vec2<T> operator+(const vec2<T>& other)
	{
		return vec2<T>(this->x + other.x, this->y + other.y);
	}
	vec2<T> operator-(const vec2<T>& other)
	{
		return vec2<T>(this->x - other.x, this->y - other.y);
	}
	vec2<T> operator*(const vec2<T>& other)
	{
		return vec2<T>(this->x * other.x, this->y * other.y);
	}
	vec2<T> operator/(const vec2<T>& other)
	{
		return vec2<T>(this->x / other.x, this->y / other.y);
	}

	vec2<T>& operator+=(const vec2<T>& other)
	{
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	vec2<T>& operator-=(const vec2<T>& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}vec2<T>& operator*=(const vec2<T>& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}vec2<T>& operator/=(const vec2<T>& other)
	{
		this->x /= other.x;
		this->y /= other.y;
		return *this;
	}

	vec2<T> operator+(const T& value)
	{
		return vec2<T>(this->x + value, this->y + value);
	}
	vec2<T> operator-(const T& value)
	{
		return vec2<T>(this->x - value, this->y - value);
	}
	vec2<T> operator*(const T& value)
	{
		return vec2<T>(this->x * value, this->y * value);
	}
	vec2<T> operator/(const T& value)
	{
		return vec2<T>(this->x / value, this->y / value);
	}

	vec2<T>& operator+=(const T& value)
	{
		this->x += value;
		this->y += value;
		return *this;
	}
	vec2<T>& operator-=(const T& value)
	{
		this->x -= value;
		this->y -= value;
		return *this;
	}vec2<T>& operator*=(const T& value)
	{
		this->x *= value;
		this->y *= value;
		return *this;
	}
	vec2<T>& operator/=(const T& value)
	{
		if (value == 0) return *this;
		this->x /= value;
		this->y /= value;
		return *this;
	}

	vec2<T> operator-()
	{
		return vec2<T>(-this->x, -this->y);
	}

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& other)
{
	os << other.x << " " << other.y;
	return os;
}

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;


template<typename T>
struct vec3
{
	T x, y, z;

	vec3(T x, T y, T z)
		: x(x), y(y), z(z) {}


	vec3()
		: x(0), y(0), z(0) {}

	vec3(const vec3<T>& other)
		:
		x(other.x), y(other.y), z(other.z)
	{}


	T lenght()
	{
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	void normalize()
	{
		this->operator/=(this->lenght());
	}



	vec3<T>& operator=(const vec3<T>& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		return *this;
	}

	vec3<T> operator+(const vec3<T>& other)
	{
		return vec3<T>(this->x + other.x, this->y + other.y, this->z + other.z);
	}
	vec3<T> operator-(const vec3<T>& other)
	{
		return vec3<T>(this->x - other.x, this->y - other.y, this->z - other.z);
	}
	vec3<T> operator*(const vec3<T>& other)
	{
		return vec3<T>(this->x * other.x, this->y * other.y, this->z * other.z);
	}
	vec3<T> operator/(const vec3<T>& other)
	{
		return vec3<T>(this->x / other.x, this->y / other.y, this->z / other.z);
	}

	vec3<T>& operator+=(const vec3<T>& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		return *this;
	}
	vec3<T>& operator-=(const vec3<T>& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		return *this;
	}vec3<T>& operator*=(const vec3<T>& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		return *this;
	}vec3<T>& operator/=(const vec3<T>& other)
	{
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		return *this;
	}

	vec3<T> operator+(const T& value)
	{
		return vec3<T>(this->x + value, this->y + value, this->z + value);
	}
	vec3<T> operator-(const T& value)
	{
		return vec3<T>(this->x - value, this->y - value, this->z - value);
	}
	vec3<T> operator*(const T& value)
	{
		return vec3<T>(this->x * value, this->y * value, this->z * value);
	}
	vec3<T> operator/(const T& value)
	{
		return vec3<T>(this->x / value, this->y / value, this->z / value);
	}

	vec3<T>& operator+=(const T& value)
	{
		this->x += value;
		this->y += value;
		this->z += value;
		return *this;
	}
	vec3<T>& operator-=(const T& value)
	{
		this->x -= value;
		this->y -= value;
		this->z -= value;
		return *this;
	}vec3<T>& operator*=(const T& value)
	{
		this->x *= value;
		this->y *= value;
		this->z *= value;
		return *this;
	}
	vec3<T>& operator/=(const T& value)
	{
		if (value == 0) return *this;
		this->x /= value;
		this->y /= value;
		this->z /= value;
		return *this;
	}

	vec3<T> operator-()
	{
		return vec3<T>(-this->x, -this->y, -this->z);
	}


};

template <typename T>
std::ostream& operator<<(std::ostream& os, const vec3<T>& other)
{
	os << other.x << " " << other.y << " " << other.z;
	return os;
}

typedef vec3<int> vec3i;
typedef vec3<float> vec3f;


template<typename T>
struct vec4
{
	T x, y, z, w;

	vec4(T x, T y, T z, T w)
		: x(x), y(y), z(z), w(w) {}


	vec4()
		: x(0), y(0), z(0), w(0) {}

	vec4(const vec4<T>& other)
		:
		x(other.x), y(other.y), z(other.z), w(other.w)
	{}


	T lenght()
	{
		return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
	}

	void normalize()
	{
		this->operator/=(this->lenght());
	}



	vec4<T>& operator=(const vec4<T>& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->w = other.w;
		return *this;
	}

	vec4<T> operator+(const vec4<T>& other)
	{
		return vec4<T>(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);
	}
	vec4<T> operator-(const vec4<T>& other)
	{
		return vec4<T>(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w);
	}
	vec4<T> operator*(const vec4<T>& other)
	{
		return vec4<T>(this->x * other.x, this->y * other.y, this->z * other.z, this->w * other.w);
	}
	vec4<T> operator/(const vec4<T>& other)
	{
		return vec4<T>(this->x / other.x, this->y / other.y, this->z / other.z, this->w / other.w);
	}

	vec4<T>& operator+=(const vec4<T>& other)
	{
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
		this->w += other.w;
		return *this;
	}
	vec4<T>& operator-=(const vec4<T>& other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
		this->w -= other.w;
		return *this;
	}vec4<T>& operator*=(const vec4<T>& other)
	{
		this->x *= other.x;
		this->y *= other.y;
		this->z *= other.z;
		this->w *= other.w;
		return *this;
	}vec4<T>& operator/=(const vec4<T>& other)
	{
		this->x /= other.x;
		this->y /= other.y;
		this->z /= other.z;
		this->w /= other.w;
		return *this;
	}

	vec4<T> operator+(const T& value)
	{
		return vec4<T>(this->x + value, this->y + value, this->z + value, this->w + value);
	}
	vec4<T> operator-(const T& value)
	{
		return vec4<T>(this->x - value, this->y - value, this->z - value, this->w - value);
	}
	vec4<T> operator*(const T& value)
	{
		return vec4<T>(this->x * value, this->y * value, this->z * value, this->w * value);
	}
	vec4<T> operator/(const T& value)
	{
		return vec4<T>(this->x / value, this->y / value, this->z / value, this->w / value);
	}

	vec4<T>& operator+=(const T& value)
	{
		this->x += value;
		this->y += value;
		this->z += value;
		this->w += value;
		return *this;
	}
	vec4<T>& operator-=(const T& value)
	{
		this->x -= value;
		this->y -= value;
		this->z -= value;
		this->w -= value;
		return *this;
	}vec4<T>& operator*=(const T& value)
	{
		this->x *= value;
		this->y *= value;
		this->z *= value;
		this->w *= value;
		return *this;
	}
	vec4<T>& operator/=(const T& value)
	{
		if (value == 0) return *this;
		this->x /= value;
		this->y /= value;
		this->z /= value;
		this->w /= value;
		return *this;
	}

	vec4<T> operator-()
	{
		return vec4<T>(-this->x, -this->y, -this->z, -this->w);
	}

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const vec4<T>& other)
{
	os << other.x << " " << other.y << " " << other.z << " " << other.w;
	return os;
}

typedef vec4<int> vec4i;
typedef vec4<float> vec4f;


