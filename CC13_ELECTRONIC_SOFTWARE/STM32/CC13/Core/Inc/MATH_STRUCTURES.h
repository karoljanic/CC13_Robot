#ifndef MATH_STRUCTURES_H_
#define MATH_STRUCTURES_H_

#include <math.h>

template<typename T = float>
class Vector{
	public:
	// vector[x,y,z]
	T x; 	T y; 	T z;
		Vector(){ x = (T) 0.0f; y = (T) 0.0f; z = (T) 0.0f;}
		Vector(T _x, T _y, T _z){ x = _x; y = _y; z = _z;}
		Vector(Vector<T> &v){ x = v.x; y = v.y; z = v.z;}

		T get_x(){ return x;}
		T get_y(){ return y;}
		T get_z(){ return z;}


		// return magnitude^2
		T get_magnitude2(){ return x*x + y*y + z*z;}

		Vector<T> operator - (){x = -x; y = -y; z = -z; return *this;}

		bool operator == (Vector<T> &v){ return(this->x == v.x && this->y == v.y && this->z == v.z);}
		bool operator != (Vector<T> &v){ return (this->x != v.x || this->y != v.y || this->z != v.z);}

		Vector<T> operator + (Vector<T> v){ Vector<T> res(this->x+v.x, this->y+v.y, this->z+v.z); return res;}
		Vector<T> operator - (Vector<T> v){ Vector<T> res(this->x-v.x, this->y-v.y, this->z-v.z); return res;}

		Vector<T> & operator += (Vector<T> v){ this->x = this->x + v.x; this->y = this->y + v.y; this->z = this->z + v.z; return *this;}
		Vector<T> & operator -= (Vector<T> v){ this->x = this->x - v.x; this->y = this->y - v.y; this->z = this->z - v.z; return *this;}

		Vector<T> operator * (T k){ Vector<T> res((T)this->x*(T)k, (T)this->y*(T)k, (T)this->z*(T)k); return res;}
		Vector<T> & operator *= (T k){ this->x = this->x * k; this->y = this->y * k; this->z = this->z  * k; return *this;}

		Vector<T> operator / (T k){ Vector<T> res((T)this->x/(T)k, (T)this->y/(T)k, (T)this->z/(T)k); return res;}
		Vector<T> & operator /= (T k){ this->x = this->x / k; this->y = this->y / k; this->z = this->z / k; return *this;}

		// scalar product
		T operator * (Vector<T> v){ return this->x*v.x + this->y*v.y + this->z*v.z;}

		// vector product
		Vector<T> vector_multiplication(Vector<T> v){ Vector res(this->y*v.z - this->z*v.y,
																	this->z*v.x - this->x*v.z,
																	this->x*v.y - this->y*v.z);
																	return res;}

		void normalize(){
			T m = (T)sqrt((T)get_magnitude2());
			this->x = this->x / m;		this->y = this->y / m;		this->z = this->z / m;}

		Vector<T> get_normalized(){
			T m = (T)sqrt((T)get_magnitude2());
		    Vector<T> res((T)(this->x/m), (T)(this->y/m), (T)(this->z/m)); return res;}

		// cosine of the angle between this and given vectors
		T get_cosine(Vector<T> v1){ return (*this)*v1 / sqrt(this->get_magnitude2()) / sqrt(v1.get_magnitude2());}

		bool are_parallel(Vector<T> v1){ return (vector_multiplication(this, v1) == Vector(0,0,0));}
		bool are_perpendicular(Vector<T> v1){ return ((*this)*v1 == 0);}

	private:
};

template<typename T = float>
class Quaternion{
	public:
	// Quaternion[w,x,y,z]
	T w;	T x;	T y;	T z;

		Quaternion(){ w = (T) 1.0f;	x = (T) 0.0f; y = (T) 0.0f;	z = (T) 0.0f;}
		Quaternion(T _w, T _x, T _y, T _z){ w = _w; x = _x;	y = _y;	z = _z; }
		Quaternion(Quaternion<T> &q){ w = q.w; x = q.x;	y = q.y; z = q.z;}

		T get_w(){ return w;}
		T get_x(){ return x;}
		T get_y(){ return y;}
		T get_z(){ return z;}

		T get_real(){ return w;}
		Vector<T> get_unreal(){ Vector<T> res(x,y,z); return res;}

		T get_determinant() { return w*w + x*x + y*y + z*z;}
		T get_magnitude() { return sqrt(get_determinant());}

	    Quaternion<T>& operator - (){ w = -w; x = -x; y = -y; z = -z; return *this;}

		friend bool operator == (Quaternion<T> q1, Quaternion<T> q2){ return(q1.w==q2.w && q1.x==q2.x && q1.y==q2.y && q1.z==q2.z);}
		friend bool operator != (Quaternion<T> q1, Quaternion<T> q2){ return(q1.w!=q2.w || q1.x!=q2.x || q1.y!=q2.y || q1.z!=q2.z);}

		Quaternion<T> operator + (Quaternion<T> q){ Quaternion<T> res(this->w+q.w, this->x+q.x, this->y+q.y, this->z+q.z); return res;}
		Quaternion<T> operator - (Quaternion<T> q){ Quaternion<T> res(this->w-q.w, this->x-q.x, this->y-q.y, this->z-q.z); return res;}
		Quaternion<T> operator * (Quaternion<T> q){ Quaternion<T> res(this->w*q.w - this->x*q.x - this->y*q.y - this->z*q.z,
																	  this->w*q.y + this->x*q.z + this->y*q.w - this->z*q.x,
																	  this->w*q.x - this->x*q.w + this->y*q.z + this->z*q.y,
																	  this->w*q.z + this->x*q.y - this->y*q.x + this->z*q.w); return res;}
		Quaternion<T> operator / (Quaternion<T> q){ T k = (T)q.get_determinant();
													Quaternion<T> res((this->w*q.w + this->x*q.x + this->y*q.y + this->z*q.z)/k,
																	  (this->w*q.y - this->x*q.z - this->y*q.w + this->z*q.x)/k,
																	  (this->w*q.x + this->x*q.w - this->y*q.z - this->z*q.y)/k,
																	  (this->w*q.z - this->x*q.y + this->y*q.x - this->z*q.w)/k); return res;}

		Quaternion<T> operator += (Quaternion<T> q){ this->w = this->w+q.w; this->x = this->x+q.x; this->y = this->y+q.y; this->z = this->z+q.z; return *this;}
		Quaternion<T> operator -= (Quaternion<T> q){ this->w = this->w-q.w; this->x = this->x-q.x; this->y = this->y-q.y; this->z = this->z-q.z; return *this;}
		Quaternion<T> operator *= (Quaternion<T> q){ this->w = (this->w*q.w - this->x*q.x - this->y*q.y - this->z*q.z);
												     this->x = (this->w*q.y + this->x*q.z + this->y*q.w - this->z*q.x);
													 this->y = (this->w*q.x - this->x*q.w + this->y*q.z + this->z*q.y);
												     this->z = (this->w*q.z + this->x*q.y - this->y*q.x + this->z*q.w); return *this;}
		Quaternion<T> operator /= (Quaternion<T> q){ T k = (T)q.get_determinant();
													this->w = ((this->w*q.w + this->x*q.x + this->y*q.y + this->z*q.z)/k);
													this->x = ((this->w*q.y - this->x*q.z - this->y*q.w + this->z*q.x)/k);
													this->y = ((this->w*q.x + this->x*q.w - this->y*q.z - this->z*q.y)/k);
													this->z = ((this->w*q.z - this->x*q.y + this->y*q.x - this->z*q.w)/k); return *this;}

		Quaternion<T> operator * (T k){ Quaternion<T> res(this->w*k, this->x*k, this->y*k, this->z*k); return res;}
		Quaternion<T> operator *= (T k){ this->w = this->w*k; this->x = this->x*k; this->y = this->y*k; this->z = this->z*k; return *this;}

		Quaternion<T> operator / (T k){ Quaternion<T> res(this->w/k, this->x/k, this->y/k, this->z/k); return res;}
		Quaternion<T> operator /= (T k){ this->w = this->w/k; this->x = this->x/k; this->y = this->y/k; this->z = this->z/k; return *this;}

		Quaternion<T> get_conjugate(){ Quaternion<T> res(this->w,-this->x,-this->y,-this->z); return res;}
		void conjugate(){this->x = -this->x; this->y = -this->y; this->z = -this->z;}

		void normalize(){
			T m = get_magnitude();
			this->w = this->w / m; this->x = this->x / m; this->y = this->y / m; this->z = this->z / m;}

		Quaternion<T> get_normalized(){
			T m = get_magnitude();
			Quaternion<T> res(this->w / m, this->x / m,	this->y / m, this->z / m); return res;}

	private:
};




#endif /* MATH_STRUCTURES_H_ */
