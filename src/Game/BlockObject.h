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
			virtual ~CBlockObject();

			virtual void					drawModel(GLfloat deltaTime = 1.0f);
			virtual void					drawEffect();

			virtual std::string				getName();
			virtual std::string				getEffectName();
			virtual std::string				getClassName();
			virtual GLfloat					getRotateY();
			virtual loaders::CBR5Model		*getModel();
			virtual LUA_effects::CLUAEffect	*getEffect();

			virtual void				setName(std::string name);
			virtual void				setRotateY(GLfloat rotateY);
			virtual void				setModel(loaders::CBR5Model	*model);
			virtual void				setEffect(LUA_effects::CLUAEffect *effect);
			virtual void				setEffectName(std::string effectName);
			virtual void				setClassName(std::string className);
			void						setMarked(bool marked);

			bool						isMarked();

		protected:

			std::string			m_name,		// name for the model resource
								m_effectName,	// name for the effect resource
								m_className;	// not a must to use. only some object use this (traps)
			
			// every block object MUST have a model (that's why the model param is in the constructor)
			loaders::CBR5Model	*m_model;

			// every block object may have an effect
			LUA_effects::CLUAEffect	*m_effect;

			// used for statues in the temple
			GLfloat				m_rotateY;

			// is true, the object will get deleted upon room selling and similar
			bool				m_marked;			

		};
	};
};

#endif // BLOCK_OBJECT_H