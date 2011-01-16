/******************************************************************************
	世界樹 -yggdrasil-
		世界樹チュートリアルソースファイル
											Coded by Wraith	in July 14, 2002.
******************************************************************************/

//　Tab幅を４文字に設定して表示させてください。

///////////////////////////////////////////////////////////////////////////////
//
//	■ ygg_test.cpp
//		http://tricklib.com/cxx/ex/yggdrasil/ygg_test.cpp
//
//	□ 関連ファイル
//		yggモジュールのヘッダファイル
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.h
//		yggモジュールの本体
//		http://tricklib.com/cxx/ex/yggdrasil/ygg.cpp
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

#include <stdio.h>

//#include <iostream>	//	for debug

//#define __USING_UTF8__

//#include "..\babel\babel.cpp"
#include "ygg.cpp"

#include <stdio.h>
#include <malloc.h>

//	VC対策
#if defined(_MSC_VER)
#	define std_for  if(0) ; else for
#else
#	define std_for  for
#endif

//	gcc対策(一時オブジェクトの左辺値化)
template<class T> T & vitalize(const T &X) { return (T&)X; }

using namespace yggdrasil;

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

class error_print: public sax_handler {
  public:
	void on_error(ygg_error *parse_error);
};
void error_print::on_error(ygg_error *parse_error) {
	if (parse_error->is_error()) {
		printf("On Error !!!\n"
			"\tlocation %d:%d\n"
			"\tcode     %02x:%02x:%02x:%02x\n"
			"\tmessage  %s\n",
			parse_error->get_line(),
			parse_error->get_row(),
			parse_error->get_genre(),
			parse_error->get_category(),
			parse_error->get_sequence(),
			parse_error->get_sequence_case(),
			parse_error->get_message().c_str());
	}
}

void test_enum() {
	sox_file root("test.sox");
	root.read(&vitalize(error_print()));

	ygg_node node_list = root["//node()"];
//	std_for(int i = 0; node_list[i].is_valid(); ++i) {
//		printf("path:%s\n", node_list[i].get_path().c_str());
//	}

//	std_for(ygg_iterator i = node_list.begin(); i != node_list.end(); ++i) {
//		printf("path:%s\n", i->get_path().c_str());
//	}

	std_for(ygg_iterator i = node_list.begin(); i.is_not_end(); ++i) {
		printf("path:%s\n", i->get_path().c_str());
	}
}

void test_enum_stream() {
	sox_stream root;
	std::ifstream fs("test.sox");
	root.read(fs, &vitalize(error_print()));

	ygg_node node_list = root["//node()"];
	std_for(int i = 0; node_list[i].is_valid(); ++i) {
		printf("path:%s\n", node_list[i].get_path().c_str());
	}
}

void test_path() {
	sox_file root("test.sox");

	root.read(&vitalize(error_print()));

	ygg_node length = root["/first/hige/@length"];
	std_for(int i = 0; length[i].is_valid(); ++i) {
		printf("%s\n", length[i].get_path().c_str());
	}

	ygg_node hige = root["/first/hige/p/b/c/d/..../.."];
	std_for(int i = 0; hige[i].is_valid(); ++i) {
		printf("%s\n", hige[i].get_path().c_str());
	}

	ygg_node p = root["/first/hige/p"];
	std_for(int i = 0; p[i].is_valid(); ++i) {
		printf("%s\n", p[i].get_path().c_str());
	}
}

void test_csv() {
	sox_file root("csv.sox");

	ygg_node item = root.read(&vitalize(error_print()))["/csv/item"];
	std_for(int i = 0; item[i].is_valid(); ++i) {
		printf("%s\n", ygg_utility::create_line(item[i]["node()"], ",").c_str());
	}
}

void test_text() {
	printf("%s\n", sox_file("test.sox").read(&vitalize(error_print())).get_text().c_str());
}

class sax_print: public error_print {
  public:
	void on_error(ygg_error* parse_error);
	void start_document(ygg_node root);
	void end_document(ygg_node &root);
	void start_element(ygg_node element);
	void end_element(ygg_node element);
	void catch_text(ygg_node text);
	void catch_comment(ygg_node comment);
};
void sax_print::on_error(ygg_error *parse_error) {
	printf("sax_print::on_error\n");
	error_print::on_error(parse_error);
}
void sax_print::start_document(ygg_node) {
	printf("sax_print::start_document\n");
}
void sax_print::end_document(ygg_node&) {
	printf("sax_print::end_document\n");
}
void sax_print::start_element(ygg_node element) {
	printf("sax_print::start_element[%s]\n", element.get_name().c_str());

//	ygg_node attributes = element["@*"];
//	std_for(int i = 0; attributes[i].is_valid(); ++i) {
//		printf("@%s=\"%s\"\n", attributes[i].get_name().c_str(), attributes[i].get_value().c_str());
//	}

	std_for(ygg_iterator i = element["@*"].begin(); i.is_not_end(); ++i) {
		printf("@%s=\"%s\"\n", i->get_name().c_str(), i->get_value().c_str());
	}
}
void sax_print::end_element(ygg_node element) {
	printf("sax_print::end_element[%s]\n", element.get_name().c_str());
}
void sax_print::catch_text(ygg_node text) {
	printf("sax_print::catch_text\n%s\n", text.get_text().c_str());
}
void sax_print::catch_comment(ygg_node) {
	printf("sax_print::catch_comment\n");
}

void test_handler() {
	sax_print sax;
	sox_file("test.sox").read(&sax);
}

void test_const() {
	const ygg_node node_list = sox_file("test.sox").read(&vitalize(error_print()))["//node()"];
	std_for(ygg_const_iterator i = node_list.begin(); i.is_not_end(); ++i) {
		printf("path:%s\n", i->get_path().c_str());
	}
}

void test_locacl_enum() {
	sox_file root("test.sox");
	root.read(&vitalize(error_print()));

	ygg_node node_list = root["/first/hige[0]/.//node()"];
	std_for(ygg_iterator i = node_list.begin(); i.is_not_end(); ++i) {
		printf("path:%s\n", i->get_path().c_str());
	}

}

void test_xml() {
	sax_print sax;
	xml_file root("test.xml");
	root.read(&sax);

	ygg_node node_list = root["//node()"];
	std_for(ygg_iterator i = node_list.begin(); i.is_not_end(); ++i) {
		printf("path:%s\n", i->get_path().c_str());
	}

	printf("xml:\n%s\n", root.get_xml().c_str());
}

void test_string() {
	ygg_node root = ygg_root::parse_sox(
		"sox>\n"
		"\traw>\n"
		"\t\ttext\n", &vitalize(error_print()));
	root["/sox"].adopt_sox("raw>\n\tadopt_sox\n");
	root["/sox"].adopt_xml("<raw>adopt_xml</raw>");
	printf("%s\n", root.get_xml().c_str());
}


void test_error() {
	error_catcher error_holder;
	ygg_root::parse_sox(
		"sox>\n"
//		"\t&raw>\n"			//	(02:00:02:01)
//		"\traw*>\n"			//	(02:00:02:02)
		"\t\traw>\n"		//	(02:01:01:00)
		"\traw>\n"
		"\t\thage=true\n"
//		"\t\thage=true\n"	//	(02:00:03:00)
//		"\t\t-hage=true\n"	//	(02:00:02:01)
//		"\t\thage?=true\n"	//	(02:00:02:02)
		"\t\ttext\n", &error_holder);
	ygg_error &error = error_holder.parse_error;
	if (error.is_error()) {
		printf("On Error !!!\n"
			"\tlocation %d:%d\n"
			"\tcode     %02x:%02x:%02x:%02x\n"
			"\tmessage  %s\n",
			error.get_line(),
			error.get_row(),
			error.get_genre(),
			error.get_category(),
			error.get_sequence(),
			error.get_sequence_case(),
			error.get_message().c_str());
	} else {
		printf("エラーはありませんでした。\n");
	}
}

int main() {
//	std::locale::global(std::locale("japanese"));

#if defined(__WITH_BABEL__)
	babel::init_babel();
#endif //	defined(__WITH_BABEL__)

	while(true) {
		int test;
		std::cout << std::endl;
		std::cout << "  *** ygg_test menu ***" << std::endl;
		std::cout << std::endl;
		std::cout << "    0 ... version information" << std::endl;
		std::cout << std::endl;
		std::cout << "    1 ... enum test" << std::endl;
		std::cout << "    2 ... enum test with stream" << std::endl;
		std::cout << "    3 ... path test" << std::endl;
		std::cout << "    4 ... csv test" << std::endl;
		std::cout << "    5 ... text test" << std::endl;
		std::cout << "    6 ... sax handle test" << std::endl;
		std::cout << "    7 ... const test" << std::endl;
		std::cout << "    8 ... local enum test" << std::endl;
		std::cout << "    9 ... xml test" << std::endl;
		std::cout << "   10 ... string test" << std::endl;
		std::cout << "   11 ... parse error test" << std::endl;
		std::cout << std::endl;
		std::cout << "    other number ... exit" << std::endl;
		std::cout << std::endl;
		std::cout << "  Please, input number." << std::endl;
		std::cout << std::endl;
		std::cin >> test;
		std::cout << std::endl;
		switch(test) {

			case 0:
				std::cout << "Version Information(SOX)" << std::endl;
				std::cout << ygg_node::parse_sox(ygg_term::yggdrasil_version).get_sox().c_str() << std::endl;
				std::cout << "Version Information(XML)" << std::endl;
				std::cout << ygg_node::parse_sox(ygg_term::yggdrasil_version).get_xml().c_str() << std::endl;
				break;

			case 1:
				std::cout << "Execute enum test!" << std::endl;
				test_enum();
				break;

			case 2:
				std::cout << "Execute enum test with stream!" << std::endl;
				test_enum_stream();
				break;

			case 3:
				std::cout << "Execute path test!" << std::endl;
				test_path();
				break;

			case 4:
				std::cout << "Execute csv test!" << std::endl;
				test_csv();
				break;

			case 5:
				std::cout << "Execute text test!" << std::endl;
				test_text();
				break;

			case 6:
				std::cout << "Execute sax handle test!" << std::endl;
				test_handler();
				break;

			case 7:
				std::cout << "Execute const test!" << std::endl;
				test_const();
				break;

			case 8:
				std::cout << "Execute local enum test!" << std::endl;
				test_locacl_enum();
				break;

			case 9:
				std::cout << "Execute xml test!" << std::endl;
				test_xml();
				break;

			case 10:
				std::cout << "Execute string test!" << std::endl;
				test_string();
				break;

			case 11:
				std::cout << "Execute parse error test!" << std::endl;
				test_error();
				break;

			default:
				std::cout << "Exit! Good-bye!" << std::endl;
				return 0;

		}
	}
}



/******************************************************************************
	□■□■                  Wraith the Trickster                  □■□■
	■□■□ ～I'll go with heaven's advantage and fool's wisdom.～ ■□■□
******************************************************************************/

