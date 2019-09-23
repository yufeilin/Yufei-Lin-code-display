#ifndef _PARAMETERDICTIONARY
#define _PARAMETERDICTIONARY

#include "YufeiBasic.h"

using namespace std;

class Parameter;

class ParameterDictionary
{
public:
	ParameterDictionary();
	~ParameterDictionary();

	void Add(string ParameterID, ParameterType NewParameterType);
	inline void Add(string ParameterID, int NewValue) { ParameterOfName.Add(ParameterID, new IntParameter()); (*(ParameterOfName[ParameterID])).SetInt(NewValue); }
	inline void Add(string ParameterID, float NewValue) { ParameterOfName.Add(ParameterID, new FloatParameter()); (*(ParameterOfName[ParameterID])).SetFloat(NewValue); }
	inline void Add(string ParameterID, string NewValue) { ParameterOfName.Add(ParameterID, new StringParameter()); (*(ParameterOfName[ParameterID])).SetString(NewValue); }
	inline void Add(string ParameterID, bool NewValue) { ParameterOfName.Add(ParameterID, new BoolParameter()); (*(ParameterOfName[ParameterID])).SetBool(NewValue); }

	inline void Set(string ParameterID, int NewValue) { (*(ParameterOfName[ParameterID])).SetInt(NewValue); }
	inline void Set(string ParameterID, float NewValue) { (*(ParameterOfName[ParameterID])).SetFloat(NewValue); }
	inline void Set(string ParameterID, string NewValue) { (*(ParameterOfName[ParameterID])).SetString(NewValue); }
	inline void Set(string ParameterID, bool NewValue) { (*(ParameterOfName[ParameterID])).SetBool(NewValue); }

	string GetStringParameter(string ParameterID) { return (*(ParameterOfName[ParameterID])).GetString(); }
	int GetIntParameter(string ParameterID) { return (*(ParameterOfName[ParameterID])).GetInt(); }
	float GetFloatParameter(string ParameterID) { return (*(ParameterOfName[ParameterID])).GetFloat(); }
	bool GetBoolParameter(string ParameterID) { return (*(ParameterOfName[ParameterID])).GetBool(); }

	void SetStringParameter(string ParameterID, string NewValue) { (*(ParameterOfName[ParameterID])).SetString(NewValue); }
	void SetIntParameter(string ParameterID, int NewValue) { (*(ParameterOfName[ParameterID])).SetInt(NewValue); }
	void SetFloatParameter(string ParameterID, float NewValue) { (*(ParameterOfName[ParameterID])).SetFloat(NewValue); }
	void SetBoolParameter(string ParameterID, bool NewValue) { (*(ParameterOfName[ParameterID])).SetBool(NewValue); }

private:
	Dictionary<string, Parameter *> ParameterOfName;

};

ParameterDictionary::ParameterDictionary()
{
}


ParameterDictionary::~ParameterDictionary()
{
	// 释放所有参数指针指向的内存区空间
	int n = ParameterOfName.GetCount();

	for (int i = 0; i < n; i++)
	{
		Parameter* Tmp = ParameterOfName.GetByIndex(i);

		if (Tmp != NULL)
		{
			delete Tmp;
			ParameterOfName.GetByIndex(i) = NULL;
			Tmp = NULL;
		}
	}
}

void ParameterDictionary::Add(string ParameterID, ParameterType NewParameterType)
{
	switch (NewParameterType)
	{
		case Int:
		{
			ParameterOfName.Add(ParameterID, new IntParameter());
			break;
		}
		case Float:
		{
			ParameterOfName.Add(ParameterID, new FloatParameter());
			break;
		}
		case String:
		{
			ParameterOfName.Add(ParameterID, new StringParameter());
			break;
		}
		case Bool:
		{
			ParameterOfName.Add(ParameterID, new BoolParameter());
			break;
		}
	}
}


#endif