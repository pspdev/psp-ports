/// BulletML のパーサ・ツリー by Xerces
/**
 * Xerces による BulletML のパーサ
 */

#ifndef BULLETMLPARSER_XERCES_H_
#define BULLETMLPARSER_XERCES_H_

#ifdef USE_XERCES

#include <string>

class BulletMLParserXerces : public BulletMLParser {
public:
    explicit BulletMLParserXerces(const std::string& filename);
    virtual ~BulletMLParserXerces();

    virtual void parse();

private:
    std::string xmlFile_;

	friend class BulletMLParserXercesSAXHandler;
};

#endif // USE_XERCES

#endif // ! BULLETMLPARSER_XERCES_H_
