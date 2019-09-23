#ifndef _EFFECTLOADER
#define _EFFECTLOADER

#include"Global.h"
#include"YufeiBasic.h"

class EffectLoader
{
public:
	EffectLoader();
	~EffectLoader();
	Effect* GetNewEffectByType(string EffectType);

private:
	Dictionary<string, Effect*> EffectLoaderDictionary;

};

EffectLoader::EffectLoader()
{
	EffectLoaderDictionary.Add("DealDamage", new DealDamage());
	EffectLoaderDictionary.Add("ApplyBehavior", new DealDamage());
	EffectLoaderDictionary.Add("RemoveBehavior", new DealDamage());
	EffectLoaderDictionary.Add("ModifyPlayer", new DealDamage());
}

EffectLoader::~EffectLoader()
{
	int n = EffectLoaderDictionary.GetCount();

	for (int i = 0; i < n; i++)
	{
		Effect* Tmp = EffectLoaderDictionary.GetByIndex(i);
		if (Tmp != NULL)
		{
			delete Tmp;
			EffectLoaderDictionary.GetByIndex(i) = NULL;
			Tmp = NULL;
		}
	}
}

inline Effect * EffectLoader::GetNewEffectByType(string EffectType)
{
	Effect * ReturnEffect = EffectLoaderDictionary[EffectType];

	EffectLoaderDictionary[EffectType] = EffectLoaderDictionary[EffectType]->ReloadNewEffect();

	return ReturnEffect;
}

#endif
