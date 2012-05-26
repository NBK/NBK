/*
	Quaternion.h

		Quaternion manipulation.

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://rsn.gamedev.net/pl3d
	Copyright (C)2000, 2001, Brett Porter. All Rights Reserved.
	This source code is released under the LGPL. See license.txt for details.

	Created: 8 January 2001
	Last Edited: 8 January 2001

	Please see the file ChangeLog.html for a revision history.
*/

#ifndef PORTALIB3D_QUATERNION_H
#define PORTALIB3D_QUATERNION_H

#include <math.h>

#ifndef PI
#	define PI	3.1415926535897932384626433832795
#endif

#ifndef PI_ON_180
#	define PI_ON_180	(PI/180.0)
#endif

class Quaternion
{
	public:
		/*	Constructor. Retrieves values from angles. */
		Quaternion( const float *angles ) { fromAngles( angles ); }

		/*	Cosntructor. Retrieves values from an interpolation between two other quaternions. */
		Quaternion( const Quaternion& q1, Quaternion& q2, float interp ) { slerp( q1, q2, interp ); }

		/*
			Set values from angles.
				angles	(x,y,z) Euler rotation angles.
		*/
		void fromAngles( const float *angles );

		/*
			Set values from an interpolation between two other quaternions.
			This will also modify the second quaternion if it is backwards.
				q1, q2	The quaternions to interpolate between
				interp	A value from 0.0-1.0 indicating the linear interpolation parameter.
		*/
		void slerp( const Quaternion& q1, Quaternion& q2, float interp );

		/*	Get a value from the quaternion. */
		float operator[]( int index ) const { return m_quat[index]; }

		/*	Set the inverse of the quaternion. */
		void inverse();

	private:
		//	Quaternion data
		float m_quat[4];
};

inline void Quaternion::inverse()
{
	m_quat[0] = -m_quat[0];
	m_quat[1] = -m_quat[1];
	m_quat[2] = -m_quat[2];
	m_quat[3] = -m_quat[3];
}

#endif // ndef PORTALIB3D_QUATERNION_H
