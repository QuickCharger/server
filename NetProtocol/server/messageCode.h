#include "../messageCode.h"

namespace ServerMessageCode
{
	enum EMessageCode
	{
		eBegin = eServerMessageCode_BEGIN,
		eTest,
		eTest2,
		eTestResponse,
		eEnd = eServerMessageCode_END,
	};
}
