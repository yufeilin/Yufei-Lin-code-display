#ifndef _EFFECT
#define _EFFECT

#include"Global.h" 

#include "tinyxml2\tinyxml2.h"

using namespace tinyxml2;

// 效果基类
class Effect
{
public:
	Effect();
	virtual ~Effect();
	virtual Effect* ReloadNewEffect() { return new Effect(); }
	virtual void LoadInfoFromXML(XMLElement* XMLEffectInfo) {}
	virtual void ExcuteEffect() {}
	//virtual void ExcuteEffect(Unit SourceUnit, GameObject SourcePiece, GameObject SourceCheckerBoard, Unit TargetUnit, GameObject TargetPiece, GameObject TargetCheckerBoard, BattleInfoSet ParaBattleInfoSet = null, BattleSequence ParaBattleSequence = null) {}
	//virtual object GetParameter(string ParaName) {}
	//virtual void SetParameter(string ParaName, string ParaValue){}
	virtual void DebugDisplay() {}
	ParameterDictionary& GetParameterDictionary() { return EffectParameterDictionary; }

private:
	ParameterDictionary EffectParameterDictionary;
};

Effect::Effect()
{
}

Effect::~Effect()
{
}

// 衍生效果类 伤害
class DealDamage : public Effect
{
public:
	DealDamage();
	~DealDamage();

	Effect* ReloadNewEffect()override { return new DealDamage(); }
	void LoadInfoFromXML(XMLElement* XMLEffectInfo)override;
	void DebugDisplay()override {}

private:

};

DealDamage::DealDamage()
{
	GetParameterDictionary().Add("EffectID", string());
	GetParameterDictionary().Add("EffectName", string());
	GetParameterDictionary().Add("DamageAmount", 0.0f);
	GetParameterDictionary().Add("ExecutionProbability", 1.0f);

}

DealDamage::~DealDamage()
{
}

void DealDamage::LoadInfoFromXML(XMLElement * XMLEffectSubInfo)
{
	for (XMLElement* XMLEffectSubInfoItem = XMLEffectSubInfo->FirstChildElement(); XMLEffectSubInfoItem != NULL; XMLEffectSubInfoItem = XMLEffectSubInfoItem->NextSiblingElement())
	{
		GetParameterDictionary().SetStringParameter(XMLEffectSubInfoItem->Value(), XMLEffectSubInfoItem->FirstChild()->ToText()->Value());
	}
}

#endif