#include <Geode/modify/CreatorLayer.hpp>
#include "MichaelJacksonLayer.hpp"

using namespace geode::prelude;

class $modify(MJCallback, CreatorLayer) {
	struct Fields {
		CCSprite* mjBtnSprite;
		CCMenuItemSpriteExtra* mjButton;
	};
	bool init() {
		bool result = CreatorLayer::init();
		CCDirector* director = CCDirector::sharedDirector();
		CCSize size = director->getWinSize();
		CCMenu* doorButton = reinterpret_cast<CCMenu*>(getChildByID("bottom-right-menu"));
		FMODAudioEngine* fm = FMODAudioEngine::sharedEngine();

		CCMenu* menu = CCMenu::create();

		m_fields->mjBtnSprite = CCSprite::create("mj_door.png"_spr);

		m_fields->mjButton = CCMenuItemSpriteExtra::create(m_fields->mjBtnSprite, this, menu_selector(MJCallback::MichaelJackson));

		m_fields->mjButton->setPosition({doorButton->getPositionX(), doorButton->getPositionY() + 50});
		m_fields->mjButton->setID("mj-button");

		menu->setPosition({-5,60});
		
		menu->addChild(m_fields->mjButton);
		addChild(menu);

		return result;
	}

	void MichaelJackson(CCObject*) {
		FMODAudioEngine* fm = FMODAudioEngine::sharedEngine();
		fm->stopAllMusic();
		fm->stopAllActions();
		fm->stopAllEffects();
        MJLayer::scene();
    }
};