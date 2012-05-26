/*
	Vector.h

		Vector manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 9 December 2000
	Last Edited: 8 January 2001

	Please see the file ChangeLog.html for a revision history.
*/

#ifndef PORTALIB3D_VECTOR_H
#define PORTALIB3D_VECTOR_H

#include "Matrix.h"

class Vector
{
	public:
		/*	Constructor. (0, 0, 0, 1) */
		Vector();

		/*	Constructor. 3 float values. */
		Vector( const float *vector );

		/*	Destructor. */
		~Vector();

		/*	Retrieve vector. */
		const float *getVector() const { return m_vector; }

		/*	Transform the vector by a matrix. */
		void transform( const Matrix& m );

		/*	Transform the vector by a matrix, not including the scaling or transformation elements (use only top-left 3x3 of matrix). */
		void transform3( const Matrix& m );

		/*	Set the values of the vector. Takes 3 float values. */
		void set( const float *vector );

		/*	Translate by another vector. */
		void add( const Vector& v );

		/*	Reset to (0, 0, 0, 1). */
		void reset();

		/*	Get the length of the vector. */
		float length() const;

		/*	Normalize (make it a unit vector). */
		void normalize();

		const float operator[]( int index ) const { return m_vector[index]; }

	private:
		//	Vector data
		float m_vector[4];
};

inline void Vector::reset()
{
	m_vector[0] = m_vector[1] = m_vector[2] = 0;
	m_vector[3] = 1;
}

inline void Vector::set( const float *values )
{
	m_vector[0] = values[0];
	m_vector[1] = values[1];
	m_vector[2] = values[2];
}

inline void Vector::add( const Vector& v )
{
	m_vector[0] += v.m_vector[0];
	m_vector[1] += v.m_vector[1];
	m_vector[2] += v.m_vector[2];
	m_vector[3] += v.m_vector[3];
}

inline void Vector::normalize()
{
	float len = length();

	m_vector[0] /= len;
	m_vector[1] /= len;
	m_vector[2] /= len;
}

inline float Vector::length() const
{
	return ( float )sqrt( m_vector[0]*m_vector[0]+m_vector[1]*m_vector[1]+m_vector[2]*m_vector[2] );
}

#endif // ndef PORTALIB3D_VECTOR_H
