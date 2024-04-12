/*
* DEFINE_PTR_BASE is base ptr macro which not provide reference count etc.
* may memory leak
*/
#ifndef _BASE_MACRO_H_
#define _BASE_MACRO_H_

#define DEFINE_PTR_BASE(TYPE, NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
TYPE	m_p##NAME = DEFAULT_VALUE; \
public: \
const TYPE	FUNC_GET()				{ return m_p##NAME; } \
void		FUNC_SET(TYPE a_pValue)	{ m_p##NAME = a_pValue; } \
private:

/*
* danger to use
* may low efficiency
* may memory leak
* may crash ?
*/
#define DEFINE_TYPE_BASE(TYPE, NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
TYPE	NAME = DEFAULT_VALUE; \
public: \
const TYPE	FUNC_GET()			{ return NAME; } \
void		FUNC_SET(TYPE a_Value)	{ NAME = a_Value; } \
private:

#define DEFINE_TYPE_REFER(TYPE, NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
TYPE	NAME = DEFAULT_VALUE; \
public: \
const TYPE&	FUNC_GET()			{ return NAME; } \
void		FUNC_SET(const TYPE& a_Value)	{ NAME = a_Value; } \
private:

#define DEFINE_BOOL(NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
bool	m_b##NAME = DEFAULT_VALUE; \
public: \
bool	GFUNC_GET()				{ return m_b##NAME; } \
void	FUNC_SET(bool a_bValue)	{ m_b##NAME = a_bValue; } \
private:

#define DEFINE_CHAR(NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
char	m_c##NAME = DEFAULT_VALUE; \
public: \
char	FUNC_GET()				{ return m_c##NAME; } \
void	FUNC_SET(char a_fValue)	{ m_c##NAME = a_cValue; } \
private:

#define DEFINE_INT(NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
int		m_n##NAME = DEFAULT_VALUE; \
public: \
int		FUNC_GET()				{ return m_n##NAME; } \
void	FUNC_SET(int a_nValue)	{ m_n##NAME = a_nValue; } \
private:

#define DEFINE_FLOAT(NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
float	m_f##NAME = DEFAULT_VALUE; \
public: \
float	FUNC_GET				{ return m_f##NAME; } \
void	FUNC_SET(float a_fValue){ m_f##NAME = a_fValue; } \
private:

#define DEFINE_DOUBLE(NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
DOUBLE	m_f##NAME = DEFAULT_VALUE; \
public: \
double	GET_FUNC()					{ return m_f##NAME; } \
void	SET_FUNC(double a_fValue)	{ m_f##NAME = a_fValue; } \
private:

#define DEFINE_LONG_LONG(NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
long long	m_l##NAME = DEFAULT_VALUE; \
public: \
long long	FUNC_GET()						{ return m_l##NAME; } \
void		FUNC_SET(long long a_lValue)	{ m_l##NAME = a_lValue; } \
private:

#define DEFINE_STRING(NAME, DEFAULT_VALUE, FUNC_GET, FUNC_SET)\
private: \
std::string m_str##NAME = DEFAULT_VALUE; \
public: \
const	std::string& FUNC_GET()					{ return m_str##NAME; } \
void	FUNC_SET(const std::string& a_strValue)	{ m_str##NAME = a_strValue; } \
private:

//#define UNDEF_DEFINE #undef DEFINE_BOOL\
//	#undef DEFINE_CHAR\
//	#undef DEFINE_INT\
//	#undef DEFINE_FLOAT\
//	#undef DEFINE_DOUBLE\
//	#undef DEFINE_LONG_LONG\
//#undef UNDEF_DEFINE

#endif
