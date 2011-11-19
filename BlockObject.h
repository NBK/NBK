#ifndef BLOCK_OBJECT_H
#define BLOCK_OBJECT_H

#include "BR5Model.h"
#include "Entity.h"
#include "LUA/LUAEffect.h"

/*
	Represents an object that resides no a block. That can be either
	a static (non action object): training room rod, prison bars,
	gold... or a dynamic (action) object: traps...
*/

namespace game_objects
{
	namespace block_objects
	{
		class CBlockObject: public CEntity
		{
		public:
			CBlockObject(std::string name, cml::vector3f position, loaders::CBR5Model *model, GLfloat rotateY = 0.0f);
			~CBlockObject();

			virtual GLvoid					drawModel(GLfloat deltaTime = 1.0f);
			virtual GLvoid					drawEffect();

			virtual std::string				getName();
			virtual std::string				getEffectName();
			virtual std::string				getClassName();
			virtual GLfloat					getRotateY();
			virtual loaders::CBR5Model		*getModel();
			virtual LUA_effects::CLUAEffect	*getEffect();

			virtual GLvoid				setName(std::string name);
			virtual GLvoid				setRotateY(GLfloat rotateY);
			virtual GLvoid				setModel(loaders::CBR5Model	*model);
			virtual GLvoid				setEffect(LUA_effects::CLUAEffect *effect);
			virtual GLvoid				setEffectName(std::string effectName);
			virtual GLvoid				setClassName(std::string className);
			GLvoid						setMarked(bool marked);

			bool						isMarked();

		protected:

			std::string			name,		// name for the model resource
								effectName,	// name for the effect resource
								className;	// not a must to use. only some object use this (traps)
			
			// every block object MUST have a model (that's why the model param is in the constructor)
			loaders::CBR5Model	*model;

			// every block object may have an effect
			LUA_effects::CLUAEffect	*effect;

			// used for statues in the temple
			GLfloat				rotateY;

			// is true, the object will get deleted upon room selling and similar
			bool				marked;			

		};
	};
};

#endif // BLOCK_OBJECT_H