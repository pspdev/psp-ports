#ifdef USE_XERCES

#include "bulletmlparser.h"
#include "bulletmlparser-xerces.h"
#include "bulletmlerror.h"
#include "bulletmltree.h"

#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <string>
#include <memory>
#include <sstream>

class BulletMLParserXercesSAXHandler : public HandlerBase {
public:
	BulletMLParserXercesSAXHandler(BulletMLParserXerces* parser);

	virtual void characters(const XMLCh* const chars,
											  const unsigned int length);
	virtual void startElement(const XMLCh* const name,
												AttributeList& attributes);
	virtual void endElement(const XMLCh* const name);

	virtual void error(const SAXParseException& e) {
		handleError(e);
	}

	virtual void fatalError(const SAXParseException& e) {
		handleError(e);
	}

	virtual void warning(const SAXParseException& e) {
		handleError(e);
	}

private:
	void handleError(const SAXParseException& e) {
		std::ostringstream oss;
		oss << "Error at file " << parser_->uc2string(e.getSystemId())
			<< ", line " << e.getLineNumber()
			<< ", char " << e.getColumnNumber()
			<< "\nMessage: " << parser_->uc2string(e.getMessage()) << endl;
		throw BulletMLError(oss.str());
	}

private:
	BulletMLParserXerces* parser_;
	BulletMLNode* curNode_;

};

BulletMLParserXercesSAXHandler::BulletMLParserXercesSAXHandler(
	BulletMLParserXerces* parser)
	: parser_(parser), curNode_(0)
{}

void BulletMLParserXercesSAXHandler::characters(const XMLCh* const chars,
												const unsigned int length)
{
	curNode_->setValue(parser_->uc2string(chars, length));
}

void BulletMLParserXercesSAXHandler::startElement(const XMLCh* const name,
												  AttributeList& attributes)
{
	BulletMLNode* node = parser_->addContent(parser_->uc2string(name));

	if (node->getName() == BulletMLNode::bulletml) {
		for (unsigned int i = 0; i < attributes.getLength(); i++) {
			if (parser_->uc2string(attributes.getName(i)) == "type" &&
				parser_->uc2string(attributes.getValue(i)) == "horizontal")
			{
				parser_->setHorizontal();
			}
		}
	}
	else {
		BulletMLParserXerces::MyAttributes mattr;
		for (unsigned int i = 0; i < attributes.getLength(); i++) {
			mattr.push_back(parser_->uc2string(attributes.getName(i)));
			mattr.push_back(parser_->uc2string(attributes.getValue(i)));
		}
		parser_->addAttribute(mattr, node);
	}

	if (curNode_ != 0) curNode_->addChild(node);
	curNode_ = node;
}

void BulletMLParserXercesSAXHandler::endElement(const XMLCh* const) {
	curNode_ = curNode_->getParent();
}

BulletMLParserXerces::BulletMLParserXerces(const std::string& filename)
    : xmlFile_(filename)
{
	setName(filename);
}

BulletMLParserXerces::~BulletMLParserXerces() {}

void BulletMLParserXerces::parse() {
	try {
		XMLPlatformUtils::Initialize();

		std::auto_ptr<SAXParser> parser(new SAXParser);

		parser->setValidationScheme(SAXParser::Val_Always);
		parser->setDoNamespaces(false);
		parser->setDoSchema(false);
		parser->setValidationSchemaFullChecking(false);

		BulletMLParserXercesSAXHandler handler(this);
		parser->setDocumentHandler(&handler);
		parser->setErrorHandler(&handler);

		parser->parse(xmlFile_.c_str());

		parser.reset(0);
	}
	catch (const XMLException& toCatch) {
		throw BulletMLError(uc2string(toCatch.getMessage()));
    }

    XMLPlatformUtils::Terminate();
}

#endif // USE_XERCES
