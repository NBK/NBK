#include <iostream>
#include <fstream>
#include <windows.h>
#include <gl/gl.h>
#include <string>
#include "MilkshapeModel.h"
#include "Vector.h"
#include "Matrix.h"

using namespace std;

/*
	Converts MS3D to BR5 model for simpler in-game usage. 
*/

int main(int argc, char **argv)
{
	if (argc<2)
	{
		cout << "Wrong number of params!" << endl;
		return -1;
	}

	//string ms3dFile = "knight.ms3d"; /// just temp
	string ms3dFile = argv[1];
	bool transparent = false;
	bool animated = (argc==3);

	MilkshapeModel model(transparent,animated,false);

	int numFrames = 0;
	
	if (animated)
	{
		numFrames = atoi(argv[2]);
	}

	if (animated)
	{
		model.numFrames = numFrames;
	}

	if (!model.loadModelData(ms3dFile.c_str()))
	{
		return -1;
	}

	cout << "Converting: " << ms3dFile << endl;

	// we loaded the model successfully

	string br5Name = ms3dFile.substr(0,ms3dFile.length()-4)+"br5";

	FILE *br5 = fopen(br5Name.c_str(),"wb");

	char *mark = "BR5";

	fwrite(mark,1,3,br5);

	// write the texture name
	char textureName[256];
	ZeroMemory(textureName,256);
	if (model.m_numMaterials>0)
	{
		char *mtn = model.m_pMaterials[0].m_pTextureFilename;
		memcpy(textureName,mtn,strlen(mtn));
	}
	fwrite(textureName,sizeof(char),256,br5);

	// write the number of vertices
	int vertCnt = model.m_numTriangles*3;
	fwrite(&vertCnt,sizeof(int),1,br5);

	// write the texture coordinates
	float *texCoords = new float[vertCnt*2];
	float *vertCoords = new float[vertCnt*3];
	float *normCoords = new float[vertCnt*3];

	//bool *indexTaken = new bool[model.m_numVertices];

	//ZeroMemory(indexTaken,model.m_numVertices);

	int texCoordIndex = 0;
	int vertNormCoordIndex = 0;

	if (!animated)
	{
		for ( int i = 0; i < model.m_numMeshes; i++ )
		{
			for ( int j = 0; j < model.m_pMeshes[i].m_numTriangles; j++ )
			{
				int triangleIndex = model.m_pMeshes[i].m_pTriangleIndices[j];
				const Model::Triangle* pTri = &model.m_pTriangles[triangleIndex];

				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];

					//if  (!indexTaken[index])
					{
						//indexTaken[index] = true;
						
						texCoords[texCoordIndex+0] = pTri->m_s[k];
						texCoords[texCoordIndex+1] = pTri->m_t[k];
						
						memcpy(vertCoords+vertNormCoordIndex,model.m_pVertices[index].m_location,sizeof(GLfloat)*3);
						memcpy(normCoords+vertNormCoordIndex,pTri->m_vertexNormals[k],sizeof(GLfloat)*3);

						texCoordIndex+=2;
						vertNormCoordIndex+=3;
					}
				}
			}
		}

		fwrite(texCoords,sizeof(GLfloat),vertCnt*2,br5);

		int frames = 1;
		fwrite(&frames,sizeof(int),1,br5);
		fwrite(vertCoords,sizeof(GLfloat),vertCnt*3,br5);
		fwrite(normCoords,sizeof(GLfloat),vertCnt*3,br5);
	}
	else
	{
		for (int i=0; i<numFrames; i++)
		{
			vertNormCoordIndex = 0;
			texCoordIndex = 0;

			model.advanceAnimation();

			for ( int j = 0; j < model.m_pMeshes[0].m_numTriangles; j++ )
			{
				int triangleIndex = model.m_pMeshes[0].m_pTriangleIndices[j];
				const Model::Triangle* pTri = &model.m_pTriangles[triangleIndex];

				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];

					texCoords[texCoordIndex+0] = pTri->m_s[k];
					texCoords[texCoordIndex+1] = pTri->m_t[k];
					texCoordIndex+=2;

					if ( model.m_pVertices[index].m_boneID == -1 )
					{												
						memcpy(vertCoords+vertNormCoordIndex,model.m_pVertices[index].m_location,sizeof(GLfloat)*3);						
					}
					else
					{
						// rotate according to transformation matrix
						const Matrix& final = model.m_pJoints[model.m_pVertices[index].m_boneID].m_final;

						Vector newNormal( pTri->m_vertexNormals[k] );
						newNormal.transform3( final );
						newNormal.normalize();

						//glNormal3fv( newNormal.getVector());

						Vector newVertex( model.m_pVertices[index].m_location );
						newVertex.transform( final );
						//glVertex3fv( newVertex.getVector());

						memcpy(vertCoords+vertNormCoordIndex,newVertex.getVector(),sizeof(GLfloat)*3);
						memcpy(normCoords+vertNormCoordIndex,newNormal.getVector(),sizeof(GLfloat)*3);
					}

					vertNormCoordIndex+=3;
				}
			}

			if (i==0)
			{
				fwrite(texCoords,sizeof(GLfloat),vertCnt*2,br5);
				int frames = numFrames;
				fwrite(&frames,sizeof(int),1,br5);					
			}

			fwrite(vertCoords,sizeof(GLfloat),vertCnt*3,br5);
			fwrite(normCoords,sizeof(GLfloat),vertCnt*3,br5);
		}
	}

	fclose(br5);

	cout << "Done: " << br5Name << endl << endl;

	return 0;
}