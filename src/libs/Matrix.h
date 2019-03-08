#ifndef _MATRIX_H
#define _MATRIX_H

#include <cmath>
#include <initializer_list>
#include <string>
#include <sstream>

namespace Mat
{

	template<unsigned int height, unsigned int width, typename T = float>
	class Matrix
	{
		static_assert(std::is_fundamental<T>::value, "Matrix and Vector can only use primitive types!");

		template<unsigned int h, unsigned int w, typename T2> friend class Matrix;

	protected:

		T vals[height*width];

	public:
		//TODO: passing 0 for diagVal looks the same as passing nullptr for values, resulting in ambiguous calls..
		explicit Matrix(T diagVal = 1);
		explicit Matrix(T values[]);
		explicit Matrix(std::initializer_list<T> values);
		Matrix(const Matrix &mat);
		~Matrix();

		T* operator[](int row);
		T const* operator[](int row) const;

		std::string toString() const;

		//Returns a T[] representing the matrix, in row-major order
		const T* getGLFormat() const;
	};


	//Totally not sure if this class is worth it, the only changed functionality is the 
	//subscript operator, but I haven't been able to find another way around it yet
	//Tried std::enable_if / SFINAE, but it errors before that since the functions differ
	//only by return type
	template<unsigned int length, typename T = float>
	class Vector : public Matrix<length, 1, T>
	{
	public:
		explicit Vector(T initVal = 0) : Matrix<length, 1, T>(initVal){}
		explicit Vector(T values[]) : Matrix<length, 1, T>(values){}
		explicit Vector(std::initializer_list<T> values) : Matrix<length, 1, T>(values){}
		Vector(const Matrix<length, 1, T> &mat) : Matrix<length, 1, T>(mat){}

		T& operator[](int index)
		{ 
			return this->vals[index]; 
		}

		const T operator[](int index) const
		{
			return this->vals[index];
		}
	};



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(T diagVal)
	{
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				if (i == j || width == 1)
				{
					vals[i*width + j] = diagVal;
				}
				else
				{
					vals[i*width + j] = 0;
				}
			}
		}
	}
	
	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(T values[])
	{
		for (unsigned int i = 0; i < height*width; i++)
		{
			vals[i] = values[i];
		}
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(std::initializer_list<T> values)
	{
		const T *begin = values.begin();

		for (unsigned int i = 0; i < height*width; i++)
		{
			vals[i] = begin[i];
		}
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::Matrix(const Matrix &mat)
	{
		for (unsigned int i = 0; i < height*width; i++)
		{
			this->vals[i] = mat.vals[i];
		}
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T>::~Matrix()
	{
	}



	template<unsigned int height, unsigned int width, typename T>
	bool operator==(const Matrix<height, width, T> &left, const Matrix<height, width, T> &right)
	{
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				if (left[i][j] != right[i][j])
					return false;
			}
		}
		return true;
	}

	//The assignment operator shouldn't need to be overloaded since vals is stored in the class



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator+(const Matrix<height, width, T> &left, const Matrix<height, width, T> &right)
	{
		Matrix<height, width, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] + right[i][j];
			}
		}
		return ret;
	}

	//Not a fan of all this code duplication, but it's better than vec<x> + vec<x> = mat<x,1> followed by an implicit
	//conversion back to vec<x>
	template<unsigned int height, typename T>
	Vector<height, T> operator+(const Vector<height, T> &left, const Vector<height, T> &right)
	{
		Vector<height, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			ret[i] = left[i] + right[i];
		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator-(const Matrix<height, width, T> &right)
	{
		Matrix<height, width, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				ret[i][j] = -right[i][j];
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator-(const Vector<height, T> &right)
	{
		Vector<height, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			ret[i] = -right[i];

		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator-(const Matrix<height, width, T> &left, const Matrix<height, width, T> &right)
	{
		Matrix<height, width, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] - right[i][j];
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator-(const Vector<height, T> &left, const Vector<height, T> &right)
	{
		Vector<height, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			ret[i] = left[i] - right[i];
		}
		return ret;
	}



	template<unsigned int height, unsigned int common, unsigned int width, typename T>
	Matrix<height, width, T> operator*(const Matrix<height, common, T> &left, const Matrix<common, width, T> &right)
	{
		Matrix<height, width, T> ret;

		for (unsigned int row = 0; row < height; row++)
		{
			for (unsigned int col = 0; col < width; col++)
			{
				ret[row][col] = 0;
				for (unsigned int i = 0; i < common; i++)
				{
					ret[row][col] += left[row][i] * right[i][col];
				}
			}
		}
		return ret;
	}

	template<unsigned int height, unsigned int common, typename T>
	Vector<height, T> operator*(const Matrix<height, common, T> &left, const Vector<common, T> &right)
	{
		Vector<height, T> ret;

		for (unsigned int row = 0; row < height; row++)
		{
			ret[row] = 0;
			for (unsigned int i = 0; i < common; i++)
			{
				ret[row] += left[row][i] * right[i];
			}
		}
		return ret;
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator*(T num, const Matrix<height, width, T> &mat)
	{
		return mat * num;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator*(T num, const Vector<height, T> &vec)
	{
		return vec * num;
	}

	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator*(const Matrix<height, width, T> &left, T num)
	{
		Matrix<height, width, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] * num;
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator*(const Vector<height, T> &left, T num)
	{
		Vector<height, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			ret[i] = left[i] * num;
		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	Matrix<height, width, T> operator/(const Matrix<height, width, T> &left, T num)
	{
		Matrix<height, width, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				ret[i][j] = left[i][j] / num;
			}
		}
		return ret;
	}

	template<unsigned int height, typename T>
	Vector<height, T> operator/(const Vector<height, T> &left, T num)
	{
		Vector<height, T> ret;
		for (unsigned int i = 0; i < height; i++)
		{
			ret[i] = left[i] / num;
		}
		return ret;
	}



	template<unsigned int height, unsigned int width, typename T>
	T* Matrix<height, width, T>::operator[](int row)
	{
		return this->vals + row * width;
	}

	template<unsigned int height, unsigned int width, typename T>
	T const* Matrix<height, width, T>::operator[](int row) const
	{
		return this->vals + row * width;
	}



	template<unsigned int height, unsigned int width, typename T>
	std::string Matrix<height, width, T>::toString() const
	{
		std::stringstream ret = std::stringstream();

		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width - 1; j++)
			{
				ret << (vals[i*width + j]) << (",");
			}
			ret << (vals[(i + 1)*width - 1]) << ("\n");
		}
		return ret.str();
	}




    //Non-member Matrix functions
    	template<unsigned int length>
	float magnitude(const Vector<length> &vec)
	{
		float mag = 0;

		for (unsigned int i = 0; i < length; i++)
		{
			mag += vec[i] * vec[i];
		}

		return std::sqrt(mag);
	}

	template<unsigned int length>
	Vector<length> normalize(const Vector<length> &vec)
	{
		float mag = magnitude(vec);

		if (mag != 0)
			return vec / mag;
		else
			return vec;
	}

	Vector<3> cross(Vector<3> left, Vector<3> right)
	{
		float vals[3];

		vals[0] = left[1] * right[2] - left[2] * right[1];
		vals[1] = left[2] * right[0] - left[0] * right[2];
		vals[2] = left[0] * right[1] - left[1] * right[0];

		return Vector<3>(vals);
	}

	template<unsigned int length>
	Vector<length> dot(const Vector<length> &left, const Vector<length> &right) 
	{
		Vector<length> ret;
		
		for (int i = 0; i < length; i++)
		{
			ret[i] = left[i] * right[i];
		}

		return ret;
	}
}


typedef Mat::Matrix<2, 2, float> Mat2;
typedef Mat::Matrix<3, 3, float> Mat3;
typedef Mat::Matrix<4, 4, float> Mat4;
typedef Mat::Vector<2, float> Vec2;
typedef Mat::Vector<3, float> Vec3;
typedef Mat::Vector<4, float> Vec4;

typedef Mat::Vector<3, unsigned char> Color;

#endif
