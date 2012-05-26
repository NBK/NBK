/*
	Model.cpp

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library

#include <math.h>
#include "Vector.h"
#include "Model.h"

#include "Quaternion.h"


Model::Model()
{
	m_numMeshes = 0;
	m_pMeshes = NULL;
	m_numMaterials = 0;
	m_pMaterials = NULL;
	m_numTriangles = 0;
	m_pTriangles = NULL;
	m_numVertices = 0;
	m_pVertices = NULL;
	m_looping=false;
	time=0.0;
	animation_speed=1.0;
	start_frame=0;
	animated=false;
	numFrames=1;
}

Model::~Model()
{
	int i;
	for ( i = 0; i < m_numMeshes; i++ )
		delete[] m_pMeshes[i].m_pTriangleIndices;
	for ( i = 0; i < m_numMaterials; i++ )
		delete[] m_pMaterials[i].m_pTextureFilename;

	m_numMeshes = 0;
	if ( m_pMeshes != NULL )
	{
		delete[] m_pMeshes;
		m_pMeshes = NULL;
	}

	m_numMaterials = 0;
	if ( m_pMaterials != NULL )
	{
		delete[] m_pMaterials;
		m_pMaterials = NULL;
	}

	m_numTriangles = 0;
	if ( m_pTriangles != NULL )
	{
		delete[] m_pTriangles;
		m_pTriangles = NULL;
	}

	m_numVertices = 0;
	if ( m_pVertices != NULL )
	{
		delete[] m_pVertices;
		m_pVertices = NULL;
	}

	if (animated)
	{
		for ( i = 0; i < m_numJoints; i++ )
		{
			delete[] m_pJoints[i].m_pRotationKeyframes;
			delete[] m_pJoints[i].m_pTranslationKeyframes;
		}

		m_numJoints = 0;
		if ( m_pJoints != NULL )
		{
			delete[] m_pJoints;
			m_pJoints = NULL;
		}
	}
}

void Model::draw(float scale) 
{
	if (animated)
	{
		advanceAnimation();

		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glScalef(scale,scale,scale);

		glBindTexture( GL_TEXTURE_2D, m_pMaterials[0].m_texture );

		glBegin( GL_TRIANGLES );
		{
			for ( int j = 0; j < m_pMeshes[0].m_numTriangles; j++ )
			{
				int triangleIndex = m_pMeshes[0].m_pTriangleIndices[j];
				const Triangle* pTri = &m_pTriangles[triangleIndex];

				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];

					// { NEW }
					if ( m_pVertices[index].m_boneID == -1 )
					{
						// same as before
						glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
						glNormal3fv( pTri->m_vertexNormals[k] );
						glVertex3fv( m_pVertices[index].m_location );
					}
					else
					{
						// rotate according to transformation matrix
						const Matrix& final = m_pJoints[m_pVertices[index].m_boneID].m_final;

						glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );

						Vector newNormal( pTri->m_vertexNormals[k] );
						newNormal.transform3( final );
						newNormal.normalize();
						glNormal3fv( newNormal.getVector());

						Vector newVertex( m_pVertices[index].m_location );
						newVertex.transform( final );
						glVertex3fv( newVertex.getVector());
					}
				}
			}
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glScalef(scale,scale,scale);
		for ( int i = 0; i < m_numMeshes; i++ )
		{
			glCallList(model[i]);
		}
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
}

void Model::draw_for_picking(GLfloat scale) 
{
	glPushMatrix();
	glScalef(scale,scale,scale);
	for (meshes=0; meshes<m_numMeshes; meshes++)
		glCallList(model[meshes]);
	glPopMatrix();
}

void Model::setJointKeyframe( int jointIndex, int keyframeIndex, float time, float *parameter, bool isRotation )
{
//	assert( m_numJoints > jointIndex );

	Keyframe& keyframe = isRotation ? m_pJoints[jointIndex].m_pRotationKeyframes[keyframeIndex] :
		m_pJoints[jointIndex].m_pTranslationKeyframes[keyframeIndex];

	keyframe.m_jointIndex = jointIndex;
	keyframe.m_time = time;
	memcpy( keyframe.m_parameter, parameter, sizeof( float )*3 );
}

void Model::setupJoints()
{
	int i;
	for ( i = 0; i < m_numJoints; i++ )
	{
		Joint& joint = m_pJoints[i];

		joint.m_relative.setRotationRadians( joint.m_localRotation );
		joint.m_relative.setTranslation( joint.m_localTranslation );
		if ( joint.m_parent != -1 )
		{
			joint.m_absolute.set( m_pJoints[joint.m_parent].m_absolute.getMatrix());
			joint.m_absolute.postMultiply( joint.m_relative );
		}
		else
			joint.m_absolute.set( joint.m_relative.getMatrix());
	}

	for ( i = 0; i < m_numVertices; i++ )
	{
		Vertex& vertex = m_pVertices[i];

		if ( vertex.m_boneID != -1 )
		{
			const Matrix& matrix = m_pJoints[vertex.m_boneID].m_absolute;

			matrix.inverseTranslateVect( vertex.m_location );
			matrix.inverseRotateVect( vertex.m_location );
		}
	}

	for ( i = 0; i < m_numTriangles; i++ ) {
		Triangle& triangle = m_pTriangles[i];
		for ( int j = 0; j < 3; j++ ) {
			const Vertex& vertex = m_pVertices[triangle.m_vertexIndices[j]];
			if ( vertex.m_boneID != -1 ) {
				const Matrix& matrix = m_pJoints[vertex.m_boneID].m_absolute;
				matrix.inverseRotateVect( triangle.m_vertexNormals[j] );
			}
		}
	}
}

void Model::restart()
{
	for ( int i = 0; i < m_numJoints; i++ )
	{
		m_pJoints[i].m_currentRotationKeyframe = m_pJoints[i].m_currentTranslationKeyframe =0;
		m_pJoints[i].m_final.set( m_pJoints[i].m_absolute.getMatrix());
	}
}

void Model::advanceAnimation()
{
	animation_speed = m_totalTime / numFrames;
	//if (animation_speed>20.0f)
	{		
		time+=animation_speed;

		if ( time > m_totalTime )
		{
			if ( m_looping )
			{
				restart();
				time = (double)(start_frame)*1000.0/animFPS;
			}
			else
				time = m_totalTime;
		} 

		for ( int i = 0; i < m_numJoints; i++ )
		{
			float transVec[3];
			Matrix transform;
			int frame;
			Joint *pJoint = &m_pJoints[i];

			if ( pJoint->m_numRotationKeyframes == 0 && pJoint->m_numTranslationKeyframes == 0 )
			{
				pJoint->m_final.set( pJoint->m_absolute.getMatrix());
				continue;
			}

			frame = pJoint->m_currentTranslationKeyframe;
			while ( frame < pJoint->m_numTranslationKeyframes && pJoint->m_pTranslationKeyframes[frame].m_time < time )
			{
				frame++;
			}
			pJoint->m_currentTranslationKeyframe = frame;

			if ( frame == 0 )
				memcpy( transVec, pJoint->m_pTranslationKeyframes[0].m_parameter, sizeof ( float )*3 );
			else if ( frame == pJoint->m_numTranslationKeyframes )
				memcpy( transVec, pJoint->m_pTranslationKeyframes[frame-1].m_parameter, sizeof ( float )*3 );
			else
			{
	//			assert( frame > 0 && frame < pJoint->m_numTranslationKeyframes );

				const Model::Keyframe& curFrame = pJoint->m_pTranslationKeyframes[frame];
				const Model::Keyframe& prevFrame = pJoint->m_pTranslationKeyframes[frame-1];

				float timeDelta = curFrame.m_time-prevFrame.m_time;
				float interpValue = ( float )(( time-prevFrame.m_time )/timeDelta );

				transVec[0] = prevFrame.m_parameter[0]+( curFrame.m_parameter[0]-prevFrame.m_parameter[0] )*interpValue;
				transVec[1] = prevFrame.m_parameter[1]+( curFrame.m_parameter[1]-prevFrame.m_parameter[1] )*interpValue;
				transVec[2] = prevFrame.m_parameter[2]+( curFrame.m_parameter[2]-prevFrame.m_parameter[2] )*interpValue; 
			}

			frame = pJoint->m_currentRotationKeyframe;
			while ( frame < pJoint->m_numRotationKeyframes && pJoint->m_pRotationKeyframes[frame].m_time < time )
			{
				frame++;
			}
			pJoint->m_currentRotationKeyframe = frame;

			if ( frame == 0 )
				transform.setRotationRadians( pJoint->m_pRotationKeyframes[0].m_parameter );
			else if ( frame == pJoint->m_numRotationKeyframes )
				transform.setRotationRadians( pJoint->m_pRotationKeyframes[frame-1].m_parameter );
			else
			{
	//			assert( frame > 0 && frame < pJoint->m_numRotationKeyframes );

				const Model::Keyframe& curFrame = pJoint->m_pRotationKeyframes[frame];
				const Model::Keyframe& prevFrame = pJoint->m_pRotationKeyframes[frame-1];
				
				float timeDelta = curFrame.m_time-prevFrame.m_time;
				float interpValue = ( float )(( time-prevFrame.m_time )/timeDelta );

	//			assert( interpValue >= 0 && interpValue <= 1 );

				Quaternion qPrev( prevFrame.m_parameter );
				Quaternion qCur( curFrame.m_parameter );
				Quaternion qFinal( qPrev, qCur, interpValue );
				transform.setRotationQuaternion( qFinal );

				/*float rotVec[3];

				rotVec[0] = prevFrame.m_parameter[0]+( curFrame.m_parameter[0]-prevFrame.m_parameter[0] )*interpValue;
				rotVec[1] = prevFrame.m_parameter[1]+( curFrame.m_parameter[1]-prevFrame.m_parameter[1] )*interpValue;
				rotVec[2] = prevFrame.m_parameter[2]+( curFrame.m_parameter[2]-prevFrame.m_parameter[2] )*interpValue;

				transform.setRotationRadians( rotVec );*/
			}

			transform.setTranslation( transVec );
			Matrix relativeFinal( pJoint->m_relative );
			relativeFinal.postMultiply( transform );

			if ( pJoint->m_parent == -1 )
				pJoint->m_final.set( relativeFinal.getMatrix());
			else
			{
				pJoint->m_final.set( m_pJoints[pJoint->m_parent].m_final.getMatrix());
				pJoint->m_final.postMultiply( relativeFinal );
			}
		}
	}
}

void Model::set_total_animation_time(GLint start_frame, GLint end_frame)
{
	time = (double)(start_frame)*1000.0/animFPS;
	m_totalTime = (double)(end_frame)*1000.0/animFPS;
}