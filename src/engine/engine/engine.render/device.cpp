#include "device.h"

namespace render
{

	device::device(void)
	{
	}

	device::~device(void)
	{
	}

	HRESULT device::Create(const HWND& windowHandle)
	{
		UNREFERENCED_PARAMETER(windowHandle);
		return E_FAIL;
	}

} // render