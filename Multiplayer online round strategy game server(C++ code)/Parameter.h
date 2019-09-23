#ifndef _PARAMETER
#define _PARAMETER

#include "YufeiBasic.h"

using namespace std;

//使用用法
//IntParameter tmp;
//int tt = 2;
//cout << &tt << endl;
//tmp.SetParameter(&tt);
//cout << *((int*)(tmp.GetParameter())) << endl;

enum ParameterType
{
	Int = 0,
	Float = 1,
	String = 2,
	Bool = 3,
};

class Parameter
{
public:
	Parameter(){}
	virtual ~Parameter() {}
	virtual void SetParameter(void* NewValue)
	{

	}
	virtual void* GetParameter()
	{
		return NULL;
	}
	virtual void SetInt(int NewValue){}
	virtual void SetFloat(float NewValue){}
	virtual void SetString(string NewValue){}
	virtual void SetBool(bool NewValue){}
	virtual int GetInt() { return 0; }
	virtual float GetFloat() { return 0.0f; }
	virtual bool GetBool() { return false; }
	virtual string GetString() { return ""; }


private:

};


class IntParameter:public Parameter
{
public:
	IntParameter(){}
	~IntParameter(){}
	void SetParameter(void* NewValue)override { Value = *((int*)NewValue); }
	void* GetParameter()override { return &Value; }
	void SetInt(int NewValue)override { Value = NewValue; }
	int GetInt()override { return Value; }
	void SetFloat(float NewValue)override { Value = (int)NewValue; }
	float GetFloat()override { return (float)Value; }
	void SetString(string NewValue)override { Value = atoi(NewValue.c_str()); }
	string GetString()override { return to_string(Value); }

private:
	int Value = 0;
};


class FloatParameter:public Parameter
{
public:
	FloatParameter() {}
	~FloatParameter() {}
	void SetParameter(void* NewValue)override { Value = *(float*)NewValue; }
	void* GetParameter()override { return &Value; }
	void SetInt(int NewValue)override { Value = (float)NewValue; }
	int GetInt()override { return (int)Value; }
	void SetFloat(float NewValue)override { Value = NewValue; }
	float GetFloat()override { return Value; }
	void SetString(string NewValue)override { Value = atof(NewValue.c_str()); }
	string GetString()override { return to_string(Value); }

private:
	float Value = 0.0f;
};


class StringParameter :public Parameter
{
public:
	StringParameter() {}
	~StringParameter() {}
	void SetString(string NewValue)override { Value = NewValue; }
	string GetString()override { return Value; }

private:
	string Value = "";
};

class BoolParameter :public Parameter
{
public:
	BoolParameter() {}
	~BoolParameter() {}
	void SetBool(bool NewValue)override { Value = NewValue; }
	bool GetBool()override { return Value; }
	void SetString(string NewValue)override { Value = (NewValue == "true") ? true : false; }
	string GetString()override { return (Value == true) ? "true" : "false"; }

private:
	bool Value = false;
};


#endif