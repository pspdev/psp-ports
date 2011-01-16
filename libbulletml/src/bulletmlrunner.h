/// BulletML を実行する
/**
 * 使い方：
 * BulletMLRunner を継承していくつかの純粋仮想関数を実装する。
 */

#ifndef BULLETRUNNER_H_
#define BULLETRUNNER_H_

#include "bulletmltree.h"
#include "bulletmlcommon.h"

#include <vector>
#include <boost/smart_ptr.hpp>

class BulletMLParser;
class BulletMLNode;
class BulletMLRunnerImpl;

typedef std::vector<float> BulletMLParameter;

/// BulletMLRunner が状態伝達に使用するクラス
class BulletMLState {
public:
	DECLSPEC BulletMLState(BulletMLParser* bulletml,
						   const std::vector<BulletMLNode*>& node,
						   boost::shared_ptr<BulletMLParameter> para)
		: bulletml_(bulletml), node_(node.begin(), node.end()), para_(para) {}

	DECLSPEC BulletMLParser* getBulletML() { return bulletml_; }
	DECLSPEC const std::vector<BulletMLNode*>& getNode() const { return node_; }
	DECLSPEC boost::shared_ptr<BulletMLParameter> getParameter() { return para_; }

private:
	BulletMLParser* bulletml_;
	std::vector<BulletMLNode*> node_;
	boost::shared_ptr<BulletMLParameter> para_;

};

/// BulletML を実行するクラス
/**
 * <pre>
 * 使い方。
 *  1. これを継承して、Bullet を実際に動かせるクラスを作る。
 *  2. 純粋仮想関数を全て実装する。
 *  3. 必要なら、changeDirection などのために必要な仮想関数を実装する。
 *  4. このクラスのオブジェクトに対して、com_->run(); などとする。
 * </pre>
 */

class BulletMLRunner {
public:
	DECLSPEC explicit BulletMLRunner(BulletMLParser* bulletml);
    DECLSPEC explicit BulletMLRunner(BulletMLState* state);
    DECLSPEC virtual ~BulletMLRunner();

	/// 実行する
    DECLSPEC void run();

public:
	/// 実行が終了しているかどうか
	DECLSPEC bool isEnd() const;

public:
	// ----- 絶対実装しなければならない関数群の始まり -----
	//@{
	/// この弾の角度を求める
	/**
	 * @return 角度を度単位で、上方向 0 で時計周りで返す
	 */
	DECLSPEC virtual float getBulletDirection() =0;
	/// この弾から自機を狙う角度を求める
	/**
	 * @return 角度を度単位で、上方向 0 で時計周りで返す
	 */
	DECLSPEC virtual float getAimDirection() =0;
	/// この弾の速度を求める
	DECLSPEC virtual float getBulletSpeed() =0;
	/// デフォルトの速度を求める
	DECLSPEC virtual float getDefaultSpeed() =0;
	/// ランクを求める
	/**
	 * @return 0 から 1 までの実数
	 */
	DECLSPEC virtual float getRank() =0;
	/// action を持たない弾を作る
	DECLSPEC virtual void createSimpleBullet(float direction, float speed) =0;
	/// action を持つ弾を作る
	/**
	 * @param state
	 * 新しい弾の BulletMLRunner のコンストラクタに渡すこと。
	 * もし渡さないのであれば、delete で解放しなければならない。
	 */
	DECLSPEC virtual void createBullet(BulletMLState* state,
									   float direction, float speed) =0;
	/// 弾の基準となるターンの値を返す、通常はフレーム数
	/**
	 * @return
	 * ゲームごとの基準でオーダーは変更して構いませんが、
	 * 負数は許されません。
	 * xml データ上で、wait と term のオーダーが揃っていれば問題ありません。
	 */
	DECLSPEC virtual int getTurn() =0;
	/// 死ぬ
	DECLSPEC virtual void doVanish() =0;
	//@}
	// ----- 絶対実装しなければならない関数群の終わり -----

	// ----- 必要があれば実装する関数群の始まり -----
   	//@{
	/// 弾の方向を指定した方向に変更する
	DECLSPEC virtual void doChangeDirection(float) {}
	/// 弾の速度を指定した値に変更する
	DECLSPEC virtual void doChangeSpeed(float) {}
	/// accel に対するインターフェイス
	/**
	 * @todo
	 * horizontal, vertical の type は未実装です。
	 * どれも absolute になってしまいます。
	 */
	DECLSPEC virtual void doAccelX(float) {}
	/// accel に対するインターフェイス
	/**
	 * @todo
	 * horizontal, vertical の type は未実装です。
	 * どれも absolute になってしまいます。
	 */
	DECLSPEC virtual void doAccelY(float) {}
	/// 弾の速さの X 方向成分を返します
	/**
	 * accel を使う場合はオーバーライドして下さい
 	 */
	DECLSPEC virtual float getBulletSpeedX() { return 0; }
	/// 弾の速さの Y 方向成分を返します
	/**
	 * accel を使う場合はオーバーライドして下さい
 	 */
	DECLSPEC virtual float getBulletSpeedY() { return 0; }
    //@}
	// ----- 必要があれば実装する関数群の終わり -----

	/// 乱数を返す
	/**
	 * 見てのとおり、デフォルトでは std::rand が用いられます。
	 */
	DECLSPEC virtual float getRand() { return (float)rand() / RAND_MAX; }

private:
	/// BulletMLRunnerImpl をオーバーライドする場合、これもオーバーライドする
	DECLSPEC virtual BulletMLRunnerImpl* makeImpl(BulletMLState* state);

protected:
	std::vector<BulletMLRunnerImpl*> impl_;

};

#endif // ! BULLETRUNNER_H_

