
// 以下の ifdef ブロックは DLL から簡単にエクスポートさせるマクロを作成する標準的な方法です。 
// この DLL 内のすべてのファイルはコマンドラインで定義された BULLETML_EXPORTS シンボル
// でコンパイルされます。このシンボルはこの DLL が使用するどのプロジェクト上でも未定義でなけ
// ればなりません。この方法ではソースファイルにこのファイルを含むすべてのプロジェクトが DLL 
// からインポートされたものとして BULLETML_API 関数を参照し、そのためこの DLL はこのマク 
// ロで定義されたシンボルをエクスポートされたものとして参照します。
#ifdef BULLETML_EXPORTS
#define BULLETML_API __declspec(dllexport)
#else
#define BULLETML_API __declspec(dllimport)
#endif

// このクラスは bulletml.dll からエクスポートされます
class BULLETML_API CBulletml {
public:
	CBulletml(void);
	// TODO: この位置にメソッドを追加してください。
};

extern BULLETML_API int nBulletml;

BULLETML_API int fnBulletml(void);

