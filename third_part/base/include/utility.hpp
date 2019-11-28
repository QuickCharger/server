#pragma once

template<typename T, bool get = true, bool set = true >
class Property
{
public:
	T& operator()()
	{
		if (get)
		{
			return this->t;
		}
		static T t;
		return t;
	}

	void operator=(const T& t)
	{
		if (set)
		{
			this->t = t;
		}
	}

	void operator=(T&& t)
	{
		if (set)
		{
			std::swap(this->t, t);
		}
	}

	T& valueOf()
	{
		return t;
	}
private:
	T t;
};
