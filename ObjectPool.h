#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <stack>
#include <vector>

namespace game_utils
{
	template <class X>
	class CObjectPool
	{
	public:

		CObjectPool(bool autoCreateWhenEmpty)
		{
			this->autoCreateWhenEmpty = autoCreateWhenEmpty;
			increaseFactor = 5;
		}

		~CObjectPool()
		{
			while (!objects.empty())
			{
				delete objects.top();
				objects.pop();
			}

			objectList.clear();
		}

		/*
			Creates 'count' objects of same type. This requires a copy
			constructor to be implemented for T.

			The pool will contain count+1 objects.
		*/
		void addObjects(X *object, int count, bool addFather=true)
		{
			if (addFather)
			{
				objects.push(object);
				objectList.push_back(object);
			}

			for (int i=0; i<count; i++)
			{
				objects.push(new X(*object));
				objectList.push_back(objects.top());
			}
		}

		/*
			Whenever we need an object, we call this method.
			If there is an object available, we will get it.
			If there's none, we get NULL.
		*/
		X *popObject()
		{
			if (objects.empty())
			{
				return NULL;
			}
			else
			{
				X *object = objects.top();
				objects.pop();

				if (objects.size()==1 && autoCreateWhenEmpty)
				{
					addObjects(object,increaseFactor,false);
				}
				
				return object;
			}
		}

		/*
			When the object is no more needed, the owner
			must return it to the pool.
		*/
		void pushObject(X *object)
		{
			objects.push(object);
		}

		std::vector< X* > *getObjectList()
		{
			return &objectList;
		}

	private:
		
		std::stack< X* >	objects;
		std::vector< X* >	objectList; // list of all created resources. just for "safe keeping"

		/* 
			If we're trying to get a resource that has beed depleated,
			the pool can automatically create new instances. If this is
			se to true, the number of newely create objects will be increased 
			by increaseFactor.
		*/
		bool	autoCreateWhenEmpty;

		int		increaseFactor;
	};
}

#endif // OBJECT_POOL_H