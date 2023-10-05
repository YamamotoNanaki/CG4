#include "StringUtil.h"
#include "Debug.h"
#include "Warning.h"
WarningDisabled
#include <windows.h>
#include <vector>
WarningDisabledPop


std::wstring StringToWString(const std::string& oString)
{
	// SJIS → wstring
	int32_t iBufferSize = MultiByteToWideChar(CP_ACP, 0, oString.c_str()
		, -1, (wchar_t*)NULL, 0);

	// バッファの取得
	std::vector<wchar_t>buff;
	buff.resize(iBufferSize);

	wchar_t* buffPtr = buff.data();

	// SJIS → wstring
	MultiByteToWideChar(CP_ACP, 0, oString.c_str(), -1, buffPtr
		, iBufferSize);

	// stringの生成
	std::wstring oRet(buffPtr, buffPtr + iBufferSize - 1);

	// 変換結果を返す
	return(oRet);
}
