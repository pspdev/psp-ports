/// BulletML のパーサ
/**
 * c++ 用 RELAX が無かったのでまあ自分で作ることに
 */

#ifndef BULLETMLPARSER_H_
#define BULLETMLPARSER_H_

#include "bulletmltree.h"
#include "bulletmlcommon.h"

#include <string>
#include <vector>

#include <stdio.h>

class BulletMLParser {
protected:
    typedef std::vector<std::string> MyAttributes;
    typedef MyAttributes::const_iterator MyAttributeIte;

public:
    DECLSPEC BulletMLParser();
    DECLSPEC virtual ~BulletMLParser();

public:
	DECLSPEC void build();
    DECLSPEC virtual void parse() =0;

public:
    /**
     * BulletML は仕様上ツリー構造の根っこを取れる必要はなく
     * ラベルからこれらのみ取れれば良い
     */
    //@{
    DECLSPEC BulletMLNode* getBulletRef(int id);
    DECLSPEC BulletMLNode* getActionRef(int id);
    DECLSPEC BulletMLNode* getFireRef(int id);
    //@}

	DECLSPEC const std::vector<BulletMLNode*>& getTopActions() const {
		return topActions_;
	}

	DECLSPEC void setHorizontal() { isHorizontal_ = true; }
    DECLSPEC bool isHorizontal() const { return isHorizontal_; }

protected:
    BulletMLNode* addContent(const std::string& name);
    void addAttribute(const MyAttributes& attr, BulletMLNode* elem);

protected:
    /// これはgccのバージョン間の互換のためなのだが
    template <class Char_>
    std::string uc2string(Char_* src, size_t len = std::string::npos);

protected:
    BulletMLNode* bulletml_;

	std::vector<BulletMLNode*> topActions_;

    typedef std::vector<BulletMLNode*> MyMap;
	typedef MyMap BulletMap;
    typedef MyMap ActionMap;
    typedef MyMap FireMap;
    BulletMap bulletMap_;
    ActionMap actionMap_;
    FireMap fireMap_;

    bool isHorizontal_;

protected:
	/// 一時的な導入
	void setName(const std::string& name) { name_ = name; }
	std::string name_;
public:
	DECLSPEC const std::string& getName() const { return name_; }

};

template <class Char_>
std::string BulletMLParser::uc2string(Char_* src, size_t len) {
    std::string dst;
    size_t i = 0;
    while (i != len && *src != '\0') {
		dst += *src;
		src++;
		i++;
    }
    return dst;
}

#endif // ! BULLETMLPARSER_H_
