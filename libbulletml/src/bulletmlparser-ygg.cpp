#include "bulletmlparser.h"
#include "bulletmlparser-ygg.h"
#include "bulletmlerror.h"
#include "bulletmltree.h"

#include "ygg/ygg.h"

#include <string>
#include <cstdio>

using namespace yggdrasil;

void BulletMLParserYggdrasil::on_error(yggdrasil::ygg_error* parse_error) {
	if (parse_error->is_error()) {
		char buf[2000];
		sprintf(buf,
				"BulletML sox error !!!\n"
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
		throw BulletMLError(buf);
	}
}

void BulletMLParserYggdrasil::start_document(yggdrasil::ygg_node) {

}

void BulletMLParserYggdrasil::end_document(yggdrasil::ygg_node&) {

}

void BulletMLParserYggdrasil::start_element(yggdrasil::ygg_node element) {
    BulletMLNode* xmlNode = addContent(trimString(element.get_name()));

	if (xmlNode->getName() != BulletMLNode::bulletml) {
		MyAttributes mattr;
		for (ygg_iterator ite = element["@*"].begin(); ite.is_not_end(); ++ite)
		{
			mattr.push_back(trimString(ite->get_name()));
			mattr.push_back(trimString(ite->get_value()));
		}
		if (!mattr.empty()) addAttribute(mattr, xmlNode);

		if (curNode_ != 0) curNode_->addChild(xmlNode);
	}
	curNode_ = xmlNode;
}

void BulletMLParserYggdrasil::end_element(yggdrasil::ygg_node element) {
	// @todo if (element.get_name() != curNode_
    if (curNode_ != 0) curNode_ = curNode_->getParent();
}

void BulletMLParserYggdrasil::catch_text(yggdrasil::ygg_node text) {
	curNode_->setValue(trimString(text.get_text()));
}

BulletMLParserYggdrasil::BulletMLParserYggdrasil(const std::string& filename)
    : xmlFile_(filename), curNode_(0)
{
	setName(filename);
}

BulletMLParserYggdrasil::~BulletMLParserYggdrasil() {}

void BulletMLParserYggdrasil::parse() {
	sox_file(xmlFile_).read(this);
}

std::string BulletMLParserYggdrasil::trimString(const std::string& str) {
	std::string ret(str);

	if (!ret.empty() && ret[0] == ' ')
		ret.erase(0, ret.find_first_not_of(' '));
	if (!ret.empty() && ret[str.length()-1] == ' ')
		ret.erase(ret.find_last_not_of(' ') + 1);

	return ret;
}

