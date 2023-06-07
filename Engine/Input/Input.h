#pragma once
#include <dinput.h>
#include <wrl.h>
#include <Xinput.h>
#include <cstdint>
#include "Float2.h"

#pragma comment (lib, "xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace IFE
{
	typedef BYTE KeyCode;

	struct Mouse
	{
		LONG x;
		LONG y;
		LONG z;
	};

	enum class PADCODE
	{
		UP = XINPUT_GAMEPAD_DPAD_UP,
		DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
		LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
		RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
		START = XINPUT_GAMEPAD_START,
		BACK = XINPUT_GAMEPAD_BACK,
		LTHUMB = XINPUT_GAMEPAD_LEFT_THUMB,
		RTHUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
		LSHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER,
		RSHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		A = XINPUT_GAMEPAD_A,
		B = XINPUT_GAMEPAD_B,
		X = XINPUT_GAMEPAD_X,
		Y = XINPUT_GAMEPAD_Y,
	};

	enum class MouseCode
	{
		Left = 0b00, Right = 0b01, Middle = 0b10
	};

	class Input final
	{
	private:
		char key[256]{}, oldkey[256]{};
		Microsoft::WRL::ComPtr<IDirectInput8> directInput = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> keyboard = nullptr;
		Microsoft::WRL::ComPtr<IDirectInputDevice8> devMouse = nullptr;
		DIMOUSESTATE2 mouse = {};
		DIMOUSESTATE2 oldmouse = {};
		XINPUT_STATE pad{};
		XINPUT_STATE oldpad{};
		XINPUT_VIBRATION vibration{};
		static Input* inputInstance;

	private:
		Input() {}
		Input(const Input&) {}
		Input& operator=(const Input&) {}
		~Input() {}

	public:
		static Input* Instance();
		static void Initalize();

		/// <summary>
		/// キーボード情報のアップデート
		/// </summary>
		/// <param name="result"></param>
		static void Update();

		//キーボードの判定
		/// <summary>
		/// トリガー判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		static bool KeyTriggere(KeyCode keyCode);
		/// <summary>
		/// 入力判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		static bool KeyDown(KeyCode keyCode);
		/// <summary>
		/// リリース判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		static bool KeyRelease(KeyCode keyCode);
		//マウスの判定
		static bool MousePush(MouseCode mouse);
		static bool MouseTriggere(MouseCode mouse);
		static bool MouseRelease(MouseCode mouse);
		static Mouse GetMouse3D();
		//コントローラーの判定
		static Float2 GetLAnalog(std::int32_t unresponsive_range = 200);
		static Float2 GetRAnalog(std::int32_t unresponsive_range = 200);
		static float GetLXAnalog(std::int32_t unresponsive_range = 200);
		static float GetRXAnalog(std::int32_t unresponsive_range = 200);
		static float GetLYAnalog(std::int32_t unresponsive_range = 200);
		static float GetRYAnalog(std::int32_t unresponsive_range = 200);
		static bool PadPush(PADCODE pad);
		static bool PadTriggere(PADCODE pad);
		static bool PadRelease(PADCODE pad);
		static void PadVibrationStart(WORD L, WORD R);
		static void PadVibrationStop();

	};
}

#define Key_Esc             DIK_ESCAPE          
#define Key_1               DIK_1               
#define Key_2               DIK_2               
#define Key_3               DIK_3               
#define Key_4               DIK_4               
#define Key_5               DIK_5               
#define Key_6               DIK_6               
#define Key_7               DIK_7               
#define Key_8               DIK_8               
#define Key_9               DIK_9               
#define Key_0               DIK_0               
#define Key_Minus           DIK_MINUS           
#define Key_Equals          DIK_EQUALS          
#define Key_Back            DIK_BACK            
#define Key_Tab             DIK_TAB             
#define Key_Q               DIK_Q               
#define Key_W               DIK_W               
#define Key_E               DIK_E               
#define Key_R               DIK_R               
#define Key_T               DIK_T               
#define Key_Y               DIK_Y               
#define Key_U               DIK_U               
#define Key_I               DIK_I               
#define Key_O               DIK_O               
#define Key_P               DIK_P               
#define Key_LBRACKET        DIK_LBRACKET        
#define Key_RBRACKET        DIK_RBRACKET        
#define Key_Enter           DIK_RETURN          
#define Key_LCtrl           DIK_LCONTROL        
#define Key_A               DIK_A               
#define Key_S               DIK_S               
#define Key_D               DIK_D               
#define Key_F               DIK_F               
#define Key_G               DIK_G               
#define Key_H               DIK_H               
#define Key_J               DIK_J               
#define Key_K               DIK_K               
#define Key_L               DIK_L               
#define Key_SEMICOLON       DIK_SEMICOLON       
#define Key_APOSTROPHE      DIK_APOSTROPHE      
#define Key_GRAVE           DIK_GRAVE           
#define Key_LShift          DIK_LSHIFT          
#define Key_Backslash       DIK_BACKSLASH       
#define Key_Z               DIK_Z               
#define Key_X               DIK_X               
#define Key_C               DIK_C               
#define Key_V               DIK_V               
#define Key_B               DIK_B               
#define Key_N               DIK_N               
#define Key_M               DIK_M               
#define Key_COMMA           DIK_COMMA           
#define Key_PERIOD          DIK_PERIOD          
#define Key_Slash           DIK_SLASH           
#define Key_Rshift          DIK_RSHIFT          
#define Key_MULTIPLY        DIK_MULTIPLY        
#define Key_LMENU           DIK_LMENU           
#define Key_Space           DIK_SPACE           
#define Key_CAPITAL         DIK_CAPITAL         
#define Key_F1              DIK_F1              
#define Key_F2              DIK_F2              
#define Key_F3              DIK_F3              
#define Key_F4              DIK_F4              
#define Key_F5              DIK_F5              
#define Key_F6              DIK_F6              
#define Key_F7              DIK_F7              
#define Key_F8              DIK_F8              
#define Key_F9              DIK_F9              
#define Key_F10             DIK_F10             
#define Key_NumLock         DIK_NUMLOCK         
#define Key_SCROLL          DIK_SCROLL          
#define Key_num7            DIK_NUMPAD7         
#define Key_num8            DIK_NUMPAD8         
#define Key_num9            DIK_NUMPAD9         
#define Key_SUBTRACT        DIK_SUBTRACT        
#define Key_num4            DIK_NUMPAD4         
#define Key_num5            DIK_NUMPAD5         
#define Key_num6            DIK_NUMPAD6         
#define Key_ADD             DIK_ADD             
#define Key_num1            DIK_NUMPAD1         
#define Key_num2            DIK_NUMPAD2         
#define Key_num3            DIK_NUMPAD3         
#define Key_num0            DIK_NUMPAD0         
#define Key_DECIMAL         DIK_DECIMAL         
#define Key_OEM_102         DIK_OEM_102         
#define Key_F11             DIK_F11             
#define Key_F12             DIK_F12             
#define Key_F13             DIK_F13             
#define Key_F14             DIK_F14             
#define Key_F15             DIK_F15             
#define Key_KANA            DIK_KANA            
#define Key_ABNT_C1         DIK_ABNT_C1         
#define Key_CONVERT         DIK_CONVERT         
#define Key_NOCONVERT       DIK_NOCONVERT       
#define Key_YEN             DIK_YEN             
#define Key_ABNT_C2         DIK_ABNT_C2         
#define Key_NUMPADEQUALS    DIK_NUMPADEQUALS    
#define Key_PREVTRACK       DIK_PREVTRACK       
#define Key_AT              DIK_AT              
#define Key_COLON           DIK_COLON           
#define Key_UNDERLINE       DIK_UNDERLINE       
#define Key_KANJI           DIK_KANJI           
#define Key_STOP            DIK_STOP            
#define Key_AX              DIK_AX              
#define Key_UNLABELED       DIK_UNLABELED       
#define Key_NEXTTRACK       DIK_NEXTTRACK       
#define Key_NUMPADENTER     DIK_NUMPADENTER     
#define Key_RCONTROL        DIK_RCONTROL        
#define Key_MUTE            DIK_MUTE            
#define Key_CALCULATOR      DIK_CALCULATOR      
#define Key_PLAYPAUSE       DIK_PLAYPAUSE       
#define Key_MEDIASTOP       DIK_MEDIASTOP       
#define Key_VOLUMEDOWN      DIK_VOLUMEDOWN      
#define Key_VOLUMEUP        DIK_VOLUMEUP        
#define Key_WEBHOME         DIK_WEBHOME         
#define Key_NUMPADCOMMA     DIK_NUMPADCOMMA     
#define Key_DIVIDE          DIK_DIVIDE          
#define Key_SYSRQ           DIK_SYSRQ           
#define Key_RMENU           DIK_RMENU           
#define Key_PAUSE           DIK_PAUSE           
#define Key_HOME            DIK_HOME            
#define Key_Up              DIK_UP              
#define Key_PRIOR           DIK_PRIOR           
#define Key_Left            DIK_LEFT            
#define Key_Rigth           DIK_RIGHT           
#define Key_END             DIK_END             
#define Key_Down            DIK_DOWN            
#define Key_NEXT            DIK_NEXT            
#define Key_INSERT          DIK_INSERT          
#define Key_DELETE          DIK_DELETE          
#define Key_LWIN            DIK_LWIN            
#define Key_RWIN            DIK_RWIN            
#define Key_APPS            DIK_APPS            
#define Key_POWER           DIK_POWER           
#define Key_SLEEP           DIK_SLEEP           
#define Key_WAKE            DIK_WAKE            
#define Key_WEBSEARCH       DIK_WEBSEARCH       
#define Key_WEBFAVORITES    DIK_WEBFAVORITES    
#define Key_WEBREFRESH      DIK_WEBREFRESH      
#define Key_WEBSTOP         DIK_WEBSTOP         
#define Key_WEBFORWARD      DIK_WEBFORWARD      
#define Key_WEBBACK         DIK_WEBBACK         
#define Key_MYCOMPUTER      DIK_MYCOMPUTER      
#define Key_MAIL            DIK_MAIL            
#define Key_MEDIASELECT     DIK_MEDIASELECT     