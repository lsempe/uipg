#include "input.h"
#include <algorithm>

#pragma comment( lib, "dinput8.lib" )

BOOL CALLBACK EnumDeviceCallback(LPCDIDEVICEINSTANCEW instance, LPVOID context);
BOOL CALLBACK CountDevicesCallback(LPCDIDEVICEINSTANCEW instance, LPVOID counter);

namespace input
{

/// input_state

input_state::input_state(HWND hwnd) 
: m_inputObject(0)
, m_mouse(0)
, m_keyboard(0)
, m_gamepad(0)
, m_hwnd(0)
{
	Initialize(hwnd);
}

input_state::~input_state()
{
	m_inputObject->Release();
	m_inputObject = nullptr;
}

std::shared_ptr<keyboard> input_state::CreateKeyboard()
{
	if (m_keyboard)
	{
		return m_keyboard;
	}

	m_keyboard = std::shared_ptr<keyboard>( new keyboard() );
	m_keyboard->Initialize(m_inputObject, m_hwnd, false);
	return m_keyboard;
}

std::shared_ptr<mouse> input_state::CreateMouse()
{
	if (m_mouse)
	{
		return m_mouse;
	}
	m_mouse = std::shared_ptr<mouse>( new mouse() );
	m_mouse->Initialize(m_inputObject, m_hwnd);
	return m_mouse;
}

std::shared_ptr<gamepad> input_state::CreateGamepad()
{
	if (m_gamepad)
	{
		return m_gamepad;
	}

	m_gamepad = std::shared_ptr<gamepad>( new gamepad() );
	m_gamepad->Initialize(m_inputObject, m_hwnd, false);
	return m_gamepad;
}

HRESULT input_state::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;	
	
	return DirectInput8Create(	GetModuleHandle( NULL ), 
								DIRECTINPUT_VERSION, 
								IID_IDirectInput8, 
								(VOID**)&m_inputObject,
								NULL );
}

void input_state::Update(float deltaTime)
{	
	if (m_keyboard)
	{
		m_keyboard->Update(deltaTime);
		HandleKeyboard();
	}

	if (m_mouse)
	{
		m_mouse->Update(deltaTime);
		HandleMouse();
	}

	if (m_gamepad)
	{
		m_gamepad->Update(deltaTime);
		HandleGamepad();
	}
	

//	m_Timer.Frame();
}

/// keyboard handling

void input_state::HandleKeyboard()
{
	for ( auto& it : m_keyboard_handlers )
	{
		if ( it.m_pHandler(m_keyboard, it.m_caller) )
		{
			if ( it.m_priority == IN_EXCLUSIVE )
				break;
		}
	}
}

void input_state::RegisterKeyboardHandler(fnKeyboardHandler pHandler, void* caller, size_t priority)
{
	if ( pHandler )
	{
		keyboard_handler kbh;
		kbh.m_pHandler = pHandler;
		kbh.m_priority = priority;
		kbh.m_caller = caller;
		m_keyboard_handlers.push_back(kbh);

		std::sort(m_keyboard_handlers.begin(), m_keyboard_handlers.end(), ComparePriority);
	}
}

void input_state::UnregisterKeyboardHandler(fnKeyboardHandler pHandler)
{
	if ( pHandler )
	{
		auto it = std::find_if(m_keyboard_handlers.begin(), m_keyboard_handlers.end(), [&pHandler](keyboard_handler& it) { return (it.m_pHandler == pHandler); } );
		if ( it != m_keyboard_handlers.end() )
		{
			it->Enable(false);
			std::sort(m_keyboard_handlers.begin(), m_keyboard_handlers.end(), ComparePriority);
			return;
		}
	}
}

/// mouse handling

void input_state::HandleMouse()
{
	for ( auto& it : m_mouse_handlers )
	{
		if ( it.m_pHandler(m_mouse, it.m_caller) )
		{
			if ( it.m_priority == IN_EXCLUSIVE )
				return;
		}
	}
}

void input_state::RegisterMouseHandler(fnMouseHandler pHandler, void* caller, size_t priority)
{
	if ( pHandler )
	{
		mouse_handler kbh;
		kbh.m_pHandler = pHandler;
		kbh.m_priority = priority;
		kbh.m_caller = caller;
		m_mouse_handlers.push_back(kbh);

		std::sort(m_mouse_handlers.begin(), m_mouse_handlers.end(), ComparePriority);
	}
}

void input_state::UnregisterMouseHandler(fnMouseHandler pHandler)
{
	if ( pHandler )
	{
		auto it = std::find_if(m_mouse_handlers.begin(), m_mouse_handlers.end(), [&pHandler](mouse_handler& it) { return (it.m_pHandler == pHandler); } );
		if ( it != m_mouse_handlers.end() )
		{
			m_mouse_handlers.erase(it);
			std::sort(m_mouse_handlers.begin(), m_mouse_handlers.end(), ComparePriority);
		}
	}
}

/// gamepad handling

void input_state::HandleGamepad()
{
	for ( auto& it : m_gamepad_handlers )
	{
		if ( !it.m_pHandler(m_gamepad, it.m_caller) )
		{
			if ( it.m_priority == IN_EXCLUSIVE )
				return;
		}
	}
}

void input_state::RegisterGamepadHandler(fnGamepadHandler pHandler, void* caller, size_t priority)
{
	if ( pHandler )
	{
		gamepad_handler jsh;
		jsh.m_pHandler	= pHandler;
		jsh.m_priority	= priority;
		jsh.m_caller	= caller;
		m_gamepad_handlers.push_back(jsh);

		std::sort(m_gamepad_handlers.begin(), m_gamepad_handlers.end(), ComparePriority);
	}
}

void input_state::UnregisterGamepadHandler(fnGamepadHandler pHandler)
{
	if ( pHandler )
	{
		auto it = std::find_if(m_gamepad_handlers.begin(), m_gamepad_handlers.end(), [&pHandler](gamepad_handler& it) { return (it.m_pHandler == pHandler); } );
		if ( it != m_gamepad_handlers.end() )
		{
			m_gamepad_handlers.erase(it);
			std::sort(m_gamepad_handlers.begin(), m_gamepad_handlers.end(), ComparePriority);
			return;
		}
	}
}


/// keyboard

keyboard::keyboard()
: m_device(0)
, m_bBuffered(0)
{
	memset(key_state, 0, sizeof(key_state));
}

keyboard::~keyboard()
{
	if ( m_device != nullptr )
	{
		m_device->Unacquire();
		m_device->Release();
		m_device = nullptr;
	}
}

bool keyboard::Initialize(InputObject* di, HWND hwnd, bool buffered)
{
	HRESULT hr = S_OK;

	hr = di->CreateDevice(GUID_SysKeyboard, &m_device, nullptr);	
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		return false;
	}

	DWORD dwCoopFlags = 0;
	dwCoopFlags |= DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY;

	hr = m_device->SetCooperativeLevel(hwnd, dwCoopFlags); 
	if (FAILED(hr))
	{
		return false;
	}

	if (buffered)
	{
		m_bBuffered = buffered;

		DIPROPDWORD dipdw;

		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = BUFFER_SIZE;

		if( FAILED( hr = m_device->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
		{
			return false;
		}
	}

	hr = m_device->Acquire();
	if (FAILED(hr))
	{
		return false;
	}	

	return true;
}

void keyboard::Flush()
{
	memset(key_state, 0, sizeof(key_state));
}

bool keyboard::KeyPressed(wchar_t key)
{
	if ( ( old_key_state[ key ] & 0x80 ) && !( key_state[ key ] & 0x80 ) )
		return true;

	return false;
}

bool keyboard::KeyDown(wchar_t key)
{	
	return (key_state[key] & 0x80) ? true : false;
}

bool keyboard::KeyUp(wchar_t key)
{	
	return (key_state[key] & 0x80) ? false : true;
}

WCHAR keyboard::GetKeyPress()
{
	for ( wchar_t key = 0; key < 256; ++key )
	{
		if ( key_state[key] & 0x80 )
			return key;
	}

	return 0;
}

void keyboard::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);

	if (!m_bBuffered)
	{
		memcpy_s(old_key_state, sizeof(old_key_state), key_state, sizeof(key_state));

		HRESULT hr = m_device->GetDeviceState(sizeof(key_state), key_state);

		if (SUCCEEDED(hr))
			return;

		if (FAILED(hr))
		{
			hr = m_device->Acquire();

			while ( hr == DIERR_INPUTLOST )
				hr = m_device->Acquire();

			return;
		}
	}
	else
	{		
		DWORD num_keys = BUFFER_SIZE;
		HRESULT hr = m_device->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &num_keys, 0);

		if( hr != DI_OK ) 
		{
			hr = m_device->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_device->Acquire();

			return;
		}
	}
}


const wchar_t* keyboard::GetKeyDescription(wchar_t key)
{
	bool bCapsLock		= (key_state[DIK_CAPITAL] & 0x80) ? true : false;
	bool bShiftPressed	= KeyDown(DIK_LSHIFT) || KeyDown(DIK_RSHIFT);



	if ( key == DIK_A )
		return (bShiftPressed && !bCapsLock) ? TEXT("A") : TEXT("a");

	if ( key == DIK_B )
		return (bShiftPressed && !bCapsLock) ? TEXT("B") : TEXT("b");

	if ( key == DIK_C )
		return (bShiftPressed && !bCapsLock) ? TEXT("C") : TEXT("c");

	if ( key == DIK_D )
		return (bShiftPressed && !bCapsLock) ? TEXT("D") : TEXT("d");

	if ( key == DIK_E )
		return (bShiftPressed && !bCapsLock) ? TEXT("E") : TEXT("e");

	if ( key == DIK_F )
		return (bShiftPressed && !bCapsLock) ? TEXT("F") : TEXT("f");

	if ( key == DIK_G )
		return (bShiftPressed && !bCapsLock) ? TEXT("G") : TEXT("g");

	if ( key == DIK_H )
		return (bShiftPressed && !bCapsLock) ? TEXT("H") : TEXT("h");

	if ( key == DIK_I )
		return (bShiftPressed && !bCapsLock) ? TEXT("I") : TEXT("i");

	if ( key == DIK_J )
		return (bShiftPressed && !bCapsLock) ? TEXT("J") : TEXT("j");

	if ( key == DIK_K )
		return (bShiftPressed && !bCapsLock) ? TEXT("K") : TEXT("k");

	if ( key == DIK_L )
		return (bShiftPressed && !bCapsLock) ? TEXT("L") : TEXT("l");

	if ( key == DIK_M )
		return (bShiftPressed && !bCapsLock) ? TEXT("M") : TEXT("m");

	if ( key == DIK_N )
		return (bShiftPressed && !bCapsLock) ? TEXT("N") : TEXT("n");

	if ( key == DIK_O )
		return (bShiftPressed && !bCapsLock) ? TEXT("O") : TEXT("o");

	if ( key == DIK_P )
		return (bShiftPressed && !bCapsLock) ? TEXT("P") : TEXT("p");

	if ( key == DIK_Q )
		return (bShiftPressed && !bCapsLock) ? TEXT("Q") : TEXT("q");

	if ( key == DIK_R )
		return (bShiftPressed && !bCapsLock) ? TEXT("R") : TEXT("r");

	if ( key == DIK_S )
		return (bShiftPressed && !bCapsLock) ? TEXT("S") : TEXT("s");

	if ( key == DIK_T )
		return (bShiftPressed && !bCapsLock) ? TEXT("T") : TEXT("t");

	if ( key == DIK_U )
		return (bShiftPressed && !bCapsLock) ? TEXT("U") : TEXT("u");

	if ( key == DIK_V )
		return (bShiftPressed && !bCapsLock) ? TEXT("V") : TEXT("v");

	if ( key == DIK_W )
		return (bShiftPressed && !bCapsLock) ? TEXT("W") : TEXT("w");

	if ( key == DIK_X )
		return (bShiftPressed && !bCapsLock) ? TEXT("X") : TEXT("x");

	if ( key == DIK_Y )
		return (bShiftPressed && !bCapsLock) ? TEXT("Y") : TEXT("y");

	if ( key == DIK_Z )
		return (bShiftPressed && !bCapsLock) ? TEXT("Z") : TEXT("z");

	// Digits
	if ( key == DIK_0 )
		return (bShiftPressed && !bCapsLock) ? TEXT(")") : TEXT("0");

	if ( key == DIK_1 )
		return (bShiftPressed && !bCapsLock) ? TEXT("!") : TEXT("1");

	if ( key == DIK_2 )
		return (bShiftPressed && !bCapsLock) ? TEXT("@") : TEXT("2");

	if ( key == DIK_3 )
		return (bShiftPressed && !bCapsLock) ? TEXT("#") : TEXT("3");

	if ( key == DIK_4 )
		return (bShiftPressed && !bCapsLock) ? TEXT("$") : TEXT("4");

	if ( key == DIK_5 )
		return (bShiftPressed && !bCapsLock) ? TEXT("%") : TEXT("5");

	if ( key == DIK_6 )
		return (bShiftPressed && !bCapsLock) ? TEXT("^") : TEXT("6");

	if ( key == DIK_7 )
		return (bShiftPressed && !bCapsLock) ? TEXT("&") : TEXT("7");

	if ( key == DIK_8 )
		return (bShiftPressed && !bCapsLock) ? TEXT("*") : TEXT("8");

	if ( key == DIK_9 )
		return (bShiftPressed && !bCapsLock) ? TEXT("(") : TEXT("9");

	// Symbols
	if ( key == DIK_LBRACKET )
		return (bShiftPressed && !bCapsLock) ? TEXT("{") : TEXT("[");

	if ( key == DIK_RBRACKET )
		return (bShiftPressed && !bCapsLock) ? TEXT("}") : TEXT("]");

	if ( key == DIK_SEMICOLON )
		return (bShiftPressed && !bCapsLock) ? TEXT(":") : TEXT(");");

	if ( key == DIK_PERIOD )
		return (bShiftPressed && !bCapsLock) ? TEXT(">") : TEXT(".");

	if ( key == DIK_COMMA )
		return (bShiftPressed && !bCapsLock) ? TEXT("<") : TEXT(",");

	if ( key == DIK_APOSTROPHE )
		return (bShiftPressed && !bCapsLock) ? TEXT("\"") : TEXT("\'");

	if ( key == DIK_BACKSLASH )
		return (bShiftPressed && !bCapsLock) ? TEXT("|") : TEXT("\\");

	if ( key == DIK_SLASH )
		return (bShiftPressed && !bCapsLock) ? TEXT("?") : TEXT("/");

	if ( key == DIK_GRAVE )
		return (bShiftPressed && !bCapsLock) ? TEXT("~") : TEXT("`");

	if ( key == DIK_MINUS )
		return (bShiftPressed && !bCapsLock) ? TEXT("_") : TEXT("-");

	if ( key == DIK_EQUALS )
		return (bShiftPressed && !bCapsLock) ? TEXT("+") : TEXT("=");

	if ( key == DIK_ADD )
		return TEXT("+");

	if ( key == DIK_SUBTRACT )
		return TEXT("-");

	if ( key == DIK_MULTIPLY )
		return TEXT("*");

	if ( key == DIK_DIVIDE )
		return TEXT("/");

	if ( key == DIK_NUMPAD0 )
		return TEXT("0");

	if ( key == DIK_NUMPAD1 )
		return TEXT("1");

	if ( key == DIK_NUMPAD2 )
		return TEXT("2");

	if ( key == DIK_NUMPAD3 )
		return TEXT("3");

	if ( key == DIK_NUMPAD4 )
		return TEXT("4");

	if ( key == DIK_NUMPAD5 )
		return TEXT("5");

	if ( key == DIK_NUMPAD6 )
		return TEXT("6");

	if ( key == DIK_NUMPAD7 )
		return TEXT("7");

	if ( key == DIK_NUMPAD8 )
		return TEXT("8");

	if ( key == DIK_NUMPAD9 )
		return TEXT("9");

	if ( key == DIK_DECIMAL )
		return TEXT(".");

	if ( key == DIK_SPACE )
		return TEXT(" ");

	return nullptr;
}

/// mouse

mouse::mouse()
: m_device(0)
, m_bBuffered(false)
{
	memset(&mouse_state, 0, sizeof(mouse_state));
	memset(&absolute_mouse_state, 0, sizeof(absolute_mouse_state));	
}

mouse::~mouse()
{
	if ( m_device )
	{
		m_device->Unacquire();
		m_device->Release();
		m_device = nullptr;
	}
}

bool mouse::Initialize(InputObject* di, HWND hwnd, bool buffered)
{
	HRESULT hr = S_OK;

	hr = di->CreateDevice(GUID_SysMouse, &m_device, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND); 
	//hr = m_device->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND); 
	if (FAILED(hr))
	{
		return false;
	}

	m_hwnd = hwnd;

	if (buffered)
	{
		m_bBuffered = true;

		mouse_event = CreateEvent(NULL, false, false, NULL);
		if (mouse_event == NULL)
		{
			return false;
		}

		hr = m_device->SetEventNotification(mouse_event);
		if (FAILED(hr))
		{
			return false;
		}

		DIPROPDWORD dipdw;
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = BUFFER_SIZE;

		hr = m_device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		if (FAILED(hr))
		{
			return false;
		}
	}

	m_device->Acquire();

	return true;
}

bool mouse::ButtonPressed(eButton button) const
{
	if (button >= MAX_MOUSE_BUTTONS)
		return false;

	return ( (mouse_state.rgbButtons[button] & 0x80) ? true : false );
}

void mouse::Update(float deltaTime)
{	
	if (!m_bBuffered)
	{
		m_device->Acquire();

		ZeroMemory( &mouse_state, sizeof(mouse_state) );
		HRESULT hr = m_device->GetDeviceState(sizeof(DIMOUSESTATE2), &mouse_state);

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = now - m_lastUpdate;

		if (SUCCEEDED(hr))
		{
			float kDoubleClickInterval = 1.f / 1000.f;
			if ( !m_wasPressed[Left] && ButtonPressed(Left) )
			{
				m_wasPressed[Left] = true;
				m_wasReleased[Left] = false;
				m_wasDoubleClicked[Left] = false;
			}
			else
			if ( m_wasPressed[Left] && !ButtonPressed(Left) )
			{
				m_wasPressed[Left] = false;
				m_wasReleased[Left] = true;
				m_lastReleaseTime[Left] = deltaTime;
			}	
			else
			if ( m_wasReleased[Left] && elapsed.count() > kDoubleClickInterval )
			{
				m_wasReleased[Left] = false;
				 m_lastReleaseTime[Left] = 0.f;
			}

			if ( m_wasReleased[Left] && ButtonPressed(Left) && elapsed.count() > kDoubleClickInterval )
			{
				m_wasReleased[Left] = false;
				m_wasDoubleClicked[Left] = true;
				m_lastReleaseTime[Left] = 0.f;
			}

			
			absolute_mouse_state.lX += mouse_state.lX;
			absolute_mouse_state.lY += mouse_state.lY;
			absolute_mouse_state.lZ += (mouse_state.lZ > 0) ? 1 : (mouse_state.lZ < 0) ? -1 : 0;
		}

		m_lastUpdate = std::chrono::system_clock::now();

		if (FAILED(hr))
		{
			m_device->Unacquire();
		}
	}
	else
	{
		DWORD elements = BUFFER_SIZE;
		DIDEVICEOBJECTDATA didod[ BUFFER_SIZE ];

		HRESULT hr = m_device->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &elements, 0);

		if( hr != DI_OK ) 
		{
			hr = m_device->Acquire();
			while( hr == DIERR_INPUTLOST ) 
				hr = m_device->Acquire();

			return;
		}
	}
}

void mouse::GetRelativePosition(int& x, int& y, int& z)
{
	x = static_cast<int>( mouse_state.lX );
	y = static_cast<int>( mouse_state.lY );
	z = static_cast<int>( mouse_state.lZ );
}

void mouse::GetAbsolutePosition(int& x, int& y, int& z)
{
	x = static_cast<int>( absolute_mouse_state.lX );
	y = static_cast<int>( absolute_mouse_state.lY );
	z = static_cast<int>( absolute_mouse_state.lZ );
}

math::vector2 mouse::GetPosition() const
{
	return math::vector2(absolute_mouse_state.lX, absolute_mouse_state.lY);
}

math::vector2 mouse::GetRelativePosition() const
{
	return math::vector2(mouse_state.lX, mouse_state.lY);
}

/// gamepad

gamepad::gamepad()
: m_inputObject(0)
, m_device(0)
, m_ID(0)
{
	memset(&m_gamepadState, 0, sizeof(DIJOYSTATE2));
}

gamepad::~gamepad()
{
	if  (m_device != nullptr)
	{
		m_device->Unacquire();
		m_device->Release();
		m_device = nullptr;		
	}
}

bool gamepad::Initialize(InputObject* di, HWND hwnd, bool buffered)
{
	UNREFERENCED_PARAMETER(buffered);

	HRESULT hr = S_OK;

	m_inputObject = di;

	hr = m_inputObject->EnumDevices(DI8DEVCLASS_GAMECTRL, ::EnumDeviceCallback, (LPVOID)this, DIEDFL_ATTACHEDONLY);
	if ( FAILED (hr) )
	{
		return false;
	}

	hr = m_inputObject->CreateDevice(GUID_Joystick, &m_device, NULL);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_device->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND); 
	if (FAILED(hr))
	{
		return false;
	}


	return true;
}

void gamepad::Update(float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);

	if ( !m_device )
		return;

	HRESULT hr = S_OK;
	hr = m_device->Poll();
	if ( FAILED(hr) )
	{
		hr = m_device->Acquire();
		while(hr == DIERR_INPUTLOST) 
			hr = m_device->Acquire();
	}

	hr = m_device->GetDeviceState(sizeof(DIJOYSTATE2), &m_gamepadState);
	if (FAILED(hr))
		return; 
}

int gamepad::GetAxis(eGamepadAxes axis)
{
	switch(axis) 
	{
	case JOY_AXIS_X:
		return m_gamepadState.lX;
	case JOY_AXIS_Y:
		return m_gamepadState.lY;
	case JOY_AXIS_X1:
		return m_gamepadState.lRx;
	case JOY_AXIS_Y1:
		return m_gamepadState.lRy;
	case JOY_AXIS_Z1:
		return m_gamepadState.lRz;
	default:
		return 0;
	}
}

wchar_t gamepad::GetButton(eGamepadButtons button)
{
	return ( m_gamepadState.rgbButtons[button] & 0x80 );
}

bool gamepad::IsReady() const
{
	return (m_device != nullptr);
}

size_t gamepad::CountDevices()
{
	HRESULT hr = S_OK;

	unsigned int counter = 0;

	InputObject* pDI = 0;
	if (SUCCEEDED(hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (VOID**)&pDI, NULL))) 
	{
		pDI->EnumDevices(DI8DEVCLASS_GAMECTRL, ::CountDevicesCallback, &counter, DIEDFL_ATTACHEDONLY);	
	}

	pDI->Release();

	return counter;
}

BOOL CALLBACK gamepad::EnumDeviceCallback(LPCDIDEVICEINSTANCEW instance, LPVOID context)
{
	UNREFERENCED_PARAMETER(context);

	if (m_NumDevices == m_ID) 
	{
		if (SUCCEEDED(m_inputObject->CreateDevice(instance->guidInstance, &m_device, NULL)))
			return DIENUM_STOP;		
	}

	++m_NumDevices;

	return DIENUM_CONTINUE;
}

} // input

BOOL CALLBACK EnumDeviceCallback(LPCDIDEVICEINSTANCEW instance, LPVOID context)
{
	if ( context ) 
	{
		return ((input::gamepad*)context)->EnumDeviceCallback(instance, context);
	}

	return DIENUM_STOP;
}


BOOL CALLBACK CountDevicesCallback(LPCDIDEVICEINSTANCEW instance, LPVOID counter)
{
	UNREFERENCED_PARAMETER(instance);

	if (counter != NULL) 
	{
		unsigned int *tmpCounter = (unsigned int*)counter;
		++(*tmpCounter);
		counter = tmpCounter;
	}

	return DIENUM_CONTINUE;
}