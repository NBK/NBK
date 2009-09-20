#include "BasicLight.h"

using namespace std;

namespace game_objects
{
	namespace lighting
	{
		CBasicLight::CBasicLight(string name): CLightSource(name)
		{
		}

		CBasicLight::CBasicLight(CBasicLight &basicLight): CLightSource(basicLight)
		{
		}

		CBasicLight::~CBasicLight()
		{
		}
	}
}