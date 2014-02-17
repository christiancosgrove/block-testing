struct BlockCoord
{
	int x, y, z;
	BlockCoord(int _x, int _y, int _z) : x(_x), y(_y), z(_z) {};

	BlockCoord& operator+=(const BlockCoord& c)
	{
		x+=c.x;
		y+=c.y;
		z+=c.z;
		return *this;
	}
	BlockCoord& operator-=(const BlockCoord& c)
	{
		x-=c.x;
		y-=c.y;
		z-=c.z;
		return *this;
	}
};

inline BlockCoord operator+(BlockCoord l, const BlockCoord& r)
{
	l+=r;
	return l;
}
inline BlockCoord operator-(BlockCoord l, const BlockCoord& r)
{
	l-=r;
	return l;
}