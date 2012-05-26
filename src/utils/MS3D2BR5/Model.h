/*
	Model.h

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#ifndef MODEL_H
#define MODEL_H

#include "Matrix.h"

class Model
{
	public:
		//	Mesh
		struct Mesh
		{
			int m_materialIndex;
			int m_numTriangles;
			int *m_pTriangleIndices;
		};

		//	Material properties
		struct Material
		{
			float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
			float m_shininess;
			GLuint m_texture;
			char *m_pTextureFilename;
		};

		//	Triangle structure
		struct Triangle
		{
			float m_vertexNormals[3][3];
			float m_s[3], m_t[3];
			int m_vertexIndices[3];
		};

		//	Vertex structure
		struct Vertex
		{
			char m_boneID;	// for skeletal animation
			float m_location[3];
		};

		//	Animation keyframe information
		struct Keyframe
		{
			int m_jointIndex;
			float m_time;	// in milliseconds
			float m_parameter[3];
		};

		//	Skeleton bone joint
		struct Joint
		{
			float m_localRotation[3];
			float m_localTranslation[3];
			Matrix m_absolute, m_relative;

			int m_numRotationKeyframes, m_numTranslationKeyframes;
			Keyframe *m_pTranslationKeyframes;
			Keyframe *m_pRotationKeyframes;

			int m_currentTranslationKeyframe, m_currentRotationKeyframe;
			Matrix m_final;

			int m_parent;
		};

	public:
		/*	Constructor. */
		Model();

		/*	Destructor. */
		virtual ~Model();

		/*	
			Load the model data into the private variables. 
				filename			Model filename
		*/
		virtual bool loadModelData( const char *filename ) = 0;

		/*
			Draw the model.
		*/
		virtual void draw(float scale);

		/*
			Draw the model for picking
		*/
		void draw_for_picking(GLfloat scale);

		double animation_speed;

	//protected:
		//	Meshes used
		int m_numMeshes;
		Mesh *m_pMeshes;

		//	Materials used
		int m_numMaterials;
		Material *m_pMaterials;

		//	Triangles used
		int m_numTriangles;
		Triangle *m_pTriangles;

		//	Vertices Used
		int m_numVertices;
		Vertex *m_pVertices;

		// at this time the model may consist of max 50 parts
		GLuint model[50];

		bool	transparent,
				animated;

		GLint meshes;

		GLint materialIndex,
				numFrames;

		/*	
			Set the values of a particular keyframe for a particular joint.
				jointIndex		The joint to setup the keyframe for
				keyframeIndex	The maximum number of keyframes
				time			The time in milliseconds of the keyframe
				parameter		The rotation/translation values for the keyframe
				isRotation		Whether it is a rotation or a translation keyframe
		*/
		void setJointKeyframe( int jointIndex, int keyframeIndex, float time, float *parameter, bool isRotation );

		/*	Setup joint matrices. */
		void setupJoints();

		/*	Set looping factor for animation. */
		void setLooping( bool looping ) { m_looping = looping; }

		/*	Advance animation by a frame. */
		void advanceAnimation();

		/*	Restart animation. */
		void restart();

		//	Joint information
		int m_numJoints;
		Joint *m_pJoints;

		//	Total animation time
		double m_totalTime;

		//	Is the animation looping?
		bool m_looping;

		double time;

		// for animation from a frame to a frame! End frame gets set with variable m_totalTime
		GLint	start_frame;

		float animFPS;

		void set_total_animation_time(GLint start_frame, GLint end_frame);
};

#endif // ndef MODEL_H
