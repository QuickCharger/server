#ifndef _NETPROTOCOL_MESSAGECODE_H_
#define _NETPROTOCOL_MESSAGECODE_H_

#define GROUPLENGTH 999

enum EMsgCodeGrouping
{
	//eCommonMsgCode 1->1000
	eCommonMessageCode_BEGIN = 1,
	eCommonMessageCode_END = eCommonMessageCode_BEGIN + GROUPLENGTH,

	//eServerMessageCode 1001->2000
	eServerMessageCode_BEGIN = eCommonMessageCode_END + 1,
	eServerMessageCode_END = eServerMessageCode_BEGIN + GROUPLENGTH,
	//eClientMessageCode 2001->3000
	eClientMessageCode_BEGIN = eServerMessageCode_END + 1,
	eClientMessageCode_END = eClientMessageCode_BEGIN + GROUPLENGTH,
};

#undef GROUPLENGTH

#endif
