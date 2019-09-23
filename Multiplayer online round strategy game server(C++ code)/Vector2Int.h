#ifndef _VECTOR2INT
#define _VECTOR2INT

#include<math.h>

class Vector2Int
{
public:
	int x;
	int y;

	Vector2Int()
	{
		x = 0;
		y = 0;
	}

	Vector2Int(int NewX, int NewY)
	{
		x = NewX;
		y = NewY;
	}

	bool operator ==(Vector2Int V)
	{
		if (x == V.x && y == V.y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// 这个地方要写成const函数，否则会导致该类型无法放入set中
	bool operator < (const Vector2Int V) const
	{
		if (this->x < V.x)
		{
			return true;
		}
		else if (this->x == V.x)
		{
			return this->y < V.y;
		}
		else
		{
			return false;
		}
	}

	Vector2Int operator + (const Vector2Int V) const
	{
		return Vector2Int(this->x + V.x, this->y + V.y);
	}

	Vector2Int& operator=(const Vector2Int &V)
	{
		this->x = V.x;
		this->y = V.y;

		return *this;
	}

	static double Distance(Vector2Int A, Vector2Int B)
	{
		return sqrt(pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
	}
};

#endif