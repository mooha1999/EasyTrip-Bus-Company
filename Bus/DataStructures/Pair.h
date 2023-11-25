#pragma once
template<typename F, typename S>
class Pair
{
	F first;
	S second;
public:
	Pair(F first, S second) {
		this->first = first;
		this->second = second;
	}
	F GetFirst() { return first; }
	S GetSecond() { return second; }
};

