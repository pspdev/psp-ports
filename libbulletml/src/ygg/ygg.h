/******************************************************************************
	世界樹 -yggdrasil-
		世界樹モジュールヘッダファイル
											Coded by Wraith	in July 14, 2002.
******************************************************************************/

//　Tab幅を４文字に設定して表示させてください。

///////////////////////////////////////////////////////////////////////////////
//
//	■ ygg.h
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.h
//
//	□ 関連ファイル
//		本モジュールの本体
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.cpp
//		本モジュールのチュートリアルソースファイル
//		http://tricklib.com/cxx/ex/yggdrasil/ygg_test.cpp
//		全ファイルパック
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.lzh
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.zip
//
//	□ リファレンス・サポートページ
//		http://tricklib.com/cxx/ex/yggdrasil/
//
//	□ ライセンス情報
//		http://tricklib.com/license.htm
//

#ifndef __YGGDRASIL_YGG_H__

#define __YGGDRASIL_YGG_H__

#if !defined(__WITH_YGGDRASIL__)
#	define	__WITH_YGGDRASIL__
#endif

#if !defined(__BABEL_BABEL_H__) && defined(__WITH_BABEL__)
#include "babel.h"
#endif

#if defined(__WITH_BABEL__) && defined(__BBL_USE_SELECTORS__)
#define __YGGDRASIL_WITH_BABEL__
#endif

#include <cassert>
#include <iostream>
#include <fstream>
#include <list>

#ifndef NDEBUG
#ifndef _DEBUG
#define _DEBUG
#endif
#endif

//
//	☆ベースエンコーディング指定マクロ
//

//	コンパイル時に指定するのが面倒な場合は以下のコメント行から適切な指定を
//	(コメントを解除して)有効にしてください。いっさい指定がなされない場合で
//	も自動判別されますが、正確に判断されなっかたり判別の為に(余分)コードが
//	生成されたりする可能性があります。
//
//#define __USING_ANSI__
//#define __USING_SJIS__
//#define __USING_EUC__
//#define __USING_UTF8__

#if !(defined(__USING_ANSI__) || defined(__USING_SJIS__) || defined(__USING_EUC__) || defined(__USING_UTF8__) || defined(__USING_UNKNOWN__))
#define	__USING_UNKNOWN__
#endif



#if defined(__BORLANDC__)
#	pragma warn -8022
#	pragma warn -8026
#	pragma warn -8027
#endif

/******************************************************************************
	□■□■                      TrickPalace                       □■□■
	■□■□              http://www.trickpalace.net/               ■□■□
******************************************************************************/

//
//  ●yggdrasil
//
namespace yggdrasil {

//
//  ▼使用する文字列クラス [ 〆 ]
//
}
#include <string>
namespace yggdrasil {
typedef std::string ygg_string;
//  typedef std::wstring ygg_wstring;

//
//  ●ノード型名 [ 〆 ]
//
namespace ygg_node_type {
	const ygg_string empty = "empty";
	//const ygg_string node = "node";
	const ygg_string text = "text";
	const ygg_string comment = "comment";
	const ygg_string attribute = "attribute";
	const ygg_string list = "list";
	const ygg_string root = "root";
	const ygg_string element = "element";
}

//
//  ●ターム [ 〆 ]
//
namespace ygg_term {
	const ygg_string broken_char = "?";		//	２文字以上でも可、ANSI以外は不可。
	const ygg_string empty = "";
	const ygg_string element_lead = "<";
	const ygg_string element_sign = "/";
	const ygg_string element_trail = ">";
	const ygg_string comment_lead = "<!--";
	const ygg_string comment_trail = "-->";
	const ygg_string cdata_lead = "<![CDATA[";
	const ygg_string cdata_trail = "]]>";
	const ygg_string equal = "=";
	const ygg_string attribute_prefix = "@";
	const ygg_string indent = "\t";
	const ygg_string return_code = "\n";
	const ygg_string white_space = "\n\r\t\v ";
	const ygg_string node = "node()";
	const ygg_string wildcard = "*";
	const ygg_string comment_node = "comment()";
	const ygg_string text_node = "text()";
	const ygg_string attribute_node = "attribute()";
	const ygg_string element_node = "element()";
#ifndef __REJECT_PROCESSING_INSTRUCTION__
	const ygg_string processing_instruction_node = "processing-instruction()";
#endif
//	const ygg_string doctype_node = "doctype()";
//	const ygg_string cdata_section_node = "cdata-section()";
	const ygg_string list_node = "list()";
//	const ygg_string root_node = "root()";
	const ygg_string path_dir = "/";
	const ygg_string path_root = path_dir;
	const ygg_string path_wherever = "//";
	const ygg_string path_this = ".";
	const ygg_string path_this_element = "./";
	const ygg_string path_this__wherever = ".//";
	const ygg_string path_parent = "..";
	const ygg_string path_parenthesis_begin = "[";
	const ygg_string path_parenthesis_end = "]";
	const ygg_string path_last_index = "last()";

	const ygg_string ygg_invalid_name_chars_a	= " !\"#$%&'()*+,-./;<=>?@[\\]^`{|}~";
	const ygg_string ygg_invalid_name_chars_b	= " !\"#$%&'()*+,/;<=>?@[\\]^`{|}~";

	const ygg_string yggdrasil_version = "information>\n"
											"\tmodule>\tid=yggdrasil\n"
											"\t\tname=世界樹  -yggdrasil- \n"
											"\t\turl=http://tricklib.com/cxx/ex/yggdrasil/\n"
											"\t\tversion=ベータIIII-I\n"
											"\tauthor>\tid=wraith\n"
											"\t\tname=道化師＼n"
											"\t\turl=http://www.trickpalace.net/\n"
											"\t\tmail=wraith@trickpalace.net\n";
}
//
//	SOX path term
//
//		term/node		comment		text		attribute	element
//		node()			○			○			○			○
//		comment()		○			×			×			×
//		text()			×			○			×			×
//		attribute()		×			×			○			×
//		@*				×			×			○			×
//		element()		×			×			×			○
//		*				×			×			×			○
//

/******************************************************************************
	□■□■                       cppll ML                         □■□■
	■□■□           http://www.trickpalace.net/cppll/            ■□■□
******************************************************************************/

//
//	▽先行宣言
//
class ygg_node;
class ygg_node_body;
class ygg_ghost;
class ygg_list;
class ygg_root;
class ygg_element;
class ygg_iterator;
class ygg_reverse_iterator;
class ygg_const_iterator;
class ygg_const_reverse_iterator;
class sax_parser;
class sax_handler;
class ygg_error_code;


class ygg_smart_base {
  public:
	typedef ygg_smart_base this_type;
  private:
	volatile int ref_count;
  protected:
	ygg_smart_base() :ref_count(0) {}
	virtual ~ygg_smart_base() { assert(0 == ref_count); }

  public:
	void inc_ref() volatile {
		if (NULL != this) {
			++ref_count;
		}
	}
	void dec_ref() volatile {
		if (NULL != this) {
			if (--ref_count <= 0) {
				assert(0 == ref_count);
				delete this;
			}
		}
	}
};
template<class target_object>
class ygg_smart_shell {
  public:
	typedef ygg_smart_shell<target_object> this_type;
	typedef target_object	target_type;

  private:
	target_type *value;

  public:
	ygg_smart_shell(target_type *X_value = NULL) :value(X_value) {
		value->inc_ref();
	}
	ygg_smart_shell(const this_type &X) :value(X.value) {
		value->inc_ref();
	}
	~ygg_smart_shell() {
		value->dec_ref();
	}

	this_type & operator = (target_type *X_value) {
		if (value != X_value) {
			value->dec_ref();
			value = X_value;
			value->inc_ref();
		}
		return *this;
	}
	this_type & operator = (const this_type &X) {
		if (value != X.value) {
			value->dec_ref();
			value = X.value;
			value->inc_ref();
		}
		return *this;
	}

	bool operator == (const this_type &X) const {
		return value == X.value;
	}
	bool operator != (const this_type &X) const {
		return value != X.value;
	}
	bool operator == (const target_type *X_value) const {
		return value == X_value;
	}
	bool operator != (const target_type *X_value) const {
		return value != X_value;
	}

	bool operator ! () const { return !value; }
	operator target_type* () { return value; }
	operator const target_type* () const { return value; }
	target_type& operator*() { return *value; }
	const target_type& operator*() const { return *value; }
	target_type* operator->() { return value; }
	const target_type* operator->() const { return value; }
	bool is_null() const { return NULL == value; }
	bool is_not_null() const { return NULL != value; }
};

//
//  ●utilities
//
namespace ygg_utility {
	ygg_string make_indent(int indent);
	ygg_string encode_xml(const ygg_string &X);
	ygg_string encode_attribute(const ygg_string &X);
	ygg_string encode_sox(const ygg_string &X);
	ygg_string encode_sox(int indent, const ygg_string &X);
	ygg_string encode_sox(const ygg_string &indent, const ygg_string &X);
	ygg_string decode_xml(const ygg_string &X);
	ygg_string decode_attribute(const ygg_string &X);
	ygg_string decode_sox(int indent, const ygg_string &X);

	ygg_node & format_tree(ygg_node &node, unsigned int max_row_length = 40);

	ygg_string xml_trim(const ygg_string &X);
	ygg_string both_trim(const ygg_string &X);

	ygg_error_code check_name(const ygg_string &X);

	ygg_string & ygg_string_replace(
			ygg_string &body,
			const ygg_string &search, const ygg_string &replace);

	ygg_string 	create_line(ygg_node X_list, const ygg_string &separator);

	namespace base_encoding {
		enum {
			ansi,
			sjis,
			jis,
			euc,
			utf8
		};
	};
	inline const int get_base_encoding() {
		using namespace base_encoding;
#ifdef	__USING_ANSI__
		return ansi;
#endif
#ifdef	__USING_SJIS__
		return sjis;
#endif
#ifdef	__USING_EUC__
		return euc;
#endif
#ifdef	__USING_UTF8__
		return utf8;
#endif
#ifdef	__USING_UNKNOWN__
		const int fingerprint = ((unsigned char*)("漢字"))[0];
		if (0x8A == fingerprint) {
			return sjis;
		}
		if (0x84 == fingerprint) {
			return euc;
		}
		if (0xE6 == fingerprint) {
			return utf8;
		}
		return ansi;
#endif
	}
}

//
//	■yggバリュークラス
//
class ygg_value {
	ygg_string value;

  public:
	ygg_value() {}
	ygg_value(const ygg_string &X_value) :value(X_value) {}
	ygg_value(const char *X_value) :value(X_value) {}
	ygg_value(int X) { operator=(X); }
	ygg_value(double X) { operator=(X); }
	ygg_value(const ygg_value &X) :value(X.value) {}

	ygg_value & operator=(const ygg_string &X_value) {
		return set_string(X_value);
	}
	ygg_value & operator=(const char *X_value) {
		return set_string(X_value);
	}
	ygg_value & operator=(int X) {
		return set_int(X);
	}
	ygg_value & operator=(double X) {
		return set_double(X);
	}
	ygg_value & operator=(const ygg_value &X) {
		return set_string(X.value);
	}

	operator const ygg_string & () const {
		return value;
	}
	operator const char * () const {
		return value.c_str();
	}
	operator const int () const {
		return atol(value.c_str());
	}
	operator const double () const {
		return atof(value.c_str());
	}

	const ygg_string & get_string() const {
		return value;
	}
	const char * get_primary() const {
		return value.c_str();
	}
	const char * c_str() const {
		return value.c_str();
	}
	const int get_int() const {
		return atol(value.c_str());
	}
	const double get_double() const {
		return atof(value.c_str());
	}

	ygg_value & set_string(const ygg_string &X_value) {
		value = X_value;
		return *this;
	}
	ygg_value & set_int(int X);
	ygg_value & set_double(double X);
};

/******************************************************************************
	□■□■                          cuppa                         □■□■
	■□■□                 http://www.unittest.org/               ■□■□
******************************************************************************/

//
//	■yggノードホルダ
//
class ygg_node {
	friend class ygg_ghost;
	friend class ygg_proto_list;
	friend class ygg_root;

	typedef ygg_iterator iterator;
	typedef ygg_reverse_iterator reverse_iterator;

	ygg_smart_shell<ygg_node_body>	body;
//	ygg_node_body *body;

  public:
inline 	ygg_node(ygg_node_body *X_body = NULL);
inline 	ygg_node(const ygg_node &X);
inline 	ygg_node & operator = (ygg_node_body *X_body);
inline 	ygg_node & operator = (const ygg_node &X);
	bool operator == (const ygg_node &X) const {
		return body == X.body;
	}
	bool operator != (const ygg_node &X) const {
		return body != X.body;
	}

inline	ygg_node get_parent() const;
inline	bool is_live() const;	//	←こいつは将来的になくなるかもしれないので
								//		is_valid()/is_empty() を使用してください。
inline	ygg_string get_type() const;
inline	ygg_string get_name() const;
		ygg_string get_path_name() const {
			if (ygg_node_type::attribute == get_type()) {
				return ygg_term::attribute_prefix +get_name();
			} else {
				return get_name();
			}
		}
inline	ygg_value get_value() const;
inline	ygg_string get_text() const;
inline	ygg_string get_xml(const ygg_string &indent = ygg_term::empty) const;
inline	ygg_string get_xml_attribute() const;
inline	ygg_string get_sox(const ygg_string &inden = ygg_term::empty) const;
inline	ygg_node operator[](const ygg_string &path);
inline	ygg_node operator[](const unsigned index);
		const ygg_node operator[](const ygg_string &path) const {
			return ((ygg_node*)this)->operator[](path);
		}
		const ygg_node operator[](const unsigned index) const {
			return ((ygg_node*)this)->operator[](index);
		}
inline 	const int get_size() const;

inline	ygg_iterator begin();
inline	ygg_iterator end();
inline	ygg_reverse_iterator rbegin();
inline	ygg_reverse_iterator rend();
inline	ygg_const_iterator begin() const;
inline	ygg_const_iterator end() const;
inline	ygg_const_reverse_iterator rbegin() const;
inline	ygg_const_reverse_iterator rend() const;

inline	bool match_path(const ygg_string &path) const;

inline	ygg_node & set_name(const ygg_string &X);
inline	ygg_node & set_value(const ygg_value &X);
inline	ygg_node & adopt_node(ygg_node X);
inline	ygg_node & adopt_sox(const ygg_string &sox, sax_handler *sax = NULL) {
			return adopt_node(parse_sox(sox,sax));
		}
inline	ygg_node & adopt_xml(const ygg_string &xml, sax_handler *sax = NULL) {
			return adopt_node(parse_xml(xml,sax));
		}
inline	ygg_node & exile_node(ygg_node X);
inline	ygg_node & exile_path(const ygg_string &path) {
			return exile_node(operator[](path));
		}
		ygg_node & self_exile();

inline  ygg_node clone() const;

inline	const bool operator !() const {
			return !is_valid();
		}
//inline	operator const bool() const {
//			return is_valid();
//		}

	//	alias functions
		bool is_valid() const {
			return is_live();
		}
		bool is_empty() const {
			return !is_live();
		}
inline	ygg_node & add_node(ygg_node X);
inline	ygg_node & sub_node(ygg_node X);

#ifdef _DEBUG
inline	bool assert_other(const ygg_node &X) const;
#endif

		ygg_node & vanish(); //  親ノードからの exile 及び、this(ygg_node)の空ノード化
		ygg_string get_path() const;
		ygg_node & purge(); //  結合可能なテキストノードの結合、除霊、空ノードの exile、空リストの空ノード化
		ygg_node enum_node(const ygg_string &path_where);

static inline ygg_node parse_sox(const ygg_string &sox, sax_handler *sax = NULL);
static inline ygg_node parse_xml(const ygg_string &xml, sax_handler *sax = NULL);

  protected:
inline	ygg_node & regist_parent(ygg_root *X);
inline	ygg_node & unregist_parent(ygg_root *X);
};

//
//	□yggノードクラス
//
class ygg_node_body :public ygg_smart_base {
	friend class ygg_node;

  protected:
	ygg_root	*parent;

	ygg_node_body() :parent(NULL) {}
	virtual	~ygg_node_body() { }

		ygg_node & get_parent() {
			return *((ygg_node *)&parent);
		}
		const ygg_node & get_parent() const {
			return *((ygg_node *)&parent);
		}
virtual bool is_live() const = 0;
virtual	ygg_string get_type() const = 0;
virtual	ygg_string get_name() const = 0;
virtual	ygg_string get_value() const = 0;
virtual	ygg_string get_text() const = 0;
virtual	ygg_string get_xml(const ygg_string &indent) const = 0;
virtual	ygg_string get_xml_attribute() const = 0;
virtual	ygg_string get_sox(const ygg_string &indent) const = 0;
virtual	ygg_node operator[](const ygg_string &path);
virtual	ygg_node operator[](const unsigned index);
virtual	const int get_size() const;
virtual	ygg_iterator begin();
virtual	ygg_iterator end();
virtual	ygg_reverse_iterator rbegin();
virtual	ygg_reverse_iterator rend();
virtual	ygg_const_iterator begin() const;
virtual	ygg_const_iterator end() const;
virtual	ygg_const_reverse_iterator rbegin() const;
virtual	ygg_const_reverse_iterator rend() const;

virtual	bool match_path(const ygg_string &path) const;

virtual	void set_name(const ygg_string &X);
virtual	void set_value(const ygg_string &X);
virtual	void adopt_node(ygg_node X);
virtual	void exile_node(ygg_node X);

virtual ygg_node clone() const = 0;

		ygg_node get_shell() {
			return this;
		}
		const ygg_node get_shell() const {
			return (ygg_node_body*)this;
		}

#ifdef _DEBUG
virtual	bool assert_other(const ygg_node &X) const;
#endif

inline
	void regist_parent(ygg_root *X);
	void unregist_parent(ygg_root *X);
};

inline ygg_node::ygg_node(ygg_node_body *X_body) :body(X_body) {}
inline ygg_node::ygg_node(const ygg_node &X) :body(X.body) {}
inline ygg_node & ygg_node::operator = (ygg_node_body *X_body) {
	body = X_body;
	return *this;
}
inline ygg_node & ygg_node::operator = (const ygg_node &X) {
	body = X.body;
	return *this;
}

inline ygg_node ygg_node::get_parent() const {
	assert(body.is_not_null());
	if (body.is_null()) {
		return NULL;
	} else {
		return body->get_parent();
	}
}
inline bool ygg_node::is_live() const {
//	assert(body.is_not_null());
	if (body.is_null()) {
		return false;
	} else {
		return body->is_live();
	}
}
inline ygg_string ygg_node::get_type() const {
	if (body.is_null()) {
		return ygg_node_type::empty;
	} else {
		return body->get_type();
	}
}
inline ygg_string ygg_node::get_name() const {
	assert(body.is_not_null());
	if (body.is_null()) {
		return ygg_term::empty;
	} else {
		return body->get_name();
	}
}
inline ygg_string ygg_node::get_text() const {
	assert(body.is_not_null());
	if (body.is_null()) {
		return ygg_term::empty;
	} else {
		return body->get_text();
	}
}
inline ygg_string ygg_node::get_xml(const ygg_string &indent) const {
	assert(body.is_not_null());
	if (body.is_null()) {
		return ygg_term::empty;
	} else {
		return body->get_xml(indent);
	}
}
inline ygg_string ygg_node::get_xml_attribute() const {
	assert(body.is_not_null());
	if (body.is_null()) {
		return ygg_term::empty;
	} else {
		return body->get_xml_attribute();
	}
}
inline ygg_string ygg_node::get_sox(const ygg_string &indent) const {
	assert(body.is_not_null());
	if (body.is_null()) {
		return ygg_term::empty;
	} else {
		return body->get_sox(indent);
	}
}
inline ygg_value ygg_node::get_value() const {
	if (body.is_null()) {
		return ygg_term::empty;
	} else {
		return body->get_value();
	}
}
inline ygg_node ygg_node::operator[](const ygg_string &path) {
//	assert(body.is_not_null());
	if (body.is_null()) {
		return NULL;
	} else {
		return body->operator[](path);
	}
}
inline ygg_node ygg_node::operator[](const unsigned index) {
//	assert(body.is_not_null());
	if (body.is_null()) {
		return NULL;
	} else {
		return body->operator[](index);
	}
}
inline const int ygg_node::get_size() const {
//	assert(body.is_not_null());
	if (body.is_null()) {
		return 0;
	} else {
		return body->get_size();
	}
}

inline bool ygg_node::match_path(const ygg_string &path) const {
	if (body.is_null()) {
		return false;
	} else {
		return body->match_path(path);
	}
}


inline ygg_node & ygg_node::set_name(const ygg_string &X) {
	assert(body.is_not_null());
	if (body.is_not_null()) {
		body->set_name(X);
	}
	return *this;
}
inline ygg_node & ygg_node::set_value(const ygg_value &X) {
	assert(body.is_not_null());
	if (body.is_not_null()) {
		body->set_value(X.get_string());
	}
	return *this;
}
inline ygg_node & ygg_node::adopt_node(ygg_node X) {
	assert(body.is_not_null());
	if (body.is_not_null()) {
		body->adopt_node(X);
	}
	return *this;
}
inline ygg_node & ygg_node::exile_node(ygg_node X) {
	assert(body.is_not_null());
	if (body.is_not_null()) {
		body->exile_node(X);
	}
	return *this;
}
inline ygg_node ygg_node::clone() const {
	if (body.is_null()) {
		return NULL;
	} else {
		return body->clone();
	}
}

inline	ygg_node & ygg_node::add_node(ygg_node X) {
	return adopt_node(X);
}
inline	ygg_node & ygg_node::sub_node(ygg_node X) {
	return exile_node(X);
}

#ifdef _DEBUG
inline bool ygg_node::assert_other(const ygg_node &X) const {
	assert(body.is_not_null());
	return body->assert_other(X);
}
#endif

inline ygg_node & ygg_node::regist_parent(ygg_root *X) {
	assert(body.is_not_null());
	if (body.is_not_null()) {
		body->regist_parent(X);
	}
	return *this;
}
inline ygg_node & ygg_node::unregist_parent(ygg_root *X) {
	assert(body.is_not_null());
	if (body.is_not_null()) {
		body->unregist_parent(X);
	}
	return *this;
}

//
//	■yggゴーストクラス
//
class ygg_ghost :public ygg_node_body {
	ygg_string path;

  protected:
	ygg_ghost(ygg_root *X_parent, const ygg_string &X_path) {
		parent = X_parent;
		path = X_path;
	}

	ygg_node get_life() const;
	ygg_node realize() const;

	bool is_live() const;
	ygg_string get_type() const;
	ygg_string get_name() const;
	ygg_string get_value() const;
	ygg_string get_text() const;
	ygg_string get_xml(const ygg_string &indent = ygg_term::empty) const;
	ygg_string get_xml_attribute() const;
	ygg_string get_sox(const ygg_string &indent) const;

	ygg_node operator[](const ygg_string &path);
	ygg_node operator[](const unsigned index);
	const int get_size() const;

	bool match_path(const ygg_string &path) const;

	void set_name(const ygg_string &X);
	void set_value(const ygg_string &X);
	void adopt_node(ygg_node X);
	void exile_node(ygg_node X);

	ygg_node clone() const;

#ifdef _DEBUG
	bool assert_other(const ygg_node &X) const;
#endif

  public:
static
	ygg_node create(ygg_root *X_parent, const ygg_string &X_path) {
		return new ygg_ghost(X_parent, X_path);
	}
};

//
//	■yggコメントクラス
//
class ygg_comment :public ygg_node_body {

	ygg_string value;

  protected:
	ygg_comment() {}

	bool is_live() const;
	ygg_string get_type() const;
	ygg_string get_name() const;
	ygg_string get_value() const;
	ygg_string get_text() const;
	ygg_string get_xml(const ygg_string &indent = ygg_term::empty) const;
	ygg_string get_xml_attribute() const;
	ygg_string get_sox(const ygg_string &indent) const;

	bool match_path(const ygg_string &path) const;

	void set_value(const ygg_string &X);

	ygg_node clone() const;

  public:
static
	ygg_node create() {
		return new ygg_comment();
	}
};

//
//	■yggテキストクラス
//
class ygg_text :public ygg_node_body {

	ygg_string value;

  protected:
	ygg_text() {}

	bool is_live() const;
	ygg_string get_type() const;
	ygg_string get_name() const;
	ygg_string get_value() const;
	ygg_string get_text() const;
	ygg_string get_xml(const ygg_string &indent = ygg_term::empty) const;
	ygg_string get_xml_attribute() const;
	ygg_string get_sox(const ygg_string &indent) const;

	bool match_path(const ygg_string &path) const;

	void set_value(const ygg_string &X);

	ygg_node clone() const;

  public:
static
	ygg_node create() {
		return new ygg_text();
	}
};

//
//	■ygg属性クラス
//
class ygg_attribute :public ygg_node_body {

	ygg_string name;
	ygg_string value;

  protected:
	ygg_attribute()	{}

	bool is_live() const;
	ygg_string get_type() const;
	ygg_string get_name() const;
	ygg_string get_value() const;
	ygg_string get_text() const;
	ygg_string get_xml(const ygg_string &indent = ygg_term::empty) const;
	ygg_string get_xml_attribute() const;
	ygg_string get_sox(const ygg_string &indent) const;

	bool match_path(const ygg_string &path) const;

	void set_name(const ygg_string &X);
	void set_value(const ygg_string &X);

	ygg_node clone() const;

  public:
static
	ygg_node create() {
			return new ygg_attribute();
		}
};

//
//  ▼使用するコンテナクラス [ 〆 ]
//
typedef std::list<ygg_node> ygg_list_type;
//typedef std::iterator<std::bidirectional_iterator_tag, ygg_node> ygg_iterator_base;
//typedef std::bidirectional_iterator<ygg_node, ptrdiff_t> ygg_iterator_base;
struct ygg_iterator_base {
  typedef std::bidirectional_iterator_tag	iterator_category;
  typedef ygg_node							value_type;
  typedef ptrdiff_t							difference_type;
  typedef ygg_node*							pointer;
  typedef ygg_node&							reference;
};

class ygg_iterator :public ygg_iterator_base {
	friend class ygg_const_iterator;

	ygg_node	list;
	ygg_list_type::iterator iterator;

	public:
	ygg_iterator(ygg_node X_list, ygg_list_type::iterator X_iterator)
			:list(X_list), iterator(X_iterator) {}
	ygg_iterator(const ygg_iterator &X)
			:list(X.list), iterator(X.iterator) {}

	const ygg_node & get_list() const {
		return list;
	}
inline	bool is_begin() const;
inline	bool is_not_begin() const;
inline	bool is_end() const;
inline	bool is_not_end() const;

	ygg_iterator & operator++() {
		++iterator;
		return *this;
	}
	const ygg_iterator operator++(int) {
		return ygg_iterator(list, iterator++);
	}

	ygg_iterator & operator -- () {
		--iterator;
		return *this;
	}
	const ygg_iterator operator -- (int) {
		return ygg_iterator(list, iterator--);
	}

	ygg_node & operator*() {
		return *iterator;
	}
	ygg_node * operator->() {
		return iterator.operator->();
	}

	bool operator==(const ygg_iterator &X) const {
		assert(list == X.list);
		return iterator == X.iterator;
	}
	bool operator!=(const ygg_iterator &X) const {
		assert(list == X.list);
		return iterator != X.iterator;
	}
};
class ygg_reverse_iterator :public ygg_iterator_base {
	friend class ygg_const_reverse_iterator;

	ygg_node	list;
	ygg_list_type::reverse_iterator iterator;

	public:
	ygg_reverse_iterator(ygg_node X_list, ygg_list_type::reverse_iterator X_iterator)
			:list(X_list), iterator(X_iterator) {}
	ygg_reverse_iterator(const ygg_reverse_iterator &X)
			:list(X.list), iterator(X.iterator) {}

	const ygg_node & get_list() const {
		return list;
	}
inline	bool is_rbegin() const;
inline	bool is_not_rbegin() const;
inline	bool is_rend() const;
inline	bool is_not_rend() const;

	ygg_reverse_iterator & operator++() {
		++iterator; return *this;
	}
	const ygg_reverse_iterator operator++(int) {
		return ygg_reverse_iterator(list, iterator++);
	}

	ygg_reverse_iterator & operator--() {
		--iterator;
		return *this;
	}
	const ygg_reverse_iterator operator--(int) {
		return ygg_reverse_iterator(list, iterator--);
	}

	ygg_node & operator*() {
		return *iterator;
	}
	ygg_node * operator->() {
		return iterator.operator->();
	}

	bool operator==(const ygg_reverse_iterator &X) const {
		assert(list == X.list);
		return iterator == X.iterator;
	}
	bool operator!=(const ygg_reverse_iterator &X) const {
		assert(list == X.list);
		return iterator != X.iterator;
	}
};
class ygg_const_iterator :public ygg_iterator_base {
	friend class ygg_iterator;

	const ygg_node	list;
	ygg_list_type::const_iterator iterator;

	public:
	ygg_const_iterator(const ygg_node X_list, ygg_list_type::const_iterator X_iterator)
			:list(X_list), iterator(X_iterator) {}
	ygg_const_iterator(const ygg_iterator &X)
			:list(X.list), iterator(X.iterator) {}
	ygg_const_iterator(const ygg_const_iterator &X)
			:list(X.list), iterator(X.iterator) {}

	const ygg_node & get_list() const {
		return list;
	}
	bool is_begin() const {
		return iterator == list.begin().iterator;
	}
	bool is_not_begin() const {
		return iterator != list.begin().iterator;
	}
	bool is_end() const {
		return iterator == list.end().iterator;
	}
	bool is_not_end() const {
		return iterator != list.end().iterator;
	}

	ygg_const_iterator & operator++() {
		++iterator;
		return *this;
	}
	const ygg_const_iterator operator++(int) {
		return ygg_const_iterator(list, iterator++);
	}

	ygg_const_iterator & operator--() {
		--iterator;
		return *this;
	}
	const ygg_const_iterator operator--(int) {
		return ygg_const_iterator(list, iterator--);
	}

	const ygg_node & operator*() const {
		return *iterator;
	}
	const ygg_node * operator->() const {
		return iterator.operator->();
	}

	bool operator==(const ygg_const_iterator &X) const {
		assert(list == X.list);
		return iterator == X.iterator;
	}
	bool operator!=(const ygg_const_iterator &X) const {
		assert(list == X.list);
		return iterator != X.iterator;
	}
};
class ygg_const_reverse_iterator :public ygg_iterator_base {
	friend class ygg_reverse_iterator;

	const ygg_node	list;
	ygg_list_type::const_reverse_iterator iterator;

	public:
	ygg_const_reverse_iterator(ygg_node X_list, ygg_list_type::const_reverse_iterator X_iterator)
			:list(X_list), iterator(X_iterator) {}
	ygg_const_reverse_iterator(const ygg_reverse_iterator &X)
#if defined(_MSC_VER) && (_MSC_VER < 1300)
			:list(X.list), iterator((ygg_list_type::const_reverse_iterator&)(X.iterator)) {}
#else
			:list(X.list), iterator(X.iterator) {}
#endif
	ygg_const_reverse_iterator(const ygg_const_reverse_iterator &X)
			:list(X.list), iterator(X.iterator) {}

	const ygg_node & get_list() const {
		return list;
	}
	bool is_rbegin() const {
		return iterator == list.rbegin().iterator;
	}
	bool is_not_rbegin() const {
		return iterator != list.rbegin().iterator;
	}
	bool is_rend() const {
		return iterator == list.rend().iterator;
	}
	bool is_not_rend() const {
		return iterator != list.rend().iterator;
	}

	ygg_const_reverse_iterator & operator++() {
		++iterator;
		return *this;
	}
	const ygg_const_reverse_iterator operator++(int) {
		return ygg_const_reverse_iterator(list, iterator++);
	}

	ygg_const_reverse_iterator & operator--() {
		--iterator;
		return *this;
	}
	const ygg_const_reverse_iterator operator--(int) {
		return ygg_const_reverse_iterator(list, iterator--);
	}

	const ygg_node & operator*() const {
		return *iterator;
	}
	const ygg_node * operator->() const {
		return iterator.operator->();
	}

	bool operator==(const ygg_const_reverse_iterator &X) const {
		assert(list == X.list);
		return iterator == X.iterator;
	}
	bool operator!=(const ygg_const_reverse_iterator &X) const {
		assert(list == X.list);
		return iterator != X.iterator;
	}
};
inline bool ygg_iterator::is_begin() const {
	return *((ygg_const_iterator*)(this)) == list.begin();
}
inline bool ygg_iterator::is_not_begin() const {
	return *((ygg_const_iterator*)(this)) != list.begin();
}
inline bool ygg_iterator::is_end() const {
	return *((ygg_const_iterator*)(this)) == list.end();
}
inline bool ygg_iterator::is_not_end() const {
	return *((ygg_const_iterator*)(this)) != list.end();
}
inline bool ygg_reverse_iterator::is_rbegin() const {
	return *((ygg_const_reverse_iterator*)(this)) == list.rbegin();
}
inline bool ygg_reverse_iterator::is_not_rbegin() const {
	return *((ygg_const_reverse_iterator*)(this)) != list.rbegin();
}
inline bool ygg_reverse_iterator::is_rend() const {
	return *((ygg_const_reverse_iterator*)(this)) == list.rend();
}
inline bool ygg_reverse_iterator::is_not_rend() const {
	return *((ygg_const_reverse_iterator*)(this)) != list.rend();
}


//
//	■yggリストクラス
//
class ygg_proto_list :public ygg_node_body {
	friend class ygg_node_body;

  protected:
	ygg_list_type	body;

	ygg_proto_list() {}

	bool is_live() const;
	ygg_string get_value() const;
	ygg_string get_text() const;
	ygg_string get_attribute() const;
	ygg_string get_xml(const ygg_string &indent = ygg_term::empty) const;
	ygg_string get_xml_attribute() const;
	ygg_string get_sox(const ygg_string &indent) const;

	void adopt_node(ygg_node X);
	void exile_node(ygg_node X);

	ygg_proto_list * adopt_child(ygg_node X_node);
	ygg_proto_list * exile_child(ygg_node X_node);

#ifdef _DEBUG
	bool assert_other(const ygg_node &X) const;
#endif

inline	ygg_node get_single() {
			return operator[](0);
		}

};
class ygg_list :public ygg_proto_list {

  protected:
	ygg_list() {}

	ygg_string get_type() const;
	ygg_string get_name() const;
	ygg_node operator[](const ygg_string &path);
	ygg_node operator[](const unsigned index);
	const int get_size() const;
	ygg_iterator begin();
	ygg_iterator end();
	ygg_reverse_iterator rbegin();
	ygg_reverse_iterator rend();
	ygg_const_iterator begin() const;
	ygg_const_iterator end() const;
	ygg_const_reverse_iterator rbegin() const;
	ygg_const_reverse_iterator rend() const;

	bool match_path(const ygg_string &path) const;

	ygg_node clone() const;

  public:
static
	ygg_node create() {
		return new ygg_list();
	}

};

inline ygg_iterator ygg_node::begin() {
	if (ygg_node_type::list == get_type()) {
		return body->begin();
	} else {
		return ygg_list::create().adopt_node(*this).begin();
	}
}
inline ygg_iterator ygg_node::end() {
	if (ygg_node_type::list == get_type()) {
		return body->end();
	} else {
		return ygg_list::create().adopt_node(*this).end();
	}
}
inline ygg_reverse_iterator ygg_node::rbegin() {
	if (ygg_node_type::list == get_type()) {
		return body->rbegin();
	} else {
		return ygg_list::create().adopt_node(*this).rbegin();
	}
}
inline ygg_reverse_iterator ygg_node::rend() {
	if (ygg_node_type::list == get_type()) {
		return body->rend();
	} else {
		return ygg_list::create().adopt_node(*this).rend();
	}
}
inline ygg_const_iterator ygg_node::begin() const {
	if (ygg_node_type::list == get_type()) {
		return body->begin();
	} else {
		return ygg_list::create().adopt_node(*this).begin();
	}
}
inline ygg_const_iterator ygg_node::end() const {
	if (ygg_node_type::list == get_type()) {
		return body->end();
	} else {
		return ygg_list::create().adopt_node(*this).end();
	}
}
inline ygg_const_reverse_iterator ygg_node::rbegin() const {
	if (ygg_node_type::list == get_type()) {
		return body->rbegin();
	} else {
		return ygg_list::create().adopt_node(*this).rbegin();
	}
}
inline ygg_const_reverse_iterator ygg_node::rend() const {
	if (ygg_node_type::list == get_type()) {
		return body->rend();
	} else {
		return ygg_list::create().adopt_node(*this).rend();
	}
}

//
//	■ygg基底クラス
//
class ygg_root :public ygg_proto_list {
	friend class ygg_ghost;

  protected:
	ygg_root() {}
	~ygg_root() {
		exile_node(operator[](ygg_term::node));
	}

	ygg_string get_type() const;
	ygg_string get_name() const;
	ygg_node operator[](const ygg_string &path);

	bool match_path(const ygg_string &path) const;

	void adopt_node(ygg_node X);
	void exile_node(ygg_node X);

	ygg_node clone() const;

  public:
static ygg_node create() {
		return new ygg_root;
	}
static inline ygg_node parse_sox(const ygg_string &sox, sax_handler *sax = NULL);
static inline ygg_node parse_xml(const ygg_string &xml, sax_handler *sax = NULL);
};

inline ygg_node ygg_node::parse_sox(const ygg_string &sox, sax_handler *sax) {
	ygg_node node = ygg_root::parse_sox(sox, sax)[ygg_term::node];
//	node.self_exile();
	return node;
}
inline ygg_node ygg_node::parse_xml(const ygg_string &xml, sax_handler *sax) {
	return ygg_root::parse_xml(xml, sax)[ygg_term::node];
}

//
//	■ygg要素クラス
//
class ygg_element :public ygg_root {
	ygg_string	name;

  protected:
	ygg_element() {}

	ygg_string get_type() const;
	ygg_string get_name() const;
//	ygg_string get_value() const; use ygg_list::get_value()
//	ygg_string get_text() const; use ygg_list::get_text()
	ygg_string get_xml(const ygg_string &indent = ygg_term::empty) const;
	ygg_string get_xml_attribute() const;
	ygg_string get_sox(const ygg_string &indent) const;

	bool match_path(const ygg_string &path) const;

	void set_name(const ygg_string &X);
	void set_value(const ygg_string &X);

	ygg_node clone() const;

  public:
static
	ygg_node create() {
		return new ygg_element;
	}
};
inline void ygg_node_body::regist_parent(ygg_root *X) {
	assert(NULL == parent);
	//assert(("このノードには既に親がいます。いったん '親ノード.exile_node(このノード)' で絶縁するか、'このノード.close()' でクローンを作成してください。", NULL == parent));
	if (NULL != this && ygg_node_type::empty != get_type()) {
		parent = X;
		X->adopt_child(this);
	}
}
inline void ygg_node_body::unregist_parent(ygg_root *X) {
	if (NULL != this && ygg_node_type::empty != get_type()) {
		parent = NULL;
		X->exile_child(this);
	}
}

/******************************************************************************
	□■□■                 Trick Library 'dagger'                 □■□■
	■□■□             http://tricklib.com/cxx/dagger/            ■□■□
******************************************************************************/

//
//	■ygg_position
//
class ygg_position {
public:
//struct ygg_position {

	int line;
	int row;

	ygg_position(int X_line = 1, int X_row = 1)
		:line(X_line), row(X_row) {}
	ygg_position(const ygg_position &X)
		:line(X.line), row(X.row) {}

	ygg_position & set_line(int X_line = 1) {
		line = X_line;
		return *this;
	}
	ygg_position & set_row(int X_row = 1) {
		row = X_row;
		return *this;
	}
	ygg_position & set_position(int X_line = 1, int X_row = 1) {
		line = X_line;
		row = X_row;
		return *this;
	}
	ygg_position & set_position(const ygg_position &X) {
		line = X.line;
		row = X.row;
		return *this;
	}
	ygg_position & clear() {
		return set_position();
	}
	const int get_line() const {
		return line;
	}
	const int get_row() const {
		return row;
	}

	ygg_position & next_line() {
		++line;
		return set_row();
	}
	const ygg_position operator+(const ygg_string &X) const {
		return ygg_position() += X;
	}
	ygg_position & operator+=(const ygg_string &X);

};

//
//	■ygg_error_term(1/2)
//
namespace ygg_error_term {
	const int genre			= 0x01000000;
	const int category		= 0x00010000;
	const int sequence		= 0x00000100;
	const int sequence_case	= 0x00000001;

	const ygg_string default_message = "何らかのエラーが発生しました。";
};

class ygg_error_code {

  protected:
	int value;

  public:

	ygg_error_code(int X_value = 0) :value(X_value) {}
	ygg_error_code(const ygg_error_code &X, int X_case = 0) :value(X.value +X_case) {
		assert(0 == X.get_sequence_case() || 0 == X_case);
	}

	bool is_error() const {
		return 0 != get_code();
	}

	const int get_strict_code() const {
		return value;
	}
	const int get_code() const {
		return get_strict_code() & -0x100;
	}

	const int get_spectrum(int spectrum) const {
		return (get_strict_code() / spectrum) & 0xFF;
	}
	const int get_genre() const {
		return get_spectrum(ygg_error_term::genre);
	}
	const int get_category() const {
		return get_spectrum(ygg_error_term::category);
	}
	const int get_sequence() const {
		return get_spectrum(ygg_error_term::sequence);
	}
	const int get_sequence_case() const {
		return get_spectrum(ygg_error_term::sequence_case);
	}

	ygg_error_code & set_code(int X_value) {
		value = X_value;
		return *this;
	}
	ygg_error_code & set_code(const ygg_error_code &X) {
		value = X.value;
		return *this;
	}
};

//
//	■ygg_error_term(2/2)
//
namespace ygg_error_term {

inline const ygg_error_code make_genre(int X_genre) {
		return X_genre *genre;
	}
inline const ygg_error_code make_category(const ygg_error_code &X_genre, int X_category) {
		return X_genre.get_code() +X_category *category;
	}
inline const ygg_error_code make_code(const ygg_error_code &X_category, int X_sequence, int X_sequence_case = 0) {
		return X_category.get_code() +X_sequence *sequence +X_sequence_case *sequence_case;
	}
inline const ygg_error_code make_strict_code(const ygg_error_code &X_sequence, int X_sequence_case) {
		assert(0 == X_sequence.get_sequence_case());
		return X_sequence.get_code() +X_sequence_case *sequence_case;
	}

const ygg_error_code default_genre		= make_genre(0x00);
const ygg_error_code default_category	= make_category(default_genre, 0x00);
const ygg_error_code no_error			= make_code(default_category, 0x00);
const ygg_error_code default_error		= make_code(default_category, 0x01);

const ygg_error_code device				= make_genre(0x01);
const ygg_error_code file				= make_category(device, 0x01);
const ygg_error_code cannot_open_file	= make_code(file, 0x01);
const ygg_error_code stream				= make_category(device, 0x02);
const ygg_error_code invalid_stream		= make_code(stream, 0x01);

const ygg_error_code syntax				= make_genre(0x02);
const ygg_error_code ygg_syntax			= make_category(syntax, 0x00);
const ygg_error_code ygg_broken_char	= make_code(ygg_syntax, 0x01);
const ygg_error_code ygg_invalid_name	= make_code(ygg_syntax, 0x02);
const ygg_error_code ygg_invalid_name_a	= make_strict_code(ygg_invalid_name, 0x01);
const ygg_error_code ygg_invalid_name_b	= make_strict_code(ygg_invalid_name, 0x02);
const ygg_error_code duplication_attribute
										= make_code(ygg_syntax, 0x03);
const ygg_error_code many_roots			= make_code(ygg_syntax, 0x04);
const ygg_error_code sox_syntax			= make_category(syntax, 0x01);
const ygg_error_code sox_deep_indent	= make_code(sox_syntax, 0x01);
const ygg_error_code xml_syntax			= make_category(syntax, 0x02);
const ygg_error_code unmatch_tags		= make_code(xml_syntax, 0x01);
const ygg_error_code unmatch_tags_a		= make_strict_code(unmatch_tags, 0x01);
const ygg_error_code unmatch_tags_b		= make_strict_code(unmatch_tags, 0x02);
const ygg_error_code csv_syntax			= make_category(syntax, 0x03);

const ygg_error_code memory				= make_genre(0x03);

const ygg_error_code user				= make_genre(0x04);

};


//
//	■yggエラークラス
//
class ygg_error :public ygg_error_code, public ygg_position {
	ygg_string message;
  public:
	ygg_error(const ygg_error &X)
		:ygg_error_code(X), ygg_position(X), message(X.message) {}
	ygg_error(const ygg_error_code &X = ygg_error_term::no_error, const ygg_position &X_position = ygg_position(0,0), const ygg_string &X_message = ygg_term::empty)
		:ygg_error_code(X), ygg_position(X_position), message(X_message) {}

	ygg_error & set_error(const ygg_error_code &X = ygg_error_term::no_error, const ygg_position &X_position = ygg_position(0,0), const ygg_string &X_message = ygg_term::empty) {
		set_code(X);
		set_position(X_position);
		message = X_message;
		return *this;
	}
	ygg_error & clear() {
		return set_error();
	}

	const ygg_string & get_message() const {
		return message;
	}
	ygg_error & set_message(const ygg_string & X_message) {
		message = X_message;
		return *this;
	}

};


//
//	■SAXハンドラ基本クラス
//
class sax_handler {
	friend class ygg_parser;
	ygg_parser *parser;

	void set_parser(ygg_parser *X_parser) {
		parser = X_parser;
	}
  protected:
	ygg_parser * get_parser();
  public:
	virtual void on_error(ygg_error* parse_error);	//	parse_error->clear() 可
	virtual void start_document(ygg_node root);		//	コメントノードの付加のみ 可
	virtual void end_document(ygg_node &root);		//	root.vanish() および、その他の全操作 可
	virtual void start_element(ygg_node element);	//	属性の全操作 可
	virtual void end_element(ygg_node element);		//	element.vanish() および、同element内のその他の全操作 可
	virtual void catch_text(ygg_node text);			//	text.set_value(...) 可
	virtual void catch_comment(ygg_node comment);	//	comment.set_value(...) 可
};
//
//
//
class error_catcher :public sax_handler {
  public:
	ygg_error parse_error;
	virtual ~error_catcher() {}
	void on_error(ygg_error* X_parse_error) {
		this->parse_error = *X_parse_error;
	}
};

//
//	□yggパーザクラス
//
class ygg_parser {
  protected:
	ygg_position anchor_position, hot_position;
	sax_handler *sax;

	ygg_parser (sax_handler *X_sax = NULL) :anchor_position(), hot_position(), sax(X_sax) {
		if (sax) {
			sax->set_parser(this);
		}
	}

	const ygg_error_code check_name(const ygg_string &type, const ygg_string &X);
	const ygg_error_code check_element_name(const ygg_string &X) {
		return check_name("要素名", X);
	}
	const ygg_error_code check_attribute_name(const ygg_string &X) {
		return check_name("属性名", X);
	}

  public:

	ygg_error parse_error;

	ygg_parser * set_sax_handler(sax_handler *X_sax);
	const int get_line() const {
		return anchor_position.get_line();
	}
	const int get_row() const {
		return anchor_position.get_row();
	}
	void raise_error(const ygg_error_code &X_code = ygg_error_term::default_error, const ygg_string &X_message = ygg_error_term::default_message) {
		raise_error(X_code, anchor_position, X_message);
	}
	void raise_error(const ygg_error_code &X_code, const ygg_position &X_position, const ygg_string &X_message = ygg_error_term::default_message) {
		parse_error.set_error(X_code, X_position, X_message);
		if (sax) {
			sax->on_error(&parse_error);
		}
	}
};


//
//	■SOXパーザクラス
//
class sox_parser :public ygg_parser {
	ygg_node &root;
	ygg_node hot_element;
	ygg_node last_node;
	ygg_node indent_node_list;
	int anchor_indent;
	ygg_string unparsed_buffer;

  public:

	sox_parser(ygg_node &X_root, sax_handler *X_sax = NULL)
											:ygg_parser(X_sax), root(X_root) {}
	sox_parser & set_sax_handler(sax_handler *X_sax) { set_sax_handler(X_sax); return *this; }
#if defined(__YGGDRASIL_WITH_BABEL__)
	sox_parser & read(std::istream &stream, int encoding = babel::base_encoding::unknown);
	sox_parser & write(std::ostream &stream, int encoding = babel::base_encoding::unknown);
	sox_parser & load(const ygg_string &X_filename, int encoding = babel::base_encoding::unknown);
	sox_parser & save(const ygg_string &X_filename, int encoding = babel::base_encoding::unknown);
#else //	defined(__YGGDRASIL_WITH_BABEL__)
	sox_parser & read(std::istream &stream);
	sox_parser & write(std::ostream &stream);
	sox_parser & load(const ygg_string &X_filename);
	sox_parser & save(const ygg_string &X_filename);
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	sox_parser & init_root();
	sox_parser & parse_line(const ygg_string &X_line);
	sox_parser & parse_string(const ygg_string &X_text);
	sox_parser & flush();
	sox_parser & end_stream();

  protected:
	void parse(const ygg_string &X_line);
	void catch_event(ygg_node node);
};
#if defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::read(std::istream &stream, int encoding) {
#else //	defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::read(std::istream &stream) {
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	init_root();
	if (!stream) {
		raise_error(ygg_error_term::invalid_stream, "無効なストリームが指定されました。");
	} else {
#if defined(__YGGDRASIL_WITH_BABEL__)
		using namespace babel;

		ygg_string::value_type first_buffer[4096];
		ygg_string second_buffer, third_buffer;
		if (!stream.eof()) {

			stream.read(first_buffer, 4096);
			second_buffer.assign(first_buffer, stream.gcount());

			const int determine_encoding = (babel::base_encoding::unknown == encoding) ? analyze_base_encoding(second_buffer): encoding;
			bbl_translater<bbl_binary, ygg_string> translater = manual_translate_engine<bbl_binary, ygg_string>::order(determine_encoding);

			while(true) {
				translater << second_buffer;
				translater >> third_buffer;
				parse_string(third_buffer);
				if (stream.eof() || parse_error.is_error()) {
					break;
				}
				stream.read(first_buffer, 1024);
				second_buffer.assign(first_buffer, stream.gcount());
			}
		}
#else //	defined(__YGGDRASIL_WITH_BABEL__)
		ygg_string buffer;
		while(!stream.eof() && !parse_error.is_error()) {
			std::getline(stream, buffer);
			parse_line(buffer);
		}
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	}
	end_stream();
	return *this;

}
#if defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::write(std::ostream &stream, int encoding) {
	using namespace babel;
#else //	defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::write(std::ostream &stream) {
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	if (!stream) {
		raise_error(ygg_error_term::invalid_stream, "無効なストリームが指定されました。");
		return *this;												//	return
	}
#if defined(__YGGDRASIL_WITH_BABEL__)
	stream << translate_to_binary(root.get_sox(), encoding);
#else //	defined(__YGGDRASIL_WITH_BABEL__)
	stream << root.get_sox();
#endif //	defined(__YGGDRASIL_WITH_BABEL__)


	return *this;													//	return
}
#if defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::load(const ygg_string &X_filename, int encoding) {
	std::ifstream file(X_filename.c_str(), std::ios::binary);
#else //	defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::load(const ygg_string &X_filename) {
	std::ifstream file(X_filename.c_str());
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	if (!file) {
		raise_error(ygg_error_term::cannot_open_file, "ファイル " +X_filename +" を開けませんでした。");
		return *this;												//	return
	}

#if defined(__YGGDRASIL_WITH_BABEL__)
	return read(file, encoding);									//	return
#else //	defined(__YGGDRASIL_WITH_BABEL__)
	return read(file);												//	return
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
}
#if defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::save(const ygg_string &X_filename, int encoding) {
	std::ofstream file(X_filename.c_str(), std::ios::binary);
#else //	defined(__YGGDRASIL_WITH_BABEL__)
inline sox_parser & sox_parser::save(const ygg_string &X_filename) {
	std::ofstream file(X_filename.c_str());
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	if (!file) {
		raise_error(ygg_error_term::cannot_open_file, "ファイル " +X_filename +" を開けませんでした。");
		return *this;												//	return
	}
#if defined(__YGGDRASIL_WITH_BABEL__)
	return write(file, encoding);									//	return
#else //	defined(__YGGDRASIL_WITH_BABEL__)
	return write(file);												//	return
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
}

//
//
//
class sox_file :public ygg_node {
	ygg_string filename;

  public:

	ygg_error parse_error;

	sox_file(const ygg_string &X_filename) :filename(X_filename) {}
	~sox_file() {}

#if defined(__YGGDRASIL_WITH_BABEL__)
	sox_file & read(sax_handler *sax = NULL, int encoding = babel::base_encoding::unknown) {
		parse_error = sox_parser(*this, sax).load(filename, encoding).parse_error;
#else // defined(__YGGDRASIL_WITH_BABEL__)
	sox_file & read(sax_handler *sax = NULL) {
		parse_error = sox_parser(*this, sax).load(filename).parse_error;
#endif // defined(__YGGDRASIL_WITH_BABEL__)
		return *this;
	}
#if defined(__YGGDRASIL_WITH_BABEL__)
	sox_file & write(int encoding = babel::base_encoding::unknown) {
		sox_parser(*this).save(filename, encoding);
#else // defined(__YGGDRASIL_WITH_BABEL__)
	sox_file & write() {
		sox_parser(*this).save(filename);
#endif // defined(__YGGDRASIL_WITH_BABEL__)
		return *this;
	}

	sox_file & set_filename(const ygg_string &X_filename) {
		filename = X_filename;
		return *this;
	}
	ygg_string get_filename() {
		return filename;
	}
};
class sox_autofile :public sox_file {
  public:
	sox_autofile(const ygg_string &X_filename) :sox_file(X_filename) {
		read();
	}
	~sox_autofile() {
		write();
	}
};
class sox_stream :public ygg_node {
  public:

	ygg_error parse_error;

#if defined(__YGGDRASIL_WITH_BABEL__)
	sox_stream & read(std::istream &stream, sax_handler *sax = NULL, int encoding = babel::base_encoding::unknown) {
		parse_error = sox_parser(*this, sax).read(stream, encoding).parse_error;
#else // defined(__YGGDRASIL_WITH_BABEL__)
	sox_stream & read(std::istream &stream, sax_handler *sax = NULL) {
		parse_error = sox_parser(*this, sax).read(stream).parse_error;
#endif // defined(__YGGDRASIL_WITH_BABEL__)
		return *this;
	}
#if defined(__YGGDRASIL_WITH_BABEL__)
	sox_stream & write(std::ostream &stream, int encoding = babel::base_encoding::unknown) {
		parse_error = sox_parser(*this).write(stream, encoding).parse_error;
#else // defined(__YGGDRASIL_WITH_BABEL__)
	sox_stream & write(std::ostream &stream) {
		parse_error = sox_parser(*this).write(stream).parse_error;
#endif // defined(__YGGDRASIL_WITH_BABEL__)
		return *this;
	}
};


#if defined(__YGGDRASIL_WITH_BABEL__)
inline int get_encoding_from_label(const char *label) {
	using namespace babel::base_encoding;
	if (0 == stricmp(label, "shift-jis") ||
		0 == stricmp(label, "shift_jis") ||
		0 == stricmp(label, "x-sjis") ||
		0 == stricmp(label, "sjis") ||
		0 == stricmp(label, "shiftjis")) {
		return sjis;
	}
	if (0 == stricmp(label, "jis")) {
		return jis;
	}
	if (0 == stricmp(label, "iso-2022-jp")) {
		return iso2022jp;
	}
	if (0 == stricmp(label, "euc") ||
		0 == stricmp(label, "euc-jp") ||
		0 == stricmp(label, "x-euc")) {
		return euc;
	}
	if (0 == stricmp(label, "utf-8") ||
		0 == stricmp(label, "utf8")) {
		return utf8;
	}
	if (0 == stricmp(label, "utf-16") ||
		0 == stricmp(label, "utf16")) {
		return utf16;
	}
	if (0 == stricmp(label, "utf-16le") ||
		0 == stricmp(label, "utf16le")) {
		return utf16le;
	}
	if (0 == stricmp(label, "utf-16be") ||
		0 == stricmp(label, "utf16be")) {
		return utf16be;
	}
	if (0 == stricmp(label, "utf-32") ||
		0 == stricmp(label, "utf32")) {
		return utf32;
	}
	if (0 == stricmp(label, "utf-32le") ||
		0 == stricmp(label, "utf32le")) {
		return utf32le;
	}
	if (0 == stricmp(label, "utf-32be") ||
		0 == stricmp(label, "utf32be")) {
		return utf32be;
	}
	return unknown;
}
inline babel::bbl_translater<babel::bbl_binary, ygg_string> get_translater(const ygg_string &header) {

	//	ホントならこのまわりのテンプレート引数は省略できるハズなのにぃ(ノД｀) ... bcc のヽ(｀Д´)ノばかぁ！

	using namespace babel;
	using namespace babel::base_encoding;

	const ygg_string utf8_FEFF = "\xEF\xBB\xBF";
	const int analyze_encoding = analyze_base_encoding(header);
	bbl_translater<bbl_binary, ygg_string> translater = manual_translate_engine<bbl_binary, ygg_string>::order(analyze_encoding);

	switch(analyze_encoding) {

		case utf8:		if (0 != header.find(utf8_FEFF)) break;
		case utf16be:
		case utf16le:
		case utf32be:
		case utf32le:
		case jis:
			return translater;

	}

	const ygg_string encoding_label = ygg_root::parse_xml(translater(header))["/?xml/@encoding"].get_value().get_string();
	const int label_encoding = get_encoding_from_label(encoding_label.c_str());

	//	encoding_label を優先
	if (unknown != label_encoding && analyze_encoding != label_encoding) {
		return manual_translate_engine<bbl_binary, ygg_string>::order(label_encoding);
	} else {
		return translater.clear();
	}
}
#endif //	defined(__YGGDRASIL_WITH_BABEL__)

//
//	■XMLパーザクラス
//
class xml_parser :public ygg_parser {
	ygg_node &root;
	ygg_node hot_element;
	ygg_node last_node;
	ygg_node indent_node_list;
	int anchor_indent;
	ygg_string unparsed_buffer;

  public:

	xml_parser(ygg_node &X_root, sax_handler *X_sax = NULL)
											:ygg_parser(X_sax), root(X_root) {}
	xml_parser & set_sax_handler(sax_handler *X_sax) { set_sax_handler(X_sax); return *this; }
	xml_parser & read(std::istream &stream);
	xml_parser & write(std::ostream &stream);
	xml_parser & load(const ygg_string &X_filename);
	xml_parser & save(const ygg_string &X_filename);
	xml_parser & init_root();
	xml_parser & parse_line(const ygg_string &X_line);
	xml_parser & parse_string(const ygg_string &X_text);
	xml_parser & flush();
	xml_parser & end_stream();

};

inline xml_parser & xml_parser::read(std::istream &stream) {
	init_root();
	if (!stream) {
		raise_error(ygg_error_term::invalid_stream, "無効なストリームが指定されました。");
	} else {
#if defined(__YGGDRASIL_WITH_BABEL__)
		using namespace babel;

		ygg_string::value_type first_buffer[1024];
		ygg_string second_buffer, third_buffer;
		if (!stream.eof()) {

			stream.read(first_buffer, 1024);
			second_buffer.assign(first_buffer, stream.gcount());

			bbl_translater<bbl_binary, ygg_string> translater = get_translater(second_buffer);

			while(true) {
				translater << second_buffer;
				translater >> third_buffer;
				parse_string(third_buffer);
				if (stream.eof() || parse_error.is_error()) {
					break;
				}
				stream.read(first_buffer, 1024);
				second_buffer.assign(first_buffer, stream.gcount());
			}
		}
#else //	defined(__YGGDRASIL_WITH_BABEL__)
		ygg_string buffer;
		while(!stream.eof() && !parse_error.is_error()) {
			std::getline(stream, buffer);
			parse_line(buffer);
		}
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	}
	end_stream();
	return *this;
}
inline xml_parser & xml_parser::write(std::ostream &stream) {
#if defined(__YGGDRASIL_WITH_BABEL__)
	using namespace babel;
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	if (!stream) {
		raise_error(ygg_error_term::invalid_stream, "無効なストリームが指定されました。");
		return *this;
	}
#if defined(__YGGDRASIL_WITH_BABEL__)
	const ygg_string encoding_label = root["/?xml/@encoding"].get_value().get_string();
	const int label_encoding = get_encoding_from_label(encoding_label.c_str());
	stream << translate_to_binary(root.get_xml(), label_encoding);
#else //	defined(__YGGDRASIL_WITH_BABEL__)
	stream << root.get_xml();
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	return *this;
}


inline xml_parser & xml_parser::load(const ygg_string &X_filename) {
#if defined(__YGGDRASIL_WITH_BABEL__)
	std::ifstream file(X_filename.c_str(), std::ios::binary);
#else //	defined(__YGGDRASIL_WITH_BABEL__)
	std::ifstream file(X_filename.c_str());
#endif //	defined(__YGGDRASIL_WITH_BABEL__)
	if (!file) {
		raise_error(ygg_error_term::cannot_open_file, "ファイル " +X_filename +" を開けませんでした。");
		return *this;
	}

	return read(file);
}
inline xml_parser & xml_parser::save(const ygg_string &X_filename) {
#if defined(__YGGDRASIL_WITH_BABEL__)
	std::ofstream file(X_filename.c_str(), std::ios::binary);
#else //	defined(__YGGDRASIL_WITH_BABEL__)
	std::ofstream file(X_filename.c_str());
#endif //	defined(__YGGDRASIL_WITH_BABEL__)

	if (!file) {
		raise_error(ygg_error_term::cannot_open_file, "ファイル " +X_filename +" を開けませんでした。");
		return *this;
	}

	return write(file);
}

//
//
//
class xml_file :public ygg_node {
	ygg_string filename;

  public:

	ygg_error parse_error;

	xml_file(const ygg_string &X_filename) :filename(X_filename) {}
	~xml_file() {}

	xml_file & read(sax_handler *sax = NULL) {
		parse_error = xml_parser(*this, sax).load(filename).parse_error;
		return *this;
	}
	xml_file & write() {
		xml_parser(*this).save(filename);
		return *this;
	}

	xml_file & set_filename(const ygg_string &X_filename) {
		filename = X_filename;
		return *this;
	}
	ygg_string get_filename() {
		return filename;
	}
};
class xml_autofile :public xml_file {
  public:
	xml_autofile(const ygg_string &X_filename) :xml_file(X_filename) {
		read();
	}
	~xml_autofile() {
		write();
	}
};
class xml_stream :public ygg_node {
  public:

	ygg_error parse_error;

	xml_stream & read(std::istream &stream, sax_handler *sax = NULL) {
		parse_error = xml_parser(*this, sax).read(stream).parse_error;
		return *this;
	}
	xml_stream & write(std::ostream &stream) {
		parse_error = xml_parser(*this).write(stream).parse_error;
		return *this;
	}
};


inline ygg_node ygg_root::parse_sox(const ygg_string &sox, sax_handler *sax) {
	ygg_node root;
	sox_parser(root, sax).init_root().parse_string(sox).end_stream();
	return root;
}
inline ygg_node ygg_root::parse_xml(const ygg_string &xml, sax_handler *sax) {
	ygg_node root;
	xml_parser(root, sax).init_root().parse_string(xml).end_stream();
	return root;
}

}	//	namespace yggdrasil

#if defined(__BORLANDC__)
#	pragma warn .8022
#	pragma warn .8026
#	pragma warn .8027
#endif

#endif	//	__YGGDRASIL_YGG_H__

/******************************************************************************
	□■□■                  Wraith the Trickster                  □■□■
	■□■□ ～I'll go with heaven's advantage and fool's wisdom.～ ■□■□
******************************************************************************/

