#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cml/cml.h>

namespace geometry
{
	struct sTriangle
	{
		cml::vector3f A, B, C;
		cml::vector3f Na,Nb,Nc,N;

		GLfloat u[3],v[3];
		
		cml::vector3f projectedPoint;

		sTriangle()
		{
		}

		sTriangle(cml::vector3f &A, cml::vector3f &B, cml::vector3f &C, cml::vector3f &Na, cml::vector3f &Nb, cml::vector3f &Nc)
		{
			this->A=A;
			this->B=B;
			this->C=C;
			this->Na=Na;
			this->Nb=Nb;
			this->Nc=Nc;
			this->N=calculateNormal(true);
		}

		sTriangle(cml::vector3f &A, cml::vector3f &B, cml::vector3f &C)
		{
			this->A=A;
			this->B=B;
			this->C=C;
			Na=cml::vector3f();
			Nb=cml::vector3f();
			Nc=cml::vector3f();
		}

		GLvoid setUV(GLfloat u[], GLfloat v[])
		{
			memcpy(this->u,u,sizeof(GLfloat)*3);
			memcpy(this->v,v,sizeof(GLfloat)*3);
		}

		cml::vector3f calculateNormal(bool normalize)
		{			
			N=cml::cross(B-A,C-A);

			if (normalize)
			{
				N.normalize();
			}

			return N;
		}

		cml::vector3f calculateNormalAverage(bool normalize)
		{
			cml::vector3f tmp=(Na+Nb+Nc)/3.0f;

			if (normalize)
			{
				tmp.normalize();
			}

			return tmp;
		}

		GLfloat calculateD(cml::vector3f &N)
		{
			return	-A[0]*N[0]
					-A[1]*N[1]
					-A[2]*N[2];
		}

		GLvoid set(cml::vector3f &A, cml::vector3f &B, cml::vector3f &C)
		{
			this->A=A;
			this->B=B;
			this->C=C;
		}
	};

	struct sReferenceTriangle
	{
		GLint iA,iB,iC;

		sReferenceTriangle()
		{
			iA=iB=iC=-1;
		}

		sReferenceTriangle(GLint iA, GLint iB, GLint iC)
		{
			this->iA=iA;
			this->iB=iB;
			this->iC=iC;
		}

		sReferenceTriangle(GLint *iABC)
		{
			this->iA=iABC[0];
			this->iB=iABC[1];
			this->iC=iABC[2];
		}

		sReferenceTriangle(WORD *iABC)
		{
			this->iA=iABC[0];
			this->iB=iABC[1];
			this->iC=iABC[2];
		}
	};
};

#endif // TRIANGLE_H