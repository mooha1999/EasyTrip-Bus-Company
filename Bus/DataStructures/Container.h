#pragma once
//template<typename T>
class Container
{
protected:
	int size = 0;
public:
	bool IsEmpty() { return (size == 0); }
	int Size() { return size; }
};

