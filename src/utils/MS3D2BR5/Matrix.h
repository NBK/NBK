/*
	Matrix.h

		Matrix manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 9 December 2000
	Last Edited: 8 January 2001

	Please see the file ChangeLog.html for a revision history.
*/

#ifndef PORTALIB3D_MATRIX_H
#define PORTALIB3D_MATRIX_H

#include "Quaternion.h"

#include <string.h>

class Matrix
{
	public:
		/*	Constructor. */
		Matrix();

		/*	Destructor. */
		~Matrix();

		/*	Set to identity. */
		void loadIdentity();

		/*	Set the values of the matrix. */
		void set( const float *matrix );

		/*	Post-multiply by another matrix. */
		void postMultiply( const Matrix& matrix );

		/*	Set the translation of the current matrix. Will erase any previous values. */
		void setTranslation( const float *translation );

		/*	Set the inverse translation of the current matrix. Will erase any previous values. */
		void setInverseTranslation( const float *translation );

		/*	Make a rotation matrix from Euler angles. The 4th row and column are unmodified. */
		void setRotationRadians( const float *angles );

		/*	Make a rotation matrix from Euler angles. The 4th row and column are unmodified. */
		void setRotationDegrees( const float *angles );

		/*	Make a rotation matrix from a quaternion. The 4th row and column are unmodified. */
		void setRotationQuaternion( const Quaternion& quat );

		/*	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified. */
		void setInverseRotationRadians( const float *angles );

		/*	Make an inverted rotation matrix from Euler angles. The 4th row and column are unmodified. */
		void setInverseRotationDegrees( const float *angles );

		/*	Get the matrix data. */
		const float *getMatrix() const { return m_matrix; }

		/*	Translate a vector by the inverse of the translation part of this matrix. */
		void inverseTranslateVect( float *pVect ) const;

		/*	Rotate a vector by the inverse of the rotation part of this matrix. */
		void inverseRotateVect( float *pVect ) const;

	private:
		//	Matrix data, stored in column-major order
		float m_matrix[16];
};

inline void Matrix::set( const float *matrix )
{
	memcpy( m_matrix, matrix, sizeof( float )*16 );
}

inline void Matrix::loadIdentity()
{
	memset( m_matrix, 0, sizeof( float )*16 );
	m_matrix[0] = m_matrix[5] = m_matrix[10] = m_matrix[15] = 1;
}

inline void Matrix::inverseRotateVect( float *pVect ) const
{
	float vec[3];

	vec[0] = pVect[0]*m_matrix[0]+pVect[1]*m_matrix[1]+pVect[2]*m_matrix[2];
	vec[1] = pVect[0]*m_matrix[4]+pVect[1]*m_matrix[5]+pVect[2]*m_matrix[6];
	vec[2] = pVect[0]*m_matrix[8]+pVect[1]*m_matrix[9]+pVect[2]*m_matrix[10];

	memcpy( pVect, vec, sizeof( float )*3 );
}

inline void Matrix::inverseTranslateVect( float *pVect ) const
{
	pVect[0] = pVect[0]-m_matrix[12];
	pVect[1] = pVect[1]-m_matrix[13];
	pVect[2] = pVect[2]-m_matrix[14];
}

#endif // ndef PORTALIB3D_MATRIX_H
