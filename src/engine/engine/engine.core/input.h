
#pragma once

#define DIRECTINPUT_VERSION 0x800

#include <dinput.h>
#include <vector>
#include <memory>
#include <chrono>

#include "engine.math/vector.h"

#define BUFFER_SIZE (256)

typedef IDirectInput8 InputObject;
typedef IDirectInputDevice8 InputDevice;

namespace input
{

class keyboard;
class mouse;
class gamepad;

enum {
	IN_DEFAULT,
	IN_EXCLUSIVE,
};

typedef bool (*fnKeyboardHandler)(std::shared_ptr<keyboard> pKeyboard, void* userData);
typedef bool (*fnMouseHandler)(std::shared_ptr<mouse> pMouse, void* userData);
typedef bool (*fnGamepadHandler)(std::shared_ptr<gamepad> pgamepad, void* userData);

class input_handler
{
public:

	size_t	m_priority;
	void*	m_caller;

	input_handler()
		: m_priority(IN_DEFAULT)
		, m_caller(0)
	{}
};

class keyboard_handler : public input_handler
{
	bool m_bEnabled;
	
public:
	fnKeyboardHandler m_pHandler;
	keyboard_handler() 
		: input_handler()
		, m_pHandler(0)
		, m_bEnabled(true)
	{}

	void Enable(bool bEnabled) { m_bEnabled = bEnabled; }
	bool IsEnabled() const { return m_bEnabled; }
};

class mouse_handler : public input_handler
{
public:
	fnMouseHandler m_pHandler;
	mouse_handler() 
		: input_handler()
		, m_pHandler(0)
	{}
};

class gamepad_handler : public input_handler
{
public:
	fnGamepadHandler m_pHandler;
	gamepad_handler() 
		: input_handler()
		, m_pHandler(0)
	{}
};

class input_state
{

public:

	input_state(HWND hwnd);
	~input_state();
	
	HRESULT		Initialize(HWND hwnd);
	
	std::shared_ptr<keyboard>	CreateKeyboard();
	std::shared_ptr<mouse>		CreateMouse();	
	std::shared_ptr<gamepad>	CreateGamepad();

	void Update(float deltaTime);

	std::shared_ptr<keyboard>	GetKeyboard() const { return m_keyboard; }
	std::shared_ptr<mouse>		GetMouse() const { return m_mouse; }
	std::shared_ptr<gamepad>	GetGamepad() const { return m_gamepad; }
	
	// keyboard handling
	void HandleKeyboard();
	void RegisterKeyboardHandler(fnKeyboardHandler pHandler, void* caller = 0, size_t priority=IN_DEFAULT);
	void UnregisterKeyboardHandler(fnKeyboardHandler pHandler);

	// mouse handling
	void HandleMouse();
	void RegisterMouseHandler(fnMouseHandler pHandler, void* caller = 0, size_t priority=IN_DEFAULT);
	void UnregisterMouseHandler(fnMouseHandler pHandler);

	// gamepad handling
	void HandleGamepad();
	void RegisterGamepadHandler(fnGamepadHandler pHandler, void* caller = 0, size_t priority=IN_DEFAULT);
	void UnregisterGamepadHandler(fnGamepadHandler pHandler);

private:

	InputObject* m_inputObject;
	
	std::shared_ptr<keyboard>	m_keyboard;
	std::shared_ptr<mouse>		m_mouse;
	std::shared_ptr<gamepad>	m_gamepad;

	HWND m_hwnd;

	std::vector< keyboard_handler >	m_keyboard_handlers;
	std::vector< mouse_handler >	m_mouse_handlers;
	std::vector< gamepad_handler >	m_gamepad_handlers;

	enum InputDeviceType { 
		KEYBOARD,
		MOUSE
	};
	
	float m_lastUpdate;
};

//////////////////////////////////////////////////////////////////////////
// keyboard

class keyboard
{
	InputDevice*	m_device;
	DIDEVICEOBJECTDATA didod[ BUFFER_SIZE ];
	
	char			key_state[256];
	char			old_key_state[256];

	bool			m_bBuffered;

	bool Initialize(InputObject* di, HWND hwnd, bool buffered=false);
	void ClearDevice();
	InputDevice* GetDevice() const { return m_device; }

	friend class input_state;

public:

	keyboard();
	~keyboard();

	void Update(float deltaTime);
	bool KeyDown(wchar_t key);		
	bool KeyPressed(wchar_t key);
	bool KeyUp(wchar_t key);

	WCHAR GetKeyPress();
	const wchar_t* GetKeyDescription(wchar_t key);

	void Flush();

};

//////////////////////////////////////////////////////////////////////////
// mouse

enum {
	MOUSE_BUTTON_0, 
	MOUSE_BUTTON_1, 
	MOUSE_BUTTON_2, 
	MOUSE_BUTTON_3, 
	MOUSE_BUTTON_4, 
	MOUSE_BUTTON_5, 
	MOUSE_BUTTON_6, 
	MOUSE_BUTTON_7,
	MAX_MOUSE_BUTTONS
};

class mouse
{

public:

	mouse();
	~mouse();

	enum eButton {
		Left,
		Right,
		Middle
	};
	void Update(float deltaTime);

	bool ButtonPressed(eButton button) const;
	bool ButtonReleased(eButton button) { return m_wasReleased[button]; }
	bool ButtonDoubleClicked(eButton button) { return m_wasDoubleClicked[button]; }

	int GetRelativeX() const { return mouse_state.lX; }
	int GetRelativeY() const { return mouse_state.lY; }
	int GetRelativeZ() const { return mouse_state.lZ; }

	int GetAbsoluteX() const { return absolute_mouse_state.lX; }
	int GetAbsoluteY() const { return absolute_mouse_state.lY; }
	int GetAbsoluteZ() const { return absolute_mouse_state.lZ; }

	void GetAbsolutePosition(int& x, int& y, int& z);
	void GetRelativePosition(int& x, int& y, int& z);

	math::vector2 GetPosition() const;
	math::vector2 GetRelativePosition() const;

	int GetMouseWheelValue() const { return absolute_mouse_state.lZ; }

private:

	InputDevice*	m_device;
	HANDLE			mouse_event;
	DIMOUSESTATE2	mouse_state;
	DIMOUSESTATE2	absolute_mouse_state;
	HWND m_hwnd;
	std::chrono::system_clock::time_point m_lastUpdate;

	bool m_wasPressed[8];
	bool m_wasReleased[8];
	bool m_wasDoubleClicked[8];
	float m_lastReleaseTime[8];	
	
	bool m_bBuffered;

	bool Initialize(InputObject* di, HWND hwnd, bool buffered=FALSE);
	void ClearDevice();
	

	InputDevice* GetDevice() const { return m_device; }

	friend class input_state;
};

//////////////////////////////////////////////////////////////////////////
// gamepad

enum eGamepadAxes {
	JOY_AXIS_X,
	JOY_AXIS_Y,
	JOY_AXIS_Z,
	JOY_AXIS_X1,
	JOY_AXIS_Y1,
	JOY_AXIS_Z1,
	JOY_MAX_AXES,
};

enum eGamepadButtons {
	JOY_BUTTON_00,
	JOY_BUTTON_01,
	JOY_BUTTON_02,
	JOY_BUTTON_03,
	JOY_BUTTON_04,
	JOY_BUTTON_05,
	JOY_BUTTON_06,
	JOY_BUTTON_07,
	JOY_BUTTON_08,
	JOY_BUTTON_09,
	JOY_BUTTON_10,
	JOY_BUTTON_11,
	JOY_BUTTON_12,
	JOY_BUTTON_13,
	JOY_BUTTON_14,
	JOY_BUTTON_15,
	JOY_BUTTON_16,
	JOY_BUTTON_17,
	JOY_BUTTON_18,
	JOY_BUTTON_19,
	JOY_BUTTON_20,
	JOY_BUTTON_21,
	JOY_BUTTON_22,
	JOY_BUTTON_23,
	JOY_MAX_BUTTONS
};

class gamepad
{
	InputObject*	m_inputObject;
	InputDevice*	m_device;
	DIJOYSTATE2		m_gamepadState;

	size_t			m_ID;
	size_t			m_NumDevices;

	bool Initialize(InputObject* di, HWND hwnd, bool buffered=FALSE);
	void ClearDevice();

	InputDevice* GetDevice() const { return m_device; }

	friend class input_state;

public:

	gamepad();
	~gamepad();

	void Update(float deltaTime);

	int GetAxis(eGamepadAxes axis);
	wchar_t GetButton(eGamepadButtons button);

	bool IsReady() const;
	
	static size_t CountDevices();
	BOOL CALLBACK EnumDeviceCallback(LPCDIDEVICEINSTANCEW instance, LPVOID context);

};

inline bool ComparePriority(input_handler& a, input_handler& b)
{
	if ( a.m_priority < b.m_priority )
		return false;
	if ( a.m_priority > b.m_priority )
		return true;

	return false;
}


} // input