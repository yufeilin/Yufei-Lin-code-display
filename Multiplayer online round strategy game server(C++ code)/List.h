#ifndef _List
#define _List

#include<list>


template<typename Type>
class List
{
public:
	List()
	{

	}
	~List()
	{

	}
	inline void Add(Type NewElement)
	{
		Count++;
		MainList.push_back(NewElement);
	}
	inline void Insert(int InsertIndex, Type NewElement)
	{
		Count++;
		MainList.insert(list1.begin() + InsertIndex, 1, NewElement);
	}
	void Remove(Type RemoveElement)
	{
		int RemoveIndex = -1;
		for (int i = 0; i < MainList.size(); i++)
		{
			if (MainList[i] == RemoveElement)
			{
				RemoveIndex = i;
			}
		}
		Count -= 1;
		MainList.erase(RemoveIndex);
	}
	inline void RemoveAt(int RemoveIndex)
	{
		if (RemoveIndex < MainList.size())
		{
			Count -= 1;
			MainList.erase(RemoveIndex);
		}
		else
		{
			abort();
		}
	}
	bool Contains(Type RemoveElement)
	{
		for (int i = 0; i < MainList.size(); i++)
		{
			if (MainList[i] == RemoveElement)
			{
				return true;
			}
		}

		return false;
	}

	inline void Clear()
	{
		Count = 0;
		MainList.clear();
	}

	Type& operator[](int i)
	{
		//return MainList[i];
		return *MainList.begin();
	}

	int Count = 0;

private:

	list<Type> MainList;
};



#endif