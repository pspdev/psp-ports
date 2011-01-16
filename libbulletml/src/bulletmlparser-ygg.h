/// BulletML のパーサ・ツリー by yggdrasil
/**
 * Yggdrasil による BulletML のパーサ
 */

#ifndef BULLETMLPARSER_YGG_H_
#define BULLETMLPARSER_YGG_H_

#include <string>

#include "bulletmlcommon.h"

#include "ygg/ygg.h"

class BulletMLParserYggdrasil
	: public BulletMLParser, public yggdrasil::sax_handler
{
public:
	void on_error(yggdrasil::ygg_error* parse_error);
	void start_document(yggdrasil::ygg_node root);
	void end_document(yggdrasil::ygg_node &root);
	void start_element(yggdrasil::ygg_node element);
	void end_element(yggdrasil::ygg_node element);
	void catch_text(yggdrasil::ygg_node text);

public:
    DECLSPEC explicit BulletMLParserYggdrasil(const std::string& filename);
    DECLSPEC virtual ~BulletMLParserYggdrasil();

    DECLSPEC virtual void parse();

protected:
	std::string trimString(const std::string& str);

private:
    std::string xmlFile_;
    class BulletMLNode* curNode_;
};

#endif // ! BULLETMLPARSER_YGG_H_
