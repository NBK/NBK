#ifndef BR5MODEL_H
#define BR5MODEL_H

#include "VBO.h"
#include <string>
#include <map>
#include "boundingBox.h"

typedef unsigned char byte;

#define MAX_ACTIONS	32 // a model may have at max 64 different actions (modify value if needed)

namespace loaders
{
	class CBR5Model
	{
	public:
		CBR5Model();

		/*
			Creates a copy that shares vertices, texture and texture coordinates with the original.
			The copy can be animated individually. This way we optimize memory consumption. 

			The only thing that gets recreated is a basic vertex array (size == numVerts*3*float) and
			a VBO render module.
		*/
		CBR5Model(const CBR5Model &br5Model);
		~CBR5Model();

		/*
			Loads model from .br5 model file.
		*/
		bool loadFromFile(std::string fileName);

		/*
			Loads from .md3, temp until we sort out the classes
		*/
		bool loadFromMD3(std::string fileName);

		/*
			Draws the model using VBOs. Using this call
			all the frames are drawn. If the model if animated,
			then deltaTime comes to good use. 
		*/
		void draw(GLfloat deltaTime=1.0f);

		/*
			By setting this you control the speed of animation.
		*/
		void setAnimSpeed(float animSpeed);

		/*
			If the model consists of very few frames or the animation
			looks nonsmooth set this to true. This way the animation
			will get interpolated between 2 frames.
		*/
		void setInterpolate(bool interpolate);

		/*
			For looping animations with non same first
			and last frame this should be set to true.
			This way we set the interpolation between the 
			last frame and the first frame in the animation
		*/
		void setConnected(bool connected);

		/*
			If we for some reason don't want the anim to be updated
			set this to false;
		*/
		void setUpdate(bool update);

		/*
			If we have many different animations in 1 file and we know
			from which to which frame each anim goes then we can register
			an action. Then we can call draw with action param and only
			that part of the anim will get drawn.
		*/
		int registerAction(int startFrame, int endFrame);

		/*
			After we have registered the action we can call this
			function to start displaying a registered action.
		*/
		void doAction(int actionID);

		void scaleToMaxY(float maxY);

		geometry::sBoundingBox *getBoundingBox();

		/*
			Texture name manipulation. Setting the name also reloads the texture 
			and updates the VBO.
		*/
		char *getTextureName();
		void setTextureName(std::string textureName);

		rendering::CVBO *getVBO();

		unsigned int	getVertexCount();
		float			getAnimSpeed();
		float			*getVertexCoordinates();

	private:		

		// variables
		std::string		fileName;

		char			textureName[256];

		unsigned int	vertexCount,
						framesCount,
						currentFrame,
						startFrame,
						endFrame,
						texture;

		float			*textureCoordinates,
						*vertexCoordinates,
						**vertexCoordinatesAnim,
						animSpeed,
						animNextFrame;
						//*normalCoordinates; not there yet

		bool			interpolate,
						update,
						copy,
						connected;

		std::pair<int, int> actions[MAX_ACTIONS];

		int actionsCount;

		geometry::sBoundingBox boundingBox;

		// every model gets rendered using VBOs
		rendering::CVBO *vbo;

		// functions
		long getFileSize(FILE *file);

		void setupTexture();
	};
};

#endif // BR5MODEL_H