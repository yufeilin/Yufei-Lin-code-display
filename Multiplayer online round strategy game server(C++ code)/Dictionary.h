#ifndef _DICTIONARY
#define _DICTIONARY

#include<map>

using namespace std;

template<typename Key, typename Value>
class Dictionary
{
public:
	void Add(Key NewKey, Value NewValue)
	{
		map<Key, Value>::iterator TargetNode;
		TargetNode = MainMap.find(NewKey);
		if (TargetNode == MainMap.end())
		{
			MainMap[NewKey] = NewValue;
			IndexMap[Count] = NewKey;
			Count++;
		}
		else
		{
			abort();
		}
	}

	bool ContainKey(Key SearchKey)
	{
		map<Key, Value>::iterator TargetNode;
		TargetNode = MainMap.find(SearchKey);
		if (TargetNode == MainMap.end())
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	Value& operator[](Key i)
	{
		return MainMap[i];
	}

	Value& GetByIndex(int i)
	{
		return MainMap[IndexMap[i]];
	}

	int GetCount()
	{
		return Count;
	}
private:
	map<Key, Value> MainMap;
	map<int, Key> IndexMap;
	
	int Count = 0;

};

#endif