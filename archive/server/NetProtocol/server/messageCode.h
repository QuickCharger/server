#include "../messageCode.h"

namespace ServerMessageCode
{
	enum EMessageCode
	{
		eBegin = eServerMessageCode_BEGIN,
		eRegistServer,
		eRegistServerResqonse,
		eTestResponse,
		eEnd = eServerMessageCode_END,
	};
}
