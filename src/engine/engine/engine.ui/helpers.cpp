#include "helpers.h"

namespace ui
{

namespace helpers
{

	RECT MakeRect(float left, float top, float right, float bottom)
	{
		RECT rc;
		rc.left = static_cast<LONG>( left );
		rc.top = static_cast<LONG>( top );
		rc.right = static_cast<LONG>( right );
		rc.bottom = static_cast<LONG>( bottom);
		return rc;
	}

} // helpers

} // ui