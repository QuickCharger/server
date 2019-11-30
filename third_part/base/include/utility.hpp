#pragma once

template<typename T, bool get = true, bool set = true >
class Property
{
public:
	Property()
	{
	}

	Property(const T& t)
	{
		this->t = t;
	}

	Property(T&& t)
	{
		std::swap(this->t, t);
	}

	virtual ~Property()
	{
	}

	/*
	* get
	*/
	T& operator()()
	{
		if (get)
		{
			return this->t;
		}
		static T t;
		return t;
	}

	/*
	* set
	*/
	void operator=(const T& t)
	{
		if (set)
		{
			this->t = t;
		}
	}

	/*
	* set move
	*/
	void operator=(T&& t)
	{
		if (set)
		{
			std::swap(this->t, t);
		}
	}

	/*
	* like JS
	*/
	T& valueOf()
	{
		return t;
	}
private:
	T t;
};
