/*
* DEFINE_PTR_BASE is base ptr macro which not provide reference count etc.
*/
#define DEFINE_PTR_BASE(TYPE, NAME, DEFAULT_VALUE) private: \
TYPE m_p##NAME = DEFAULT_VALUE; \
public: \
const TYPE Get##NAME()				{ return m_p##NAME; } \
void Set##NAME(TYPE a_pValue)	{ m_p##NAME = a_pValue; } \
private:

/*
* danger to use
* may low efficiency
* may crash ?
*/
#define DEFINE_TYPE_BASE(TYPE, NAME, DEFAULT_VALUE) private: \
TYPE m_##NAME = DEFAULT_VALUE; \
public: \
const TYPE Get##NAME()				{ return m_##NAME; } \
void Set##NAME(TYPE a_Value)	{ m_##NAME = a_Value; } \
private:

#define DEFINE_BOOL(NAME, DEFAULT_VALUE) private: \
bool m_b##NAME = DEFAULT_VALUE; \
public: \
bool Get##NAME()				{ return m_b##NAME; } \
void Set##NAME(bool a_bValue)	{ m_b##NAME = a_bValue; } \
private:

#define DEFINE_CHAR(NAME, DEFAULT_VALUE) private: \
char m_c##NAME = DEFAULT_VALUE; \
public: \
char Get##NAME()				{ return m_c##NAME; } \
void Set##NAME(char a_fValue)	{ m_c##NAME = a_cValue; } \
private:

#define DEFINE_INT(NAME, DEFAULT_VALUE) private: \
int m_n##NAME = DEFAULT_VALUE; \
public: \
int Get##NAME()					{ return m_n##NAME; } \
void Set##NAME(int a_nValue)	{ m_n##NAME = a_nValue; } \
private:

#define DEFINE_FLOAT(NAME, DEFAULT_VALUE) private: \
float m_f##NAME = DEFAULT_VALUE; \
public: \
float Get##NAME()				{ return m_f##NAME; } \
void Set##NAME(float a_fValue)	{ m_f##NAME = a_fValue; } \
private:

#define DEFINE_DOUBLE(NAME, DEFAULT_VALUE) private: \
DOUBLE m_f##NAME = DEFAULT_VALUE; \
public: \
double Get##NAME()				{ return m_f##NAME; } \
void Set##NAME(double a_fValue)	{ m_f##NAME = a_fValue; } \
private:

#define DEFINE_LONG_LONG(NAME, DEFAULT_VALUE) private: \
long long m_l##NAME = DEFAULT_VALUE; \
public: \
long long Get##NAME()				{ return m_l##NAME; } \
void Set##NAME(long long a_lValue)	{ m_l##NAME = a_lValue; } \
private:

#define DEFINE_STRING(NAME, DEFAULT_VALUE) private: \
std::string m_str##NAME = DEFAULT_VALUE; \
public: \
const std::string& Get##NAME()					{ return m_str##NAME; } \
void Set##NAME(const std::string& a_strValue)	{ m_str##NAME = a_strValue; } \
private:

//#define UNDEF_DEFINE #undef DEFINE_BOOL\
//	#undef DEFINE_CHAR\
//	#undef DEFINE_INT\
//	#undef DEFINE_FLOAT\
//	#undef DEFINE_DOUBLE\
//	#undef DEFINE_LONG_LONG\
//#undef UNDEF_DEFINE
