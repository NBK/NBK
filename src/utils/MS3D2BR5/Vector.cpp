/*
	Vector.cpp

		Vector manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 9 December 2000
	Last Edited: 6 February 2001

	Please see the file ChangeLog.html for a revision history.
*/

#include "Vector.h"

Vector::Vector()
{
	reset();
}

Vector::Vector( const float *values )
{
	set( values );
	m_vector[3] = 1;
}

Vector::~Vector()
{
}

void Vector::transform( const Matrix& m )
{
	double vector[4];
	const float *matrix = m.getMatrix();

	vector[0] = m_vector[0]*matrix[0]+m_vector[1]*matrix[4]+m_vector[2]*matrix[8]+matrix[12];
	vector[1] = m_vector[0]*matrix[1]+m_vector[1]*matrix[5]+m_vector[2]*matrix[9]+matrix[13];
	vector[2] = m_vector[0]*matrix[2]+m_vector[1]*matrix[6]+m_vector[2]*matrix[10]+matrix[14];
	vector[3] = m_vector[0]*matrix[3]+m_vector[1]*matrix[7]+m_vector[2]*matrix[11]+matrix[15];

	m_vector[0] = ( float )( vector[0] );
	m_vector[1] = ( float )( vector[1] );
	m_vector[2] = ( float )( vector[2] );
	m_vector[3] = ( float )( vector[3] );
}

void Vector::transform3( const Matrix& m )
{
	double vector[3];
	const float *matrix = m.getMatrix();

	vector[0] = m_vector[0]*matrix[0]+m_vector[1]*matrix[4]+m_vector[2]*matrix[8];
	vector[1] = m_vector[0]*matrix[1]+m_vector[1]*matrix[5]+m_vector[2]*matrix[9];
	vector[2] = m_vector[0]*matrix[2]+m_vector[1]*matrix[6]+m_vector[2]*matrix[10];

	m_vector[0] = ( float )( vector[0] );
	m_vector[1] = ( float )( vector[1] );
	m_vector[2] = ( float )( vector[2] );
	m_vector[3] = 1;
}

