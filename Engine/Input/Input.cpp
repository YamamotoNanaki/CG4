#include "Input.h"
#include "WindowsAPI.h"
#include "Debug.h"
#include <cassert>

using namespace IFE;

Input* Input::inputInstance = nullptr;

Input* IFE::Input::Instance()
{
	static Input inst;
	return &inst;
}

void IFE::Input::Initalize()
{
	inputInstance = Instance();
	HWND hwnd = *WindowsAPI::Instance()->GetHWnd();

	HRESULT result = DirectInput8Create(
		*WindowsAPI::Instance()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&inputInstance->directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	result = inputInstance->directInput->CreateDevice(GUID_SysKeyboard, &inputInstance->keyboard, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = inputInstance->keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = inputInstance->keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	//マウスデバイスの生成
	result = inputInstance->directInput->CreateDevice(GUID_SysMouse, &inputInstance->devMouse, NULL);
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = inputInstance->devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
	assert(SUCCEEDED(result));
	// 排他制御レベルのセット
	result = inputInstance->devMouse->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	//コントローラデバイスの生成
	XInputGetState(
		0,       // DWORD         dwUserIndex
		&inputInstance->pad); // XINPUT_STATE* pState
}

void IFE::Input::Update()
{
	HRESULT result = S_OK;
	for (int32_t i = 0; i < 256; i++)
	{
		inputInstance->oldkey[i] = inputInstance->key[i];
	}
	inputInstance->keyboard->Acquire();
	result = inputInstance->keyboard->GetDeviceState(sizeof(inputInstance->key), inputInstance->key);

	// 前回の入力を保存
	inputInstance->oldmouse = inputInstance->mouse;
	// マウス
	result = inputInstance->devMouse->Acquire();	// マウス動作開始
	// マウスの入力
	result = inputInstance->devMouse->GetDeviceState(sizeof(inputInstance->mouse), &inputInstance->mouse);

	inputInstance->oldpad = inputInstance->pad;
	XInputGetState(
		0,       // DWORD         dwUserIndex
		&inputInstance->pad); // XINPUT_STATE* pState
}

bool IFE::Input::KeyTriggere(KeyCode keyCode)
{
	return inputInstance->key[keyCode] && !inputInstance->oldkey[keyCode];
}

bool IFE::Input::KeyDown(KeyCode keyCode)
{
	return inputInstance->key[keyCode];
}

bool IFE::Input::KeyRelease(KeyCode keyCode)
{
	return !inputInstance->key[keyCode] && inputInstance->oldkey[keyCode];
}

bool Input::MousePush(MouseCode m)
{
	if (inputInstance->mouse.rgbButtons[(BYTE)m]) { return true; }
	return false;
}

bool Input::MouseTriggere(MouseCode m)
{
	if (inputInstance->mouse.rgbButtons[(BYTE)m] && !inputInstance->oldmouse.rgbButtons[(BYTE)m]) { return true; }
	return false;
}

bool Input::MouseRelease(MouseCode m)
{
	if (!inputInstance->mouse.rgbButtons[(BYTE)m] && inputInstance->oldmouse.rgbButtons[(BYTE)m]) { return true; }
	return false;
}

Mouse Input::GetMouse3D()
{
	Mouse m{};
	m.x = inputInstance->mouse.lX;
	m.y = inputInstance->mouse.lY;
	m.z = inputInstance->mouse.lZ;
	return m;
}

Float2 Input::GetLAnalog(int32_t unresponsive_range)
{
	return Float2(GetLXAnalog(unresponsive_range), GetLYAnalog(unresponsive_range));
}
Float2 Input::GetRAnalog(int32_t unresponsive_range)
{
	return Float2(GetRXAnalog(unresponsive_range), GetRYAnalog(unresponsive_range));
}
float Input::GetLXAnalog(int32_t unresponsive_range)
{
	float x = inputInstance->pad.Gamepad.sThumbLX;
	if (x < unresponsive_range && x > -unresponsive_range)
	{
		x = 0;
	}
	if (x == -32768)x = -32767;
	return x / 32767.0f;
}
float Input::GetRXAnalog(int32_t unresponsive_range)
{
	float x = inputInstance->pad.Gamepad.sThumbRX;
	if (x < unresponsive_range && x > -unresponsive_range)
	{
		x = 0;
	}
	if (x == -32768)x = -32767;
	return x / 32767.0f;
}
float Input::GetLYAnalog(int32_t unresponsive_range)
{
	float y = inputInstance->pad.Gamepad.sThumbLY;
	if (y < unresponsive_range && y > -unresponsive_range)
	{
		y = 0;
	}
	if (y == -32768)y = -32767;
	return y / 32767.0f;
}
float Input::GetRYAnalog(int32_t unresponsive_range)
{
	float y = inputInstance->pad.Gamepad.sThumbRY;
	if (y < unresponsive_range && y > -unresponsive_range)
	{
		y = 0;
	}
	if (y == -32768)y = -32767;
	return y / 32767.0f;
}
bool Input::PadPush(PADCODE p)
{
	if (inputInstance->pad.Gamepad.wButtons & (UINT)p)return true;
	return false;
}
bool Input::PadTriggere(PADCODE p)
{
	if ((inputInstance->pad.Gamepad.wButtons & (UINT)p) && !(inputInstance->oldpad.Gamepad.wButtons & (UINT)p))return true;
	return false;
}
bool Input::PadRelease(PADCODE p)
{
	if (!(inputInstance->pad.Gamepad.wButtons & (UINT)p) && (inputInstance->oldpad.Gamepad.wButtons & (UINT)p))return true;
	return false;
}
void Input::PadVibrationStart(WORD L, WORD R)
{
	inputInstance->vibration.wLeftMotorSpeed = L;
	inputInstance->vibration.wRightMotorSpeed = R;
	XInputSetState(0, &inputInstance->vibration);
}
void Input::PadVibrationStop()
{
	inputInstance->vibration.wLeftMotorSpeed = 0;
	inputInstance->vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &inputInstance->vibration);
}
