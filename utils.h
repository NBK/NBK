#ifndef UTILS_H
#define UTILS_H

#include <cml/cml.h>
#include "triangle.h"

using namespace std;
using namespace geometry;

namespace utils
{
	#define SAFE_DELETE(x) if (x) { delete x; x=NULL;}
	#define SAFE_DELETE_ARRAY(x) if (x) { delete [] x; x=NULL;}
	#define SAFE_DELETE_ARRAY_ELEMENTS(x2d,w) if (x2d) {for (int x=0; x<(GLint)w; x++) { SAFE_DELETE(x2d[x]); }}
	#define SAFE_DELETE_2D_ARRAY(x2d,h)  if (x2d) {for (int i=0; i<h; i++){SAFE_DELETE_ARRAY(x2d[i])} SAFE_DELETE_ARRAY(x2d)}
	#define SAFE_DELETE_2D_ARRAY_ELEMENTS(x2d,w,h) for (int y=0; y<h; y++) { for (int x=0; x<w; x++) { SAFE_DELETE(x2d[y][x]); } }
	#define SAFE_DELETE_3D_ARRAY(x3d,d,h)  if (x3d) {for (int _i=0; _i<d; _i++){SAFE_DELETE_2D_ARRAY(x3d[_i],h)} SAFE_DELETE_ARRAY(x3d)}
	#define NULL_POINTER_ARRAY(x2d,w) ZeroMemory(x2d,sizeof(void*)*w);
	#define NULL_VEC vector3f(0.0f,0.0f,0.0f)
	#define PI 3.14159265f
	#define TWOPI 2.0f*PI 
	#define UPDA(angle,amount) {angle+=amount; if (angle>360.0f) angle=0; else if (angle<0.0f) angle=359.0f;}
	#define EPSILON 1e-6f

	struct sColor
	{
		GLubyte r,b,g;

		sColor(GLubyte r, GLubyte g, GLubyte b)
		{
			this->r = r;
			this->g = g;
			this->b = b;
		}

		cml::vector3ub getNextColorUB()
		{			
			b++;

			if(b>=255)
			{
				g++;
				b=0;

				if (g>=255)
				{
					r++;
					g=0;

					if (r>=255)
					{
						r=0;
					}
				}
			}

			return cml::vector3ub(r,g,b);
		}		
	};

	struct sGeneralUtils
	{
		static std::string getCurrentDirectory()
		{
			char path[MAX_PATH];
			GetCurrentDirectory(MAX_PATH,path);
			return std::string(path);
		}
	};

	struct sStringUtils
	{
		static std::string trim(std::string str)
		{
			while (str[0]==' ' || str[0]=='\t')
			{
				str=str.substr(1);
			}

			while (str[str.length()-1]==' ' || str[str.length()-1]=='\t')
			{
				str=str.substr(0,str.length()-1);
			}

			return str;
		};

		static GLvoid tokenizeString(std::vector<string> &container, std::string const &in, const char * const delimiters = " \t\n")
		{
			const string::size_type len = in.length();
			string::size_type i = 0;

			while ( i < len )
			{
				// eat leading whitespace
				i = in.find_first_not_of (delimiters, i);
				if (i == string::npos)
					return;   // nothing left but white space

				// find the end of the token
				string::size_type j = in.find_first_of (delimiters, i);

				// push token
				if (j == string::npos) {
					container.push_back (in.substr(i));
					return;
				} else
					container.push_back (in.substr(i, j-i));

				// set up for next loop
				i = j + 1;
			}
		}
	};

	struct sGeometryUtils
	{
		static GLfloat N(GLint i, GLint k, GLfloat u, GLfloat *U)
		{
			if (k==0)
			{
				if (u>=U[i] && u<U[i+1])
				{
					return 1.0f;
				}
				else
				{
					return 0.0f;
				}
			}
			else
			{
				GLfloat a0=u-U[i];
				GLfloat b0=U[i+k]-U[i];
				GLfloat p0=0.0f;

				if (b0==0.0f)
				{
					p0=0.0f;
				}
				else
				{
					p0=a0/b0*N(i,k-1,u,U);
				}

				GLfloat a1=U[i+k+1]-u;
				GLfloat b1=U[i+k+1]-U[i+1];
				GLfloat p1=0.0f;

				if (b1==0.0f)
				{
					p1=0.0f;
				}
				else
				{
					p1=a1/b1*N(i+1,k-1,u,U);
				}

				return p0+p1;
			}
		}

		static GLfloat *calcU(GLint k, GLint numPoints)
		{
			GLint pos=0;	
			GLint lengthA=(k+1);
			GLint lengthB=(k+1);
			GLint lengthBetween=(numPoints-1-k);

			if (lengthBetween<0)
			{
				return NULL;
			}

			GLfloat inc=1.0f/(GLfloat)(lengthBetween+1);

			GLfloat *U = new GLfloat[lengthA+lengthBetween+lengthB];

			for (GLint i=0; i<lengthA; i++)
			{
				U[pos++]=0.0f;
			}

			GLfloat s=inc;
			for (GLint i=0; i<lengthBetween; i++)
			{
				U[pos++]=s;
				s+=inc;
			}

			for (GLint i=0; i<lengthB; i++)
			{
				U[pos++]=1.0f;
			}

			return U;
		}

		static std::vector<cml::vector3f> generateBSplines(std::vector<cml::vector3f> &inPoints, GLint resolution, GLint degree = 3)
		{
			GLfloat *U = calcU(degree,inPoints.size());

			std::vector<cml::vector3f> returnPoints;

			for (GLint r=0; r<resolution; r++)
			{
				cml::vector3f p_s(0.0f,0.0f,0.0f);
				GLfloat p_i=0.0f;
				for (std::vector<cml::vector3f>::iterator iter = inPoints.begin(); iter != inPoints.end(); iter++)
				{
					GLfloat n=N(distance(inPoints.begin(),iter),degree,(GLfloat)r/(GLfloat)resolution,U);
					p_s+=(*iter)*n;
					p_i+=n;
				}
				returnPoints.push_back(p_s/p_i);		
			}
			SAFE_DELETE_ARRAY(U);
			return returnPoints;
		}

		static bool closeEnough(GLfloat f1, GLfloat f2)
		{
			return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
		}

		static GLvoid calcTangentVector(	cml::vector3f &pos1, cml::vector3f &pos2, cml::vector3f &pos3, 
									cml::vector2f &texCoord1, cml::vector2f &texCoord2, cml::vector2f &texCoord3,
									cml::vector3f &tangent)
		{
			cml::vector3f edge1(pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2]);
			cml::vector3f edge2(pos3[0] - pos1[0], pos3[1] - pos1[1], pos3[2] - pos1[2]);

			edge1.normalize();
			edge2.normalize();


			cml::vector2f texEdge1(texCoord2[0] - texCoord1[0], texCoord2[1] - texCoord1[1]);
			cml::vector2f texEdge2(texCoord3[0] - texCoord1[0], texCoord3[1] - texCoord1[1]);

			texEdge1.normalize();
			texEdge2.normalize();

			GLfloat det = (texEdge1[0] * texEdge2[1]) - (texEdge1[1] * texEdge2[0]);

			if (closeEnough(det, 0.0f))
			{
				tangent.set(1.0f, 0.0f, 0.0f);
			}
			else
			{
				det = 1.0f / det;

				tangent[0] = (texEdge2[1] * edge1[0] - texEdge1[1] * edge2[0]) * det;
				tangent[1] = (texEdge2[1] * edge1[1] - texEdge1[1] * edge2[1]) * det;
				tangent[2] = (texEdge2[1] * edge1[2] - texEdge1[1] * edge2[2]) * det;

				tangent.normalize();
			}
		}

		inline static GLfloat squareDistance(cml::vector3f &v0, cml::vector3f &v1)
		{
			return powf(v0[0]-v1[0],2.0f)+powf(v0[1]-v1[1],2.0f)+powf(v0[2]-v1[2],2.0f);
		}

		static GLfloat angleBetweenVectors2D(cml::vector3f &v1, cml::vector3f &v2)
		{
			v1.normalize();
			v2.normalize();
			GLfloat cs = acos(dot(v1,v2));
			cml::vector3f c = cross(v1,v2);
			return ((c[2]>0.0f && c[2]<=1.0f)?360.0f-rad2deg(cs):rad2deg(cs));
		}

		static GLfloat rad2deg(GLfloat rad)
		{
			return (180.0f*rad)/PI;
		}

		static GLfloat deg2rad(GLfloat deg)
		{
			return (PI*deg)/180.0f;
		}

		static bool sameSide(cml::vector3f &p1, cml::vector3f &p2, cml::vector3f &a, cml::vector3f &b)
		{
			cml::vector3f cp1 = cml::cross(b-a, p1-a);
			cml::vector3f cp2 = cml::cross(b-a, p2-a);
			if (cml::dot(cp1, cp2) >= 0.0f) 
			{
				return true;
			}
			else 
			{
				return false;
			}
		}

		static bool pointTest(cml::vector3f &p, cml::vector3f a, cml::vector3f b, cml::vector3f c)
		{
			if (sameSide(p,a, b,c) && sameSide(p,b, a,c) && sameSide(p,c, a,b))
			{
				return true;
			}
			else 
			{
				return false;
			}
		}

		/*
			Calculates the distance from a plane defined by a normal (planeN) and a point (planeP) 
			to a given point in space (P).
		*/
		static float pointPlaneDistance(cml::vector3f planeN, cml::vector3f planeP, cml::vector3f P)
		{
			float planeD = -planeP[0]*planeN[0]-planeP[1]*planeN[1]-planeP[2]*planeN[2];
			return P[0]*planeN[0]+P[1]*planeN[1]+P[2]*planeN[2]+planeD;
		}

		static bool pointInTriangle(cml::vector3f &pnt, sTriangle *tri, GLfloat eps)
		{
			GLfloat D = pnt[0]*tri->N[0] + 
						pnt[1]*tri->N[1] +
						pnt[2]*tri->N[2] + tri->calculateD(tri->N);


			if (fabs(D)<eps)
			{
				bool in = pointTest(pnt,tri->A,tri->B,tri->C);

				if (in)
				{
					cml::vector3f NN = tri->Na;
					NN.normalize();
					tri->projectedPoint=(pnt - NN*D);
				}

				return in;
			}

			return false;
		}

		static bool pointInTriangle2D(cml::vector3f &pnt, sTriangle &tri)
		{
			return pointTest(pnt,tri.A,tri.B,tri.C);
		}

		#define FABS(x) ((float)fabs(x))        /* implement as is fastest on your machine */

		/* if USE_EPSILON_TEST is true then we do a check: 
				if |dv|<EPSILON then dv=0.0;
		else no check is done (which is less robust)
		*/
		#define USE_EPSILON_TEST TRUE  

		/* some macros */
		#define CROSS(dest,v1,v2)                      \
					dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
					dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
					dest[2]=v1[0]*v2[1]-v1[1]*v2[0];

		#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])

		#define SUB(dest,v1,v2) dest[0]=v1[0]-v2[0]; dest[1]=v1[1]-v2[1]; dest[2]=v1[2]-v2[2]; 

		#define ADD(dest,v1,v2) dest[0]=v1[0]+v2[0]; dest[1]=v1[1]+v2[1]; dest[2]=v1[2]+v2[2]; 

		#define MULT(dest,v,factor) dest[0]=factor*v[0]; dest[1]=factor*v[1]; dest[2]=factor*v[2];

		#define SET(dest,src) dest[0]=src[0]; dest[1]=src[1]; dest[2]=src[2]; 

		/* sort so that a<=b */
		#define SORT(a,b)       \
					if(a>b)    \
					{          \
					float c; \
					c=a;     \
					a=b;     \
					b=c;     \
					}

		#define ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1) \
					isect0=VV0+(VV1-VV0)*D0/(D0-D1);    \
					isect1=VV0+(VV2-VV0)*D0/(D0-D2);


		#define COMPUTE_INTERVALS(VV0,VV1,VV2,D0,D1,D2,D0D1,D0D2,isect0,isect1) \
		if(D0D1>0.0f)                                         \
		{                                                     \
			/* here we know that D0D2<=0.0 */                   \
			/* that is D0, D1 are on the same side, D2 on the other or on the plane */ \
			ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
		}                                                     \
		else if(D0D2>0.0f)                                    \
		{                                                     \
			/* here we know that d0d1<=0.0 */                   \
			ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
		}                                                     \
		else if(D1*D2>0.0f || D0!=0.0f)                       \
		{                                                     \
			/* here we know that d0d1<=0.0 or that D0!=0.0 */   \
			ISECT(VV0,VV1,VV2,D0,D1,D2,isect0,isect1);          \
		}                                                     \
		else if(D1!=0.0f)                                     \
		{                                                     \
			ISECT(VV1,VV0,VV2,D1,D0,D2,isect0,isect1);          \
		}                                                     \
		else if(D2!=0.0f)                                     \
		{                                                     \
			ISECT(VV2,VV0,VV1,D2,D0,D1,isect0,isect1);          \
		}                                                     \
		else                                                  \
		{                                                     \
			/* triangles are coplanar */                        \
			return coplanarTriTri(N1,V0,V1,V2,U0,U1,U2);      \
		}



		/* this edge to edge test is based on Franlin Antonio's gem:
		"Faster Line Segment Intersection", in Graphics Gems III,
		pp. 199-202 */ 
		#define EDGE_EDGE_TEST(V0,U0,U1)                      \
		Bx=U0[i0]-U1[i0];                                   \
		By=U0[i1]-U1[i1];                                   \
		Cx=V0[i0]-U0[i0];                                   \
		Cy=V0[i1]-U0[i1];                                   \
		f=Ay*Bx-Ax*By;                                      \
		d=By*Cx-Bx*Cy;                                      \
		if((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f))  \
		{                                                   \
			e=Ax*Cy-Ay*Cx;                                    \
			if(f>0)                                           \
			{                                                 \
			if(e>=0 && e<=f) return 1;                      \
			}                                                 \
			else                                              \
			{                                                 \
			if(e<=0 && e>=f) return 1;                      \
			}                                                 \
		}                                

		#define EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2) \
		{                                              \
		float Ax,Ay,Bx,By,Cx,Cy,e,d,f;               \
		Ax=V1[i0]-V0[i0];                            \
		Ay=V1[i1]-V0[i1];                            \
		/* test edge U0,U1 against V0,V1 */          \
		EDGE_EDGE_TEST(V0,U0,U1);                    \
		/* test edge U1,U2 against V0,V1 */          \
		EDGE_EDGE_TEST(V0,U1,U2);                    \
		/* test edge U2,U1 against V0,V1 */          \
		EDGE_EDGE_TEST(V0,U2,U0);                    \
		}

		#define POINT_IN_TRI(V0,U0,U1,U2)           \
		{                                           \
		float a,b,c,d0,d1,d2;                     \
		/* is T1 completly inside T2? */          \
		/* check if V0 is inside tri(U0,U1,U2) */ \
		a=U1[i1]-U0[i1];                          \
		b=-(U1[i0]-U0[i0]);                       \
		c=-a*U0[i0]-b*U0[i1];                     \
		d0=a*V0[i0]+b*V0[i1]+c;                   \
													\
		a=U2[i1]-U1[i1];                          \
		b=-(U2[i0]-U1[i0]);                       \
		c=-a*U1[i0]-b*U1[i1];                     \
		d1=a*V0[i0]+b*V0[i1]+c;                   \
													\
		a=U0[i1]-U2[i1];                          \
		b=-(U0[i0]-U2[i0]);                       \
		c=-a*U2[i0]-b*U2[i1];                     \
		d2=a*V0[i0]+b*V0[i1]+c;                   \
		if(d0*d1>0.0)                             \
		{                                         \
			if(d0*d2>0.0) return 1;                 \
		}                                         \
		}

		static int coplanarTriTri(float N[3],float V0[3],float V1[3],float V2[3],
							float U0[3],float U1[3],float U2[3])
		{
		float A[3];
		short i0,i1;
		/* first project onto an axis-aligned plane, that maximizes the area */
		/* of the triangles, compute indices: i0,i1. */
		A[0]=fabs(N[0]);
		A[1]=fabs(N[1]);
		A[2]=fabs(N[2]);
		if(A[0]>A[1])
		{
			if(A[0]>A[2])  
			{
				i0=1;      /* A[0] is greatest */
				i1=2;
			}
			else
			{
				i0=0;      /* A[2] is greatest */
				i1=1;
			}
		}
		else   /* A[0]<=A[1] */
		{
			if(A[2]>A[1])
			{
				i0=0;      /* A[2] is greatest */
				i1=1;                                           
			}
			else
			{
				i0=0;      /* A[1] is greatest */
				i1=2;
			}
			}               
			            
			/* test all edges of sTriangle 1 against the edges of sTriangle 2 */
			EDGE_AGAINST_TRI_EDGES(V0,V1,U0,U1,U2);
			EDGE_AGAINST_TRI_EDGES(V1,V2,U0,U1,U2);
			EDGE_AGAINST_TRI_EDGES(V2,V0,U0,U1,U2);
			            
			/* finally, test if tri1 is totally contained in tri2 or vice versa */
			POINT_IN_TRI(V0,U0,U1,U2);
			POINT_IN_TRI(U0,V0,V1,V2);

			return 0;
		}

		static int triTriIntersect(float V0[3],float V1[3],float V2[3],
							float U0[3],float U1[3],float U2[3])
		{
		float E1[3],E2[3];
		float N1[3],N2[3],d1,d2;
		float du0,du1,du2,dv0,dv1,dv2;
		float D[3];
		float isect1[2], isect2[2];
		float du0du1,du0du2,dv0dv1,dv0dv2;
		float vp0,vp1,vp2;
		float up0,up1,up2;
		float b,c,max;

		/* compute plane equation of sTriangle(V0,V1,V2) */
		SUB(E1,V1,V0);
		SUB(E2,V2,V0);
		CROSS(N1,E1,E2);
		d1=-DOT(N1,V0);
		/* plane equation 1: N1.X+d1=0 */

		/* put U0,U1,U2 into plane equation 1 to compute signed distances to the plane*/
		du0=DOT(N1,U0)+d1;
		du1=DOT(N1,U1)+d1;
		du2=DOT(N1,U2)+d1;

		/* coplanarity robustness check */
		#if USE_EPSILON_TEST==TRUE
		if(fabs(du0)<EPSILON) du0=0.0;
		if(fabs(du1)<EPSILON) du1=0.0;
		if(fabs(du2)<EPSILON) du2=0.0;
		#endif
		du0du1=du0*du1;
		du0du2=du0*du2;

		if(du0du1>0.0f && du0du2>0.0f) /* same sign on all of them + not equal 0 ? */
			return 0;                    /* no intersection occurs */

		/* compute plane of sTriangle (U0,U1,U2) */
		SUB(E1,U1,U0);
		SUB(E2,U2,U0);
		CROSS(N2,E1,E2);
		d2=-DOT(N2,U0);
		/* plane equation 2: N2.X+d2=0 */

		/* put V0,V1,V2 into plane equation 2 */
		dv0=DOT(N2,V0)+d2;
		dv1=DOT(N2,V1)+d2;
		dv2=DOT(N2,V2)+d2;

		#if USE_EPSILON_TEST==TRUE
		if(fabs(dv0)<EPSILON) dv0=0.0;
		if(fabs(dv1)<EPSILON) dv1=0.0;
		if(fabs(dv2)<EPSILON) dv2=0.0;
		#endif

		dv0dv1=dv0*dv1;
		dv0dv2=dv0*dv2;
			    
		if(dv0dv1>0.0f && dv0dv2>0.0f) /* same sign on all of them + not equal 0 ? */
			return 0;                    /* no intersection occurs */

		/* compute direction of intersection line */
		CROSS(D,N1,N2);

		/* compute and index to the largest component of D */
		max=fabs(D[0]);
		b=fabs(D[1]);
		c=fabs(D[2]);
		if(b>max) max=b;
		if(c>max) max=c;

		/* this is the simplified projection onto L*/
		vp0=*V0;
		vp1=*V1;
		vp2=*V2;
			
		up0=*U0;
		up1=*U1;
		up2=*U2;

		/* compute interval for sTriangle 1 */
		COMPUTE_INTERVALS(vp0,vp1,vp2,dv0,dv1,dv2,dv0dv1,dv0dv2,isect1[0],isect1[1]);

		/* compute interval for sTriangle 2 */
		COMPUTE_INTERVALS(up0,up1,up2,du0,du1,du2,du0du1,du0du2,isect2[0],isect2[1]);

		SORT(isect1[0],isect1[1]);
		SORT(isect2[0],isect2[1]);

		if(isect1[1]<isect2[0] || isect2[1]<isect1[0]) return 0;
		return 1;
		}

		/*
		*
		*  Three-dimensional Ray-Triangle Overlap Test.
		*  The function returns 1 if the ray defined by its 
		*  origin point 'orig' and a direction vector 'dir'
		*  intersects the sTriangle having vertices 'a', 'b' and 'c'.
		*  If an intersection exists, the triplet (t,u,v), such that
		*  orig + t. dir = (1 - u - v)a + u.b + v.c
		*  is returned.
		*
		*/


		static int rayTriangleIntersection(float orig[3], float dir[3], float a[3], float b[3], float c[3], float *t, float *u, float *v)
		{
		float vect0[3], vect1[3], nvect[3];
		float normal[3];
		float det, inv_det; 

		SUB(vect0, b,a)
		SUB(vect1, c,a)
		CROSS(normal, vect0, vect1);

		/* orientation of the ray with respect to the sTriangle's normal, 
		also used to calculate output parameters*/
		det = - DOT(dir,normal);

			#ifdef TEST_CULL           /* define TEST_CULL if culling is desired */
		if (det < EPSILON) return 0;
			
		/* calculate vector from ray origin to a */
		SUB(vect0,a,orig);
		/* vector used to calculate u and v parameters */
		CROSS(nvect,dir,vect0);
			
		/* calculate vector from ray origin to b*/
		SUB(vect1,b,orig);
		/* calculate unnormalized v parameter and test bounds */
		*v = - DOT(vect1,nvect);
			
		if (*v < 0.0 || *v > det) return 0;	

		/* calculate vector from ray origin to c*/
		SUB(vect1,c,orig);
		/* calculate unnormalized v parameter and test bounds */ 
		*u = DOT(vect1,nvect);

		if (*u < 0.0 || *u + *v > det) return 0;
			
		/* calculate unormalized t parameter */
		*t = - DOT(vect0,normal);

			inv_det = 1.0 / det;
		/* calculate u v t, ray intersects sTriangle */
			*u = *u * inv_det;
			*v = *v * inv_det;
			*t = *t * inv_det;
			
			#else                 /* the non-culling branch */
			
		/* if determinant is near zero, ray is parallel to the plane of sTriangle */
		if (det > -EPSILON && det < EPSILON) return 0; 	
			
		/* calculate vector from ray origin to a */
		SUB(vect0,a,orig);
		/* normal vector used to calculate u and v parameters */
		CROSS(nvect,dir,vect0);

   			inv_det = 1.0f / det;
		/* calculate vector from ray origin to b*/
		SUB(vect1,b,orig);
		/* calculate v parameter and test bounds */
		*v = - DOT(vect1,nvect) * inv_det;
			
		if (*v < 0.0 || *v > 1.0) return 0;	

		/* calculate vector from ray origin to c*/
		SUB(vect1,c,orig);
		/* calculate v parameter and test bounds */ 
		*u = DOT(vect1,nvect) * inv_det;

		if (*u < 0.0 || *u + *v > 1.0) return 0;
			
		/* calculate t, ray intersects sTriangle */
		*t = - DOT(vect0,normal) * inv_det;

		#endif
		return (*t >= 0);
		// return 1;
		}

		#define CLIP_RIGHT	(1<<0)	// cohen-sutherland clipping outcodes
		#define CLIP_LEFT		(1<<1)
		#define CLIP_TOP		(1<<2)
		#define CLIP_BOTTOM	(1<<3)
		#define CLIP_FRONT	(1<<4)
		#define CLIP_BACK		(1<<5)

		// calculates the cohen-sutherland outcode for a point and a bounding box.
		//
		// bbox_min:	min vector of the bounding box
		// bbox_max:	max vector of the bounding box
		// pnt:			the point to check
		//
		// returns:		the outcode
		//
		static unsigned long calcOutcode( cml::vector3f &bbox_min, cml::vector3f &bbox_max, cml::vector3f &pnt )
		{
			unsigned long outcode = 0;

			if( pnt[0] > bbox_max[0] ) {
				outcode |= CLIP_RIGHT;
			} else if( pnt[0] < bbox_min[0] ) {
				outcode |= CLIP_LEFT;
			}
			if( pnt[1] > bbox_max[1] ) {
				outcode |= CLIP_TOP;
			} else if( pnt[1] < bbox_min[1] ) {
				outcode |= CLIP_BOTTOM;
			}
			if( pnt[2] > bbox_max[2] ) {
				outcode |= CLIP_BACK;
			} else if( pnt[2] < bbox_min[2] ) {
				outcode |= CLIP_FRONT;
			}

			return outcode;
		}

		// determines if a linesegment intersects a bounding box. this is based on
		//  the cohen-sutherland line-clipping algorithm.
		//
		// bbox_min:	bounding box min vector
		// bbox_max:	bounding box max vector
		// p1:			end point of line segment
		// p2:			other end point
		// intercept:	(out) the point in/on the bounding box where the intersection 
		//				  occured.  note that this point may not be on the surface of the box.
		//
		// returns:		true if the segment and box intersect.
		//
		static bool collideLineSegmentBoundingBox( cml::vector3f &bbox_min, cml::vector3f &bbox_max, cml::vector3f &p1, cml::vector3f &p2, cml::vector3f &intercept )
		{
			unsigned long outcode1, outcode2;

			outcode1 = calcOutcode( bbox_min, bbox_max, p1 );
			if( outcode1 == 0 ) {
				// point inside bounding box
				intercept = p1;
				return true;
			}

			outcode2 = calcOutcode( bbox_min, bbox_max, p2 );
			if( outcode2 == 0 ) {
				// point inside bounding box
				intercept = p2;
				return true;
			}

			if( (outcode1 & outcode2) > 0 ) {
				// both points on same side of box
				return false;
			}

			// check intersections
			if( outcode1 & (CLIP_RIGHT | CLIP_LEFT) ) {
				if( outcode1 & CLIP_RIGHT ) {
					intercept[0] = bbox_max[0];
				} else {
					intercept[0] = bbox_min[0];
				}
				float x1 = p2[0] - p1[0];
				float x2 = intercept[0] - p1[0];
				intercept[1] = p1[1] + x2 * (p2[1] - p1[1]) / x1;
				intercept[2] = p1[2] + x2 * (p2[2] - p1[2]) / x1;

				if( intercept[1] <= bbox_max[1] && intercept[1] >= bbox_min[1] && intercept[2] <= bbox_max[2] && intercept[2] >= bbox_min[2] ) {
					return true;
				}
			}

			if( outcode1 & (CLIP_TOP | CLIP_BOTTOM) ) {
				if( outcode1 & CLIP_TOP ) {
					intercept[1] = bbox_max[1];
				} else {
					intercept[1] = bbox_min[1];
				}
				float y1 = p2[1] - p1[1];
				float y2 = intercept[1] - p1[1];
				intercept[0] = p1[0] + y2 * (p2[0] - p1[0]) / y1;
				intercept[2] = p1[2] + y2 * (p2[2] - p1[2]) / y1;

				if( intercept[0] <= bbox_max[0] && intercept[0] >= bbox_min[0] && intercept[2] <= bbox_max[2] && intercept[2] >= bbox_min[2] ) {
					return true;
				}
			}

			if( outcode1 & (CLIP_FRONT | CLIP_BACK) ) {
				if( outcode1 & CLIP_BACK ) {
					intercept[2] = bbox_max[2];
				} else {
					intercept[2] = bbox_min[2];
				}
				float z1 = p2[2] - p1[2];
				float z2 = intercept[2] - p1[2];
				intercept[0] = p1[0] + z2 * (p2[0] - p1[0]) / z1;
				intercept[1] = p1[1] + z2 * (p2[1] - p1[1]) / z1;

				if( intercept[0] <= bbox_max[0] && intercept[0] >= bbox_min[0] && intercept[1] <= bbox_max[1] && intercept[1] >= bbox_min[1] ) {
					return true;
				}
			}

			// nothing found
			return false;
		}

		typedef struct {GLfloat xs,ys,xe,ye;} SEGMENT ;

		static bool lineSegmentsIntersection(cml::vector3f &pA, cml::vector3f &pB, cml::vector3f &pC, cml::vector3f &pD, GLfloat &X, GLfloat &Y)
		{
			cml::vector3f DP,QA,QB;
			GLfloat d,la,lb; 

			SEGMENT A = { pA[0] , pA[1], pB[0], pB[1] };
			SEGMENT B = { pC[0] , pC[1], pD[0], pD[1] };

			DP[0] = B.xs - A.xs ; 
			DP[1] = B.ys - A.ys ;

			QA[0] = A.xe - A.xs ; 
			QA[1] = A.ye - A.ys ;

			QB[0] = B.xe - B.xs ; 
			QB[1] = B.ye - B.ys ;

			d  =   QA[1] * QB[0] - QB[1] * QA[0] ;
			la = ( QB[0] * DP[1] - QB[1] * DP[0] ) / d ;
			lb = ( QA[0] * DP[1] - QA[1] * DP[0] ) / d ; 

			X = A.xs + la * QA[0];
			Y = A.ys + la * QA[1]; 

			return (la>=0.0f && la<=1.0f && lb>=0.0f && lb<=1.0f);
		}

		static inline GLfloat crossProduct(cml::vector3f &v1, cml::vector3f &v2)
		{
			return v1[0] * v2[1] - v1[1] * v2[0];
		}

		static bool linesIntersect(cml::vector3f l1s, cml::vector3f l1f, cml::vector3f l2s, cml::vector3f l2f )
		{
			cml::vector3f v1 = l1f - l1s;	 
			cml::vector3f v2 = l2s - l1s;
			cml::vector3f v3 = l2f - l1s;
			
			GLfloat crossProd1 = crossProduct(v1,v2);
			GLfloat crossProd2 = crossProduct(v1,v3);
			
			if ( (crossProd1 > 0 && crossProd2 < 0) || (crossProd1 < 0 && crossProd2 > 0) || (crossProd1 == 0 && crossProd2 == 0) )
			{	    
				v1 = l2s - l2f;		
				v2 = l1s - l2f;
				v3 = l1f - l2f;

				crossProd1 = crossProduct(v1,v2);
				crossProd2 = crossProduct(v1,v3);	
			
				if ( (crossProd1 > 0 && crossProd2 < 0) || (crossProd1 < 0 && crossProd2 > 0) || (crossProd1 == 0 && crossProd2 == 0) )
				{
					return true;
				}
			}
			return false;
		}

		static bool pointInTriangleCircumcircle0(cml::vector3f &point, sTriangle &sTriangle)
		{
			GLfloat a = sTriangle.A[0];
			GLfloat b = sTriangle.A[1];

			GLfloat c = sTriangle.B[0];
			GLfloat d = sTriangle.B[1];

			GLfloat e = sTriangle.C[0];
			GLfloat f = sTriangle.C[1];

			GLfloat k = ((a*a+b*b)*(e-c) + (c*c+d*d)*(a-e) + (e*e+f*f)*(c-a)) / (2.0f*(b*(e-c)+d*(a-e)+f*(c-a)));
			GLfloat h = ((a*a+b*b)*(f-d) + (c*c+d*d)*(b-f) + (e*e+f*f)*(d-b)) / (2.0f*(a*(f-d)+c*(b-f)+e*(d-b)));

			GLfloat r = sqrt(pow(a-h,2) + pow(b-k,2));

			return ((cml::vector3f(h,k,0.0f)-point).length()<=r);
		}

		static bool pointInTriangleCircumcircle1(cml::vector3f &point, sTriangle &sTriangle)
		{
			GLfloat C[3][3];

			C[0][0] = sTriangle.A[0] - point[0];
			C[0][1] = sTriangle.A[1] - point[1];
			C[0][2] = C[0][0] * C[0][0] + C[0][1] * C[0][1];

			C[1][0] = sTriangle.B[0] - point[0];
			C[1][1] = sTriangle.B[1] - point[1];
			C[1][2] = C[1][0] * C[1][0] + C[1][1] * C[1][1];

			C[2][0] = sTriangle.C[0] - point[0];
			C[2][1] = sTriangle.C[1] - point[1];
			C[2][2] = C[2][0] * C[2][0] + C[2][1] * C[2][1];

			GLfloat D =   C[0][0] * (C[1][1] * C[2][2] - C[1][2] * C[2][1])
						+ C[0][1] * (C[1][2] * C[2][0] - C[1][0] * C[2][2])
						+ C[0][2] * (C[1][0] * C[2][1] - C[1][1] * C[2][0]);

			return (D<0.0f);
		}

		static GLdouble Angle2D(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)
		{
			GLdouble dtheta,theta1,theta2;

			theta1 = atan2(y1,x1);
			theta2 = atan2(y2,x2);
			dtheta = theta2 - theta1;

			while (dtheta > PI)
			{
				dtheta -= TWOPI;
			}

			while (dtheta < -PI)
			{
				dtheta += TWOPI;
			}

			return(dtheta);
		}

		static bool pointInsidePolygon(vector<cml::vector3f> &polygon, cml::vector3f &p)
		{
			GLint i;
			GLdouble angle=0;
			GLint n = polygon.size();
			cml::vector3f p1,p2;

			for (i=0;i<n;i++) 
			{
				p1[0] = polygon[i][0] - p[0];
				p1[1] = polygon[i][1] - p[1];
				p2[0] = polygon[(i+1)%n][0] - p[0];
				p2[1] = polygon[(i+1)%n][1] - p[1];
				angle += Angle2D(p1[0],p1[1],p2[0],p2[1]);
			}

			return (abs(angle)>PI);
		}
	};
};
#endif // UTILS_H