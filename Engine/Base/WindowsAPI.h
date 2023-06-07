#pragma once
#include <Windows.h>
#include <cstdint>

namespace IFE
{
	class WindowsAPI final
	{
	private:
		WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
		HWND hwnd;
	private:
		MSG msg{}; // ���b�Z�[�W
		WindowsAPI() {}
		WindowsAPI(const WindowsAPI&) {}
		WindowsAPI& operator=(const WindowsAPI&) {}
		~WindowsAPI() {}
	public:
		int32_t winWidth;
		int32_t winHeight;

	public:
		static WindowsAPI* Instance();
		static void Finalize();
		void Initialize(int32_t window_width, int32_t window_height, LPCWSTR windowName = L"DirectXGame");
		void Unregister();
		bool Message();
		HINSTANCE* GetHInstance();
		HWND* GetHWnd();

		//�E�B���h�E�v���V�[�W��
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}