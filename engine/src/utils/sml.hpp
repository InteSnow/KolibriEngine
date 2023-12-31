#pragma once
#include <initializer_list>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

template<typename T>
T radians(T angle) {
	return angle * (T)M_PI / 180;
}

template<typename T>
T degrees(T angle) {
	return angle * (T)180 / M_PI;
}

template <size_t size, typename T>
struct vec {
	T operator[](size_t i);
	T length();
};

template <typename T>
struct vec<2, T> {
	T x{}, y{};
	vec() {}
	vec(T x) : x{ x }, y{ x } {}
	vec(T x, T y) : x{ x }, y{ y } {}
	vec(std::initializer_list<T> l) {
		if (l.end() - l.begin() != 2) return;
		x = *l.begin();
		y = *(l.begin() + 1);
	}
	
	bool operator==(const vec<2, T>& right) const {
		return x == right.x && y == right.y;
	}
	
	T operator[](size_t i) {
		if (i >= 2) return y;
		return (&x)[i];
	}
	vec<2, T> operator*(const T& a) {
		return vec<2, T>(x * a, y * a);
	}
	vec<2, T> operator*(T&& a) {
		return vec<2, T>(x * a, y * a);
	}
	vec<2, T> operator/(const T& a) {
		return this->operator*(1 / a);
	}
	vec<2, T> operator/(T&& a) {
		return this->operator*(1 / a);
	}

	vec<2, T> operator+(vec<2, T>& right) {
		return vec<2, T>(x + right.x, y + right.y);
	}
	vec<2, T> operator-(vec<2, T>& right) {
		return vec<2, T>(x - right.x, y - right.y);
	}

	T length() {
		return std::sqrt(x * x + y * y);
	}
};

template <typename T>
struct vec<3, T> {
	T x{}, y{}, z{};
	vec() {}
	vec(T x) : x{ x }, y{ x }, z{ x } {}
	vec(T x, T y, T z) : x{ x }, y{ y }, z{ z } {}
	vec(vec<2, T> xy, T z) : z{ z } {
		x = xy.x;
		y = xy.y;
	}
	vec(vec<4, T> xyzw) : x{xyzw.x}, y{xyzw.y}, z{xyzw.z} {}
	vec(std::initializer_list<T> l) {
		if (l.end() - l.begin() != 3) return;
		x = *l.begin();
		y = *(l.begin()+1);
		z = *(l.begin()+2);
	}

	bool operator==(const vec<3, T>& right) const {
		return x == right.x && y == right.y && z == right.z;
	}
	
	T& operator[](size_t i) {
		if (i >= 3) return z;
		return (&x)[i];
	}
	vec<3, T> operator*(vec<3, T>& right) const {
		return vec<3, T>(x * right.x, y * right.y, z * right.z);
	}
	vec<3, T> operator*(vec<3, T>&& right) const {
		return vec<3, T>(x * right.x, y * right.y, z * right.z);
	}
	vec<3, T> operator*(const T& a) const {
		return vec<3, T>(x * a, y * a, z * a);
	}
	vec<3, T> operator*(T&& a) const {
		return vec<3,T>(x * a, y * a, z * a);
	}
	vec<3, T> operator/(const T& a) const {
		return this->operator*(1/a);
	}
	vec<3, T> operator/(T&& a) const {
		return this->operator*(1 / a);
	}

	vec<3, T> operator+(vec<3, T>& right) const {
		return vec<3, T>(x + right.x, y + right.y, z + right.z);
	}
	vec<3, T> operator+(vec<3, T>&& right) const {
		return vec<3, T>(x + right.x, y + right.y, z + right.z);
	}
	vec<3, T> operator-(vec<3, T>& right) const {
		return vec<3, T>(x - right.x, y - right.y, z - right.z);
	}
	vec<3, T> operator-(vec<3, T>&& right) const {
		return vec<3, T>(x - right.x, y - right.y, z - right.z);
	}
	vec<3, T> operator-() const {
		return vec<3, T>(-x, -y, -z);
	}

	T length() {
		return std::sqrt(x * x + y * y + z * z);
	}
};

template <typename T>
struct vec<4, T> {
	T x{}, y{}, z{}, w{};
	vec() {}
	vec(T x) : x{ x }, y{ x }, z{ x }, w{ x } {}
	vec(T x, T y, T z, T w) : x{ x }, y{ y }, z{ z }, w{ w } {}
	vec(std::initializer_list<T> l) {
		if (l.end() - l.begin() != 4) return;
		x = *l.begin();
		y = *(l.begin() + 1);
		z = *(l.begin() + 2);
		w = *(l.begin() + 3);
	}
	vec(const vec<3, T>& xyz, T w) : w{ w } {
		x = xyz.x;
		y = xyz.y;
		z = xyz.z;
	}
	vec(const vec<2, T>& xy, const vec<2, T>& zw) {
		x = xy.x;
		y = xy.y;
		z = zw.z;
		w = zw.w;
	}

	explicit operator vec<3, T>() const {
		return vec<3, T>(this->x, this->y, this->z);
	}

	bool operator==(const vec<4, T>& right) const {
		return x == right.x && y == right.y && z == right.z && w = right.w;
	}

	T& operator[](size_t i) {
		if (i >= 4) return w;
		return (&x)[i];
	}
	vec<4, T> operator*(vec<4, T>& right) const {
		return vec<4, T>(x * right.x, y * right.y, z * right.z, w * right.w);
	}
	vec<4, T> operator*(vec<4, T>&& right) const {
		return vec<4, T>(x * right.x, y * right.y, z * right.z, w * right.w);
	}
	vec<4, T> operator*(const T& a) const {
		return vec<4, T>(x * a, y * a, z * a, w * a);
	}
	vec<4, T> operator*(T&& a) const {
		return vec<4, T>(x * a, y * a, z * a, w * a);
	}
	vec<4, T> operator/(const T& a) {
		return this->operator*(1 / a);
	}
	vec<4, T> operator/(T&& a) const {
		return this->operator*(1 / a);
	}

	vec<4, T> operator+(vec<4, T>& right) const {
		return vec<4, T>(x + right.x, y + right.y, z + right.z, w + right.w);
	}
	vec<4, T> operator+(vec<4, T>&& right) const {
		return vec<4, T>(x + right.x, y + right.y, z + right.z, w + right.w);
	}
	vec<4, T> operator-(vec<4, T>& right) const {
		return vec<4, T>(x - right.x, y - right.y, z - right.z, w - right.w);
	}
	vec<4, T> operator-(vec<4, T>&& right) const {
		return vec<4, T>(x - right.x, y - right.y, z - right.z, w - right.w);
	}
	vec<4, T> operator-() const{
		return vec<4, T>(-x, -y, -z, -w);
	}

	T length() {
		return std::sqrt(x * x + y * y + z * z + w * w);
	}
};

// template <typename T>
// void hash_combine(size_t& h, const T& val) {
// 	h ^= std::hash<T>{}(val) + 0x9e3779b9 + (h << 6) + (h >> 2);
// }

// template <typename T>
// struct std::hash<vec<2, T>> {
// 	size_t operator()(const vec<2, T>& v) const {
// 		size_t h = 0;
// 		hash_combine(h, v.x);
// 		hash_combine(h, v.y);
// 		return h;
// 	}
// };

// template <typename T>
// struct std::hash<vec<3, T>> {
// 	size_t operator()(const vec<3, T>& v) const {
// 		size_t h = 0;
// 		hash_combine(h, v.x);
// 		hash_combine(h, v.y);
// 		hash_combine(h, v.z);
// 		return h;
// 	}
// };

// template <typename T>
// struct std::hash<vec<4, T>> {
// 	size_t operator()(const vec<4, T>& v) const {
// 		size_t h = 0;
// 		hash_combine(h, v.x);
// 		hash_combine(h, v.y);
// 		hash_combine(h, v.z);
// 		hash_combine(h, v.w);
// 		return h;
// 	}
// };

template <typename T>
vec<3, T> operator*(T left, const vec<3, T>& right){
	return right*left;
}
template <typename T>
vec<3, T> operator*(T left, const vec<3, T>&& right){
	return right*left;
}

template <typename T>
vec<4, T> operator*(T left, const vec<4, T>& right){
	return right*left;
}
template <typename T>
vec<4, T> operator*(T left, const vec<4, T>&& right){
	return right*left;
}

template <typename T>
T dot(vec<3, T>& left, vec<3, T>& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z;
}
template <typename T>
T dot(vec<3, T>& left, vec<3, T>&& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z;
}
template <typename T>
T dot(vec<3, T>&& left, vec<3, T>& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z;
}
template <typename T>
T dot(vec<3, T>&& left, vec<3, T>&& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z;
}
template <typename T>
T dot(vec<4, T>& left, vec<4, T>& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}
template <typename T>
T dot(vec<4, T>& left, vec<4, T>&& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}
template <typename T>
T dot(vec<4, T>&& left, vec<4, T>& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}
template <typename T>
T dot(vec<4, T>&& left, vec<4, T>&& right) {
	return left.x * right.x + left.y * right.y + left.z * right.z + left.w * right.w;
}


typedef vec<2, float> vec2;
typedef vec<3, float> vec3;
typedef vec<4, float> vec4;

template<size_t c, size_t r, typename T>
struct mat {};

template<typename T>
struct mat<3, 3, T> {
	mat() : cols{} {
		cols[0][0] = 1;
		cols[1][1] = 1;
		cols[2][2] = 1;
	}
	mat(T const& val) : cols{} {
		cols[0][0] = val;
		cols[1][1] = val;
		cols[2][2] = val;
	}
	mat(vec<3, T>& x,
		vec<3, T>& y,
		vec<3, T>& z) : cols{} {
		cols[0] = vec<3, T>( x[0], y[0], z[0] );
		cols[1] = vec<3, T>( x[1], y[1], z[1] );
		cols[2] = vec<3, T>( x[2], y[2], z[2] );
	}
	mat(vec<3, T>&& x,
		vec<3, T>&& y,
		vec<3, T>&& z) : cols{} {
		cols[0] = vec<3, T>(x[0], y[0], z[0]);
		cols[1] = vec<3, T>(x[1], y[1], z[1]);
		cols[2] = vec<3, T>(x[2], y[2], z[2]);
	}
	mat(T const& x1, T const& y1, T const& z1,
		T const& x2, T const& y2, T const& z2,
		T const& x3, T const& y3, T const& z3) : cols{} {
		cols[0][0] = x1; cols[1][0] = y1; cols[2][0] = z1;
		cols[0][1] = x2; cols[1][1] = y2; cols[2][1] = z2;
		cols[0][2] = x3; cols[1][2] = y3; cols[2][2] = z3;
	}
	vec<3,T>& operator[](size_t i) {
		return cols[i];
	}
	float* data() {
		return &cols[0][0];
	}
	vec<3,T> operator*(vec<3, T>& right) {
		return vec<3, T>(
			cols[0][0] * right[0] + cols[1][0] * right[1] + cols[2][0] * right[2],
			cols[0][1] * right[0] + cols[1][1] * right[1] + cols[2][1] * right[2],
			cols[0][2] * right[0] + cols[1][2] * right[1] + cols[2][2] * right[2]);
	}
	vec<3, T> operator*(vec<3, T>&& right) {
		return vec<3, T>(
			cols[0][0] * right[0] + cols[1][0] * right[1] + cols[2][0] * right[2],
			cols[0][1] * right[0] + cols[1][1] * right[1] + cols[2][1] * right[2],
			cols[0][2] * right[0] + cols[1][2] * right[1] + cols[2][2] * right[2]);
	}

	mat<3, 3, T> operator+(mat<3, 3, T>& right) {
		mat<3, 3, T> sum{};
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				sum[i][j] = cols[i][j] + right[i][j];
			}
		}
		return sum;
	}
	mat<3, 3, T> operator+(mat<3, 3, T>&& right) {
		mat<3, 3, T> sum{};
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				sum[i][j] = cols[i][j] + right[i][j];
			}
		}
		return sum;
	}
	mat<3, 3, T> operator-(mat<3, 3, T>* right) {
		
		mat<3, 3, T> sum{};
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				sum[i][j] = cols[i][j] - right[i][j];
			}
		}
		return sum;
	}
	mat<3, 3, T> operator-(mat<3, 3, T>&& right) {
		mat<3, 3, T> sum{};
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				sum[i][j] = cols[i][j] - right[i][j];
			}
		}
		return sum;
	}
	mat<3, 3, T> operator+(const T& a) {
		mat<3, 3, T> sum(*this);
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				sum[i][j] += a;
			}
		}
		return sum;
	}
	mat<3, 3, T> operator+(T&& a) {
		mat<3, 3, T> sum(*this);
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				sum[i][j] += a;
			}
		}
		return sum;
	}
	mat<3, 3, T> operator-(const T& a) {
		return this->operator+(-a);
	}
	mat<3, 3, T> operator-(T&& a) {
		return this->operator+(-a);
	}

	mat<3, 3, T> operator*(mat<3, 3, T>& right) {
		mat<3, 3, T> pdt;
		for (int i{}; i < 3; i++) {
			vec<3, T> row(cols[0][i], cols[1][i], cols[2][i]);
			for (int j{}; j < 3; j++) {
				pdt[j][i] = row * right[j];
			}
		}
		return pdt;
	}
	mat<3, 3, T> operator*(mat<3, 3, T>&& right) {
		mat<3, 3, T> pdt;
		for (int i{}; i < 3; i++) {
			vec<3, T> row(cols[0][i], cols[1][i], cols[2][i]);
			for (int j{}; j < 3; j++) {
				pdt[j][i] = row * right[j];
			}
		}
		return pdt;
	}
	mat<3, 3, T> operator*(const T& a) {
		mat<3, 3, T> output(*this);
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				output[i][j] *= a;
			}
		}
		return output;
	}
	mat<3, 3, T> operator*(T&& a) {
		mat<3, 3, T> output(*this);
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				output[i][j] *= a;
			}
		}
		return output;
	}
	mat<3, 3, T> operator/(const T& a) {
		return this->operator*(1 / a);
	}
	mat<3, 3, T> operator/(T&& a) {
		return this->operator*(1 / a);
	}
private:
	vec<3, vec<3, T>> cols;
};
template<typename T>
struct mat<4, 4, T> {
	mat() : cols{} {
		cols[0][0] = 1;
		cols[1][1] = 1;
		cols[2][2] = 1;
		cols[3][3] = 1;
	}
	mat(T const& val) : cols{} {
		cols[0][0] = val;
		cols[1][1] = val;
		cols[2][2] = val;
		cols[3][3] = val;
	}
	mat(vec<4, T>& x,
		vec<4, T>& y,
		vec<4, T>& z,
		vec<4, T>& w) : cols{} {
		cols[0] = x;
		cols[1] = y;
		cols[2] = z;
		cols[3] = w;
	}
	mat(vec<4, T>&& x,
		vec<4, T>&& y,
		vec<4, T>&& z,
		vec<4, T>&& w) : cols{} {
		cols[0] = x;
		cols[1] = y;
		cols[2] = z;
		cols[3] = w;
	}
	mat(T const& x1, T const& y1, T const& z1, T const& w1,
		T const& x2, T const& y2, T const& z2, T const& w2,
		T const& x3, T const& y3, T const& z3, T const& w3,
		T const& x4, T const& y4, T const& z4, T const& w4) : cols{} {
		cols[0][0] = x1; cols[1][0] = y1; cols[2][0] = z1; cols[3][0] = w1;
		cols[0][1] = x2; cols[1][1] = y2; cols[2][1] = z2; cols[3][1] = w2;
		cols[0][2] = x3; cols[1][2] = y3; cols[2][2] = z3; cols[3][2] = w3;
		cols[0][3] = x4; cols[1][3] = y4; cols[2][3] = z4; cols[3][3] = w4;
	}
	vec<4, T>& operator[](size_t i) {
		return cols[i];
	}
	float* data() {
		return &cols[0][0];
	}
	vec<4, T> operator*(vec<4, T>& right) {
		return vec<4, T>(
			cols[0][0] * right[0] + cols[1][0] * right[1] + cols[2][0] * right[2] + cols[3][0] * right[3],
			cols[0][1] * right[0] + cols[1][1] * right[1] + cols[2][1] * right[2] + cols[3][1] * right[3],
			cols[0][2] * right[0] + cols[1][2] * right[1] + cols[2][2] * right[2] + cols[3][2] * right[3],
			cols[0][3] * right[0] + cols[1][3] * right[1] + cols[2][3] * right[2] + cols[3][3] * right[3]);
	}
	vec<4, T> operator*(vec<4, T>&& right) {
		return vec<4, T>(
			cols[0][0] * right[0] + cols[1][0] * right[1] + cols[2][0] * right[2] + cols[3][0] * right[3],
			cols[0][1] * right[0] + cols[1][1] * right[1] + cols[2][1] * right[2] + cols[3][1] * right[3],
			cols[0][2] * right[0] + cols[1][2] * right[1] + cols[2][2] * right[2] + cols[3][2] * right[3],
			cols[0][3] * right[0] + cols[1][3] * right[1] + cols[2][3] * right[2] + cols[3][3] * right[3]);
	}

	mat<4, 4, T> operator+(mat<4, 4, T>& right) {
		mat<4, 4, T> sum{};
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				sum[i][j] = cols[i][j] + right[i][j];
			}
		}
		return sum;
	}
	mat<4, 4, T> operator+(mat<4, 4, T>&& right) {
		mat<4, 4, T> sum{};
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				sum[i][j] = cols[i][j] + right[i][j];
			}
		}
		return sum;
	}
	mat<4, 4, T> operator-(mat<4, 4, T>& right) {
		mat<4, 4, T> sum{};
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				sum[i][j] = cols[i][j] - right[i][j];
			}
		}
		return sum;
	}
	mat<4, 4, T> operator-(mat<4, 4, T>&& right) {
		mat<4, 4, T> sum{};
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				sum[i][j] = cols[i][j] - right[i][j];
			}
		}
		return sum;
	}
	mat<4, 4, T> operator+(const T& a) {
		mat<4, 4, T> sum(*this);
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				sum[i][j] += a;
			}
		}
		return sum;
	}
	mat<4, 4, T> operator+(T&& a) {
		mat<4, 4, T> sum(*this);
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				sum[i][j] += a;
			}
		}
		return sum;
	}
	mat<4, 4, T> operator-(const T& a) {
		return this->operator+(-a);
	}
	mat<4, 4, T> operator-(T&& a) {
		return this->operator+(-a);
	}

	mat<4, 4, T> operator*(mat<4, 4, T>& right) {
		mat<4, 4, T> pdt;
		for (int i{}; i < 4; i++) {
			vec<4, T> row(cols[0][i], cols[1][i], cols[2][i], cols[3][i]);
			for (int j{}; j < 4; j++) {
				pdt[j][i] = dot(row, right[j]);
			}
		}
		return pdt;
	}
	mat<4, 4, T> operator*(mat<4, 4, T>&& right) {
		mat<4, 4, T> pdt;
		for (int i{}; i < 4; i++) {
			vec<4, T> row(cols[0][i], cols[1][i], cols[2][i], cols[3][i]);
			for (int j{}; j < 4; j++) {
				pdt[j][i] = dot(row, right[j]);
			}
		}
		return pdt;
	}
	mat<4, 4, T> operator*(const T& a) {
		mat<4, 4, T> output(*this);
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				output[i][j] *= a;
			}
		}
		return output;
	}
	mat<4, 4, T> operator*(T&& a) {
		mat<4, 4, T> output(*this);
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				output[i][j] *= a;
			}
		}
		return output;
	}
	mat<4, 4, T> operator/(const T& a) {
		return this->operator*(1 / a);
	}
	mat<4, 4, T> operator/(T&& a) {
		return this->operator*(1 / a);
	}
private:
	vec<4, vec<4, T>> cols;
};

typedef mat<3, 3, float> mat3;
typedef mat<4, 4, float> mat4;

//Translation matrices
template <typename T>
mat<4, 4, T> translate(mat<4, 4, T>& in, vec<3, T>& tv) {
	return mat<4, 4, T>(
		in[0][0], in[1][0], in[2][0], in[3][0] + tv.x,
		in[0][1], in[1][1], in[2][1], in[3][1] + tv.y,
		in[0][2], in[1][2], in[2][2], in[3][2] + tv.z,
		in[0][3], in[1][3], in[2][3], in[3][3]);
}
template <typename T>
mat<4, 4, T> translate(mat<4, 4, T>& in, vec<3, T>&& tv) {
	return mat<4, 4, T>(
		in[0][0], in[1][0], in[2][0], in[3][0] + tv.x,
		in[0][1], in[1][1], in[2][1], in[3][1] + tv.y,
		in[0][2], in[1][2], in[2][2], in[3][2] + tv.z,
		in[0][3], in[1][3], in[2][3], in[3][3]);
}
template <typename T>
mat<4, 4, T> translate(mat<4, 4, T>&& in, vec<3, T>& tv) {
	return mat<4, 4, T>(
		in[0][0], in[1][0], in[2][0], in[3][0] + tv.x,
		in[0][1], in[1][1], in[2][1], in[3][1] + tv.y,
		in[0][2], in[1][2], in[2][2], in[3][2] + tv.z,
		in[0][3], in[1][3], in[2][3], in[3][3]);
}
template <typename T>
mat<4, 4, T> translate(mat<4, 4, T>&& in, vec<3, T>&& tv) {
	return mat<4, 4, T>(
		in[0][0], in[1][0], in[2][0], in[3][0] + tv.x,
		in[0][1], in[1][1], in[2][1], in[3][1] + tv.y,
		in[0][2], in[1][2], in[2][2], in[3][2] + tv.z,
		in[0][3], in[1][3], in[2][3], in[3][3]);
}

//Scale matrices
template <typename T>
mat<4,4,T> scale(mat<4,4,T>& in, vec<3,T>& s) {
	mat<4,4,T> out{in};
	out[0] = out[0] * s[0];
	out[1] = out[1] * s[1];
	out[2] = out[2] * s[2];
	return out;
}
template <typename T>
mat<4, 4, T> scale(mat<4, 4, T>& in, vec<3, T>&& s) {
	mat<4, 4, T> out{ in };
	out[0] = out[0] * s[0];
	out[1] = out[1] * s[1];
	out[2] = out[2] * s[2];
	return out;
}
template <typename T>
mat<4, 4, T> scale(mat<4, 4, T>&& in, vec<3, T>& s) {
	mat<4, 4, T> out{ in };
	out[0] = out[0] * s[0];
	out[1] = out[1] * s[1];
	out[2] = out[2] * s[2];
	return out;
}
template <typename T>
mat<4, 4, T> scale(mat<4, 4, T>&& in, vec<3, T>&& s) {
	mat<4, 4, T> out{ in };
	out[0] = out[0] * s[0];
	out[1] = out[1] * s[1];
	out[2] = out[2] * s[2];
	return out;
}

template <typename T>
mat<4, 4, T> rotate2D(mat<4, 4, T>& in, float a) {
	mat<4, 4, T> out{in};
	float c = cosf(a), s = sinf(a);
	out[0][0] = c*in[0][0]; out[1][0] = -s*in[1][1];
	out[3][0] = c * in[3][0] - s * in[3][1];
	out[0][1] = s*in[0][0]; out[1][1] =  c*in[1][1];
	out[3][1] = s * in[3][0] + c * in[3][1];
	return out;
}
template <typename T>
mat<4, 4, T> rotate2D(mat<4, 4, T>&& in, float a) {
	mat<4, 4, T> out{ in };
	float c = cosf(a), s = sinf(a);
	out[0][0] = c * in[0][0]; out[1][0] = -s * in[1][1];
	out[3][0] = c * in[3][0] - s * in[3][1];
	out[0][1] = s * in[0][0]; out[1][1] = c * in[1][1];
	out[3][1] = s * in[3][0] + c * in[3][1];
	return out;
}

template<typename T>
vec<3, T> normalize(vec<3, T>& v) {
	T len = v.length();
	return len ? v / len : vec<3, T>(0);
}
template<typename T>
vec<3, T> normalize(vec<3, T>&& v) {
	T len = v.length();
	return len ? v / len : vec<3, T>(0);
}

template<typename T>
vec<4, T> normalize(vec<4, T>& v) {
	T len = v.length();
	return len ? v / len : vec<4, T>(0);
}
template<typename T>
vec<4, T> normalize(vec<4, T>&& v) {
	T len = v.length();
	return len ? v / len : vec<4, T>(0);
}

template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>& in, float ang, vec<3,T>& axis) {
	vec<3, T> norm = normalize(axis);
	ang = radians(ang);
	T a = sinf(ang / 2) * norm.x,
		b = sinf(ang / 2) * norm.y,
		c = sinf(ang / 2) * norm.z,
		d = cosf(ang / 2);
	mat<4, 4, T> rot = mat<4, 4, T>(
		1 - 2 * (b * b + c * c), 2 * (a * b - c * d), 2 * (a * c + b * d), 0,
		2 * (a * b + c * d), 1 - 2 * (a * a + c * c), 2 * (b * c - a * d), 0,
		2 * (a * c - b * d), 2 * (b * c + a * d), 1 - 2 * (a * a + b * b), 0,
		0, 0, 0, 1
		);
	return rot * in;
}
template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>& in, float ang, vec<3, T>&& axis) {
	vec<3, T> norm = normalize(axis);
	ang = radians(ang);
	T a = sinf(ang / 2) * norm.x,
		b = sinf(ang / 2) * norm.y,
		c = sinf(ang / 2) * norm.z,
		d = cosf(ang / 2);
	mat<4, 4, T> rot = mat<4, 4, T>(
		1-2*(b*b+c*c), 2*(a*b-c*d),   2*(a*c+b*d),   0,
		2*(a*b+c*d),   1-2*(a*a+c*c), 2*(b*c-a*d),   0,
		2*(a*c-b*d),   2*(b*c+a*d),   1-2*(a*a+b*b), 0,
		0,             0,             0,             1
		);
	return rot*in;
}
template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>&& in, float ang, vec<3, T>& axis) {
	vec<3, T> norm = normalize(axis);
	ang = radians(ang);
	T a = sinf(ang / 2) * norm.x,
		b = sinf(ang / 2) * norm.y,
		c = sinf(ang / 2) * norm.z,
		d = cosf(ang / 2);
	mat<4, 4, T> rot = mat<4, 4, T>(
		1 - 2 * (b * b + c * c), 2 * (a * b - c * d), 2 * (a * c + b * d), 0,
		2 * (a * b + c * d), 1 - 2 * (a * a + c * c), 2 * (b * c - a * d), 0,
		2 * (a * c - b * d), 2 * (b * c + a * d), 1 - 2 * (a * a + b * b), 0,
		0, 0, 0, 1
		);
	return rot * in;
}
template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>&& in, float ang, vec<3, T>&& axis) {
	vec<3, T> norm = normalize(axis);
	ang = radians(ang);
	T a = sinf(ang / 2) * norm.x,
		b = sinf(ang / 2) * norm.y,
		c = sinf(ang / 2) * norm.z,
		d = cosf(ang / 2);
	mat<4, 4, T> rot = mat<4, 4, T>(
		1 - 2 * (b * b + c * c), 2 * (a * b - c * d), 2 * (a * c + b * d), 0,
		2 * (a * b + c * d), 1 - 2 * (a * a + c * c), 2 * (b * c - a * d), 0,
		2 * (a * c - b * d), 2 * (b * c + a * d), 1 - 2 * (a * a + b * b), 0,
		0, 0, 0, 1
		);
	return rot * in;
}

template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>& in, vec<3, T>& angles) {
	T a = radians(angles.x), b = radians(angles.y), c = radians(angles.z);
	T sa = sinf(a/2), ca = cosf(a/2),
		sb = sinf(b/2), cb = cosf(b/2),
		sc = sinf(c/2), cc = cosf(c/2);
	vec<3, T> axis (
		ca*sb*sc + sa*cb*cc,
		ca*sb*cc + sa*cb*sc,
		ca*cb*sc - sa*sb*cc
	);
	float angle = degrees(2*acosf(ca*cb*cc-sa*sb*sc));
	return rotate(in, angle, axis);
}

template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>& in, vec<3, T>&& angles) {
	T a = radians(angles.x), b = radians(angles.y), c = radians(angles.z);
	T sa = sinf(a/2), ca = cosf(a/2),
		sb = sinf(b/2), cb = cosf(b/2),
		sc = sinf(c/2), cc = cosf(c/2);
	vec<3, T> axis (
		ca*sb*sc + sa*cb*cc,
		ca*sb*cc + sa*cb*sc,
		ca*cb*sc - sa*sb*cc
	);
	float angle = degrees(2*acosf(ca*cb*cc-sa*sb*sc));
	return rotate(in, angle, axis);
}

template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>&& in, vec<3, T>& angles) {
	T a = radians(angles.x), b = radians(angles.y), c = radians(angles.z);
	T sa = sinf(a/2), ca = cosf(a/2),
		sb = sinf(b/2), cb = cosf(b/2),
		sc = sinf(c/2), cc = cosf(c/2);
	vec<3, T> axis (
		ca*sb*sc + sa*cb*cc,
		ca*sb*cc + sa*cb*sc,
		ca*cb*sc - sa*sb*cc
	);
	float angle = degrees(2*acosf(ca*cb*cc-sa*sb*sc));
	return rotate(in, angle, axis);
}

template <typename T>
mat<4, 4, T> rotate(mat<4, 4, T>&& in, vec<3, T>&& angles) {
	T a = radians(angles.x), b = radians(angles.y), c = radians(angles.z);
	T sa = sinf(a/2), ca = cosf(a/2),
		sb = sinf(b/2), cb = cosf(b/2),
		sc = sinf(c/2), cc = cosf(c/2);
	vec<3, T> axis (
		ca*sb*sc + sa*cb*cc,
		ca*sb*cc + sa*cb*sc,
		ca*cb*sc - sa*sb*cc
	);
	float angle = degrees(2*acosf(ca*cb*cc-sa*sb*sc));
	return rotate(in, angle, axis);
}

template <typename T>
mat<4, 4, T> ortho(T l, T r, T b, T t, T n, T f) {
	return mat<4, 4, T>(
		2 / (r-l),       0,            0,           -(r+l)/(r-l),
		0,           -2 / (t-b),        0,           (t+b)/(t-b),
		0,           0,           -1 / (f - n),  		-n/(f-n),
		0,           0,            0,            1
		);
}

template<typename T>
mat<4, 4, T> perspective(T fov, T ratio, T n, T f) {
	fov = radians(fov);
	T t = std::tan(fov / 2);
	T r = t * ratio;
	return mat<4, 4, T>(
		1 / r, 0,     	0,                  0,
		0,     1 / t, 	0,                  0,
		0,     0,    		(f+n) / (f - n),		-2 * f * n / (f - n),
		0,     0,    		1,                 	0
		);
}

template<typename T>
mat<4, 4, T> lookAt(vec<3, T>& pos, vec<3, T>& target, vec<3, T>& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}
template<typename T>
mat<4, 4, T> lookAt(vec<3, T>& pos, vec<3, T>& target, vec<3, T>&& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}
template<typename T>
mat<4, 4, T> lookAt(vec<3, T>& pos, vec<3, T>&& target, vec<3, T>& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}
template<typename T>
mat<4, 4, T> lookAt(vec<3, T>& pos, vec<3, T>&& target, vec<3, T>&& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}
template<typename T>
mat<4, 4, T> lookAt(vec<3, T>&& pos, vec<3, T>& target, vec<3, T>& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}
template<typename T>
mat<4, 4, T> lookAt(vec<3, T>&& pos, vec<3, T>& target, vec<3, T>&& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}
template<typename T>
mat<4, 4, T> lookAt(vec<3, T>&& pos, vec<3, T>&& target, vec<3, T>& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}
template<typename T>
mat<4, 4, T> lookAt(vec<3, T>&& pos, vec<3, T>&& target, vec<3, T>&& up) {
	vec<3, T> dir = normalize(target - pos);
	vec<3, T> right = normalize(cross(dir, up));
	vec<3, T> cUp = normalize(cross(right, dir));
	return mat<4, 4, T>(
		right.x, right.y, right.z, 0,
		cUp.x, cUp.y, cUp.z, 0,
		dir.x, dir.y, dir.z, 0,
		0, 0, 0, 1
		) * mat<4, 4, T>(
			1, 0, 0, -pos.x,
			0, 1, 0, -pos.y,
			0, 0, 1, -pos.z,
			0, 0, 0, 1
			);
}


template <typename T>
vec<3, T> cross(vec<3, T>& left, vec<3, T>& right) {
	return vec<3, T>(
		left.y * right.z - right.y * left.z,
		left.z * right.x - right.z * left.x,
		left.x * right.y - right.x * left.y
		);
}
template <typename T>
vec<3, T> cross(vec<3, T>& left, vec<3, T>&& right) {
	return vec<3, T>(
		left.y * right.z - right.y * left.z,
		left.z * right.x - right.z * left.x,
		left.x * right.y - right.x * left.y
		);
}
template <typename T>
vec<3, T> cross(vec<3, T>&& left, vec<3, T>& right) {
	return vec<3, T>(
		left.y * right.z - right.y * left.z,
		left.z * right.x - right.z * left.x,
		left.x * right.y - right.x * left.y
		);
}
template <typename T>
vec<3, T> cross(vec<3, T>&& left, vec<3, T>&& right) {
	return vec<3, T>(
		left.y * right.z - right.y * left.z,
		left.z * right.x - right.z * left.x,
		left.x * right.y - right.x * left.y
		);
}

template <typename T>
float det(mat<2, 2, T>& m) {
	return m[0][0] * m[1][1] - m[1][0] * m[0][1];
}
template <typename T>
float det(mat<2, 2, T>&& m) {
	return m[0][0] * m[1][1] - m[1][0] * m[0][1];
}
template <typename T>
float det(mat<3, 3, T>& m) {
	return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
		- m[1][0] * (m[0][1] * m[2][2] - m[0][2] * m[2][1])
		+ m[2][0] * (m[0][1] * m[1][2] - m[0][2] * m[1][1]);
}
template <typename T>
float det(mat<3, 3, T>&& m) {
	return m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1]
		- m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1]
		+ m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];
}
template<typename T>
float det(mat<4, 4, T>& m) {
	return m[0][0] * (
		m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2])
		- m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2])
		+ m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
		)
		- m[1][0] * (
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2])
			- m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2])
			+ m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			)
		+ m[2][0] * (
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2])
			- m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2])
			+ m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			)
		- m[3][0] * (
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
			- m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			+ m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
}
template<typename T>
float det(mat<4, 4, T>&& m) {
	return m[0][0] * (
		m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2])
		- m[2][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2])
		+ m[3][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
		)
		- m[1][0] * (
			m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2])
			- m[2][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2])
			+ m[3][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			)
		+ m[2][0] * (
			m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2])
			- m[1][1] * (m[0][2] * m[3][3] - m[0][3] * m[3][2])
			+ m[3][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			)
		- m[3][0] * (
			m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2])
			- m[1][1] * (m[0][2] * m[2][3] - m[0][3] * m[2][2])
			+ m[2][1] * (m[0][2] * m[1][3] - m[0][3] * m[1][2])
			);
}

template <typename T>
mat<4, 4, T> inverse(mat<4, 4, T>& m) {
	T c00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	T c02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	T c03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	T c04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	T c06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	T c07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	T c08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	T c10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	T c11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	T c12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	T c14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	T c15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	T c16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	T c18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	T c19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	T c20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	T c22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	T c23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	vec<4, T> F0(c00, c00, c02, c03);
	vec<4, T> F1(c04, c04, c06, c07);
	vec<4, T> F2(c08, c08, c10, c11);
	vec<4, T> F3(c12, c12, c14, c15);
	vec<4, T> F4(c16, c16, c18, c19);
	vec<4, T> F5(c20, c20, c22, c23);

	vec<4, T> V0(m[1][0], m[0][0], m[0][0], m[0][0]);
	vec<4, T> V1(m[1][1], m[0][1], m[0][1], m[0][1]);
	vec<4, T> V2(m[1][2], m[0][2], m[0][2], m[0][2]);
	vec<4, T> V3(m[1][3], m[0][3], m[0][3], m[0][3]);

	vec<4, T> I0(V1 * F0 - V2 * F1 + V3 * F2);
	vec<4, T> I1(V0 * F0 - V2 * F3 + V3 * F4);
	vec<4, T> I2(V0 * F1 - V1 * F3 + V3 * F5);
	vec<4, T> I3(V0 * F2 - V1 * F4 + V2 * F5);

	vec<4, T> S0(+1, -1, +1, -1);
	vec<4, T> S1(-1, +1, -1, +1);

	mat<4, 4, T> Inverse(I0 * S0, I1 * S1, I2 * S0, I3 * S1);

	vec<4, T> R0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	T D = dot(m[0], R0);
	return Inverse*(1/D);
}
template <typename T>
mat<4, 4, T> inverse(mat<4, 4, T>&& m) {
	T c00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
	T c02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
	T c03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

	T c04 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
	T c06 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
	T c07 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

	T c08 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
	T c10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
	T c11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

	T c12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
	T c14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
	T c15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

	T c16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
	T c18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
	T c19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

	T c20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
	T c22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
	T c23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	vec<4, T> F0(c00, c00, c02, c03);
	vec<4, T> F1(c04, c04, c06, c07);
	vec<4, T> F2(c08, c08, c10, c11);
	vec<4, T> F3(c12, c12, c14, c15);
	vec<4, T> F4(c16, c16, c18, c19);
	vec<4, T> F5(c20, c20, c22, c23);

	vec<4, T> V0(m[1][0], m[0][0], m[0][0], m[0][0]);
	vec<4, T> V1(m[1][1], m[0][1], m[0][1], m[0][1]);
	vec<4, T> V2(m[1][2], m[0][2], m[0][2], m[0][2]);
	vec<4, T> V3(m[1][3], m[0][3], m[0][3], m[0][3]);

	vec<4, T> I0(V1 * F0 - V2 * F1 + V3 * F2);
	vec<4, T> I1(V0 * F0 - V2 * F3 + V3 * F4);
	vec<4, T> I2(V0 * F1 - V1 * F3 + V3 * F5);
	vec<4, T> I3(V0 * F2 - V1 * F4 + V2 * F5);

	vec<4, T> S0(+1, -1, +1, -1);
	vec<4, T> S1(-1, +1, -1, +1);

	mat<4, 4, T> Inverse(I0 * S0, I1 * S1, I2 * S0, I3 * S1);

	vec<4, T> R0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

	T D = dot(m[0], R0);
	return Inverse * (1 / D);
}

template <typename T>
T clamp(T val, T low, T high) {
	if (val < low) val = low;
	if (val > high) val = high;
	return val;
}

template <typename T>
vec<2, T> clamp(vec<2, T> val, vec<2, T> low, vec<2, T> high) {
	return vec<2, T>(clamp(val.x, low.x, high.x), clamp(val.y, low.y, high.y));
}

template <typename T>
vec<3, T> clamp(vec<3, T> val, vec<3, T> low, vec<3, T> high) {
	return vec<3, T>(clamp(val.x, low.x, high.x), clamp(val.y, low.y, high.y),
		clamp(val.z, low.z, high.z));
}

template <typename T>
vec<4, T> clamp(vec<4, T> val, vec<4, T> low, vec<4, T> high) {
	return vec<4, T>(clamp(val.x, low.x, high.x), clamp(val.y, low.y, high.y),
		clamp(val.z, low.z, high.z), clamp(val.w, low.w, high.w));
}

//Complex numbers
struct complex {
	float r;
	float i;
	complex(float r = 0, float i = 0) : r{ r }, i{ i } {}
	complex operator+(complex right) {
		return complex(r + right.r, i + right.i);
	}
	complex operator+(vec2 right) {
		return complex(r + right.x, i + right.y);
	}
	complex operator-(complex right) {
		return complex(r - right.r, i - right.i);
	}
	complex operator-(vec2 right) {
		return complex(r - right.x, i - right.y);
	}
	complex operator*(complex right) {
		return complex(r * right.r - i * right.i, r * right.i + i * right.r);
	}
	complex operator*(vec2 right) {
		return complex(r * right.x - i * right.y, r * right.y + i * right.x);
	}
};
