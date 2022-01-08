#pragma once
#include <math.h>
#include "Vecs.h"

template<uint32_t size>
void get_cofactor(float A[size][size], float temp[size][size], uint32_t p, uint32_t q, uint32_t n)
{
	int i = 0, j = 0;

	for (size_t row = 0; row < n; row++)
	{
		for (size_t col = 0; col < n; col++)
		{

			if (row != p && col != q)
			{
				temp[i][j++] = A[row][col];

				if (j == n - 1)
				{
					j = 0;
					i++;
				}
			}

		}
	}

}

template<uint32_t size>
inline float determinant(float A[size][size], uint32_t n)
{
	float det = 0;

	if (n == 1)
		return A[0][0];

	float temp[size][size];

	int sign = 1;

	for (uint32_t f = 0; f < n; f++)
	{
		get_cofactor<size>(A, temp, 0u, f, n);
		det += sign * A[0][f] * determinant<size>(temp, n - 1);

		sign = -sign;

	}

	return det;
}

template<uint32_t size>
inline void adjoint(float A[size][size], float adj[size][size])
{

	int sign = 1;
	float temp[size][size];

	for (uint32_t i = 0; i < size; i++)
	{
		for (uint32_t j = 0; j < size; j++)
		{
			get_cofactor<size>(A, temp, i, j, size);
			sign = ((i + j) % 2 == 0) ? 1 : -1;
			adj[j][i] = (sign) * (determinant<size>(temp, size-1));
		}
	}

}


template <typename T>
class mat2
{
public:
	mat2() = default;

	T cells[2][2];

	T(&operator[](size_t y))[2]
	{
		return cells[y];
	}

};
typedef mat2<float> mat2f;
typedef mat2<int>	mat2i;

template <typename T>
class mat3
{
public:
	mat3() = default;

	T cells[3][3];
	static const uint32_t size = 3;
	T (&operator[](size_t y))[3]
	{
		return cells[y];
	}

	vec3<T> operator*(const vec3<T>& v) const
	{
		vec3<T> out;
		out.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2];
		out.y = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2];

		return out;
	}

	vec2<T> operator*(const vec2<T>& v) const
	{
		vec2<T> out;
		out.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2];
		out.x = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2];

		return out;
	}

	mat3 operator*(const mat3& rhs) const
	{
		mat3 out;
		for (size_t row_left = 0; row_left < 3; row_left++)
		{
			for (size_t col_right = 0; col_right < 3; col_right++)
			{
				out.cells[row_left][col_right] = (T)0;
				for (size_t i = 0; i < 3; i++)
				{
					out.cells[row_left][col_right] += cells[row_left][i] * rhs.cells[i][col_right];
				}
			}
		}
		return out;
	}

	mat3& operator*=(const mat3& rhs)
	{
		return *this = *this * rhs;
	}


	static mat3 identity()
	{
		return scale((T)1);
	}

	static mat3 scale(T factor)
	{
		return
		{
			(T)factor, (T)0, (T)0,
			(T)0, (T)factor, (T)0,
			(T)0, (T)0, (T)1
		};
	}

	static mat3 translate(T x, T y)
	{
		return
		{
			(T)1, (T)0, x,
			(T)0, (T)1, y,
			(T)0, (T)0, (T)1
		};
	}

	static mat3 translate(vec2<T> translation)
	{
		return
		{
			(T)1, (T)0, translation.x,
			(T)0, (T)1, translation.y,
			(T)0, (T)0, (T)1
		};
	}

	static mat3 rotate_z(T theta)
	{
		return
		{
			cos<T>(theta), -sin<T>(theta), (T)0,
			sin<T>(theta),	cos<T>(theta), (T)0,
			(T)0		 ,	(T)0		 , (T)1

		};
	}


	static mat3 ortho(float left, float right, float bottom, float top)
	{
		return
		{
			(T)(2 / (right - left)), (T)0, (T)(-((right + left) / (right - left))),
			(T)0, (T)(2 / (top - bottom)), (T)(-((top + bottom) / (top - bottom))),
			(T)0, (T)0, (T)1
		};
	}

	mat3<float> inverse()
	{
		float det = determinant<size>(cells, size);
		float adj[size][size];

		mat3<float> retmat;

		adjoint<size>(cells, adj);

		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				retmat[i][j] = adj[i][j] / det;

			}
		}

	
		
		return retmat;
	}

};

typedef mat3<float> mat3f;
typedef mat3<int> mat3i;


template <typename T>
class mat4
{
public:
	mat4() = default;

	T cells[4][4];
	static const uint32_t size = 4;
	T(&operator[](size_t y))[4]
	{
		return cells[y];
	}
	
	vec3<T> operator*(const vec3<T>& v) const
	{
		vec3<T> out;
		out.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2] * v.z;
		out.y = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2] * v.z;
		out.z = cells[2][0] * v.x + cells[2][1] * v.y + cells[2][2] * v.z;
		return out;
	}

	vec2<T> operator*(const vec2<T>& v) const
	{
		vec2<T> out;
		out.x = cells[0][0] * v.x + cells[0][1] * v.y + cells[0][2];
		out.x = cells[1][0] * v.x + cells[1][1] * v.y + cells[1][2];

		return out;
	}

	mat4 operator*(const mat4& rhs) const
	{
		mat4 out;
		for (size_t row_left = 0; row_left < 4; row_left++)
		{
			for (size_t col_right = 0; col_right < 4; col_right++)
			{
				out.cells[row_left][col_right] = (T)0;
				for (size_t i = 0; i < 4; i++)
				{
					out.cells[row_left][col_right] += cells[row_left][i] * rhs.cells[i][col_right];
				}
			}
		}
		return out;
	}

	mat4& operator*=(const mat4& rhs)
	{
		return *this = *this * rhs;
	}


	static mat4 identity()
	{
		return scale((T)1);
	}

	static mat4 scale(T factor)
	{
		return
		{
			(T)factor, (T)0, (T)0, (T)0,
			(T)0, (T)factor, (T)0, (T)0,
			(T)0, (T)0, (T)factor, (T)0,
			(T)0, (T)0, (T)0, (T)1
		};
	}

	static mat4 translate(T x, T y, T z)
	{
		return
		{
			(T)1, (T)0, (T)0, x,
			(T)0, (T)1, (T)0, y,
			(T)0, (T)0, (T)1, z,
			(T)0, (T)0, (T)0,(T)1
		};
	}

	static mat4 translate(vec3<T> translation)
	{
		return
		{
			(T)1, (T)0, (T)0, translation.x,
			(T)0, (T)1, (T)0, translation.y,
			(T)0, (T)0, (T)1, translation.z,
			(T)0, (T)0, (T)0,(T)1
		};
	}


	static mat4 translate(vec2<T> translation)
	{
		return
		{
			(T)1, (T)0, (T)0, translation.x,
			(T)0, (T)1, (T)0, translation.y,
			(T)0, (T)0, (T)1, 0,
			(T)0, (T)0, (T)0,(T)1
		};
	}

	static mat4 rotate_z(T theta)
	{
		return
		{
			cos(theta), -sin(theta), (T)0, (T)0,
			sin(theta),	cos(theta), (T)0, (T)0,
			(T)0		 ,	(T)0		 , (T)1, (T)0,
			(T)0		 ,  (T)0		 , (T)0, (T)1
		};
	}


	static mat4 ortho(T left, T right, T bottom, T top, T far, T near)
	{
		return
		{
			((T)2 / (right - left)), (T)0, (T)0, -((right + left) / (right - left)),
			(T)0, ((T)2 / (top - bottom)), (T)0, -((top + bottom) / (top - bottom)),
			(T)0, (T)0, (T)-2 / (far-near), -((far+near)/(far-near)),
			(T)0, (T)0, (T)0, (T)1
		};
	}

	mat4<float> inverse()
	{
		float det = determinant<size>(cells, size);
		float adj[size][size];

		mat4<float> retmat;

		adjoint<size>(cells, adj);

		for (size_t i = 0; i < size; i++)
		{
			for (size_t j = 0; j < size; j++)
			{
				retmat[i][j] = adj[i][j] / det;

			}
		}



		return retmat;
	}

};

typedef mat4<float> mat4f;
typedef mat4<int> mat4i;

