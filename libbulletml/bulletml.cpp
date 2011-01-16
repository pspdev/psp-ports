// bulletml.cpp : DLL アプリケーション用のエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "bulletml.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// これはエクスポートされた変数の例です。
BULLETML_API int nBulletml=0;

// これはエクスポートされた関数の例です。
BULLETML_API int fnBulletml(void)
{
	return 42;
}

// これはエクスポートされたクラスのコンストラクタです。
// クラスの定義については bulletml.h を参照してください。
CBulletml::CBulletml()
{ 
	return; 
}

