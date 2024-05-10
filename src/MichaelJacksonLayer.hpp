#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/GameLevelManager.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

int songNum;
std::string songName;

class MJLayer : public CCLayer {
protected:
    bool init();
    void keyBackClicked() override;

public:
    static MJLayer* create();
    static MJLayer* scene();

    void onGoBack(CCObject*);
    void selectSong(CCObject*);
};

bool MJLayer::init() {
    if (!CCLayer::init())
        return false;

    FMODAudioEngine::sharedEngine()->playMusic("heehee.mp3"_spr, false, 1, 0);
    selectSong(nullptr);

    auto screen = CCDirector::sharedDirector();
    CCLabelBMFont* ripText = CCLabelBMFont::create("RIP MICHAEL JACKSON", "goldFont.fnt");
    CCLabelBMFont* dateText = CCLabelBMFont::create("1958 - 2009", "goldFont.fnt");
    CCSprite* MJ = CCSprite::create("mj.png"_spr);
    CCSprite* backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    CCSprite* bg = CCSprite::create("game_bg_13_001.png");
    bg->setColor({0,4,24});
    bg->setScale(2.5);
    CCMenu* backMenu = CCMenu::create();
    CCMenu* MJMenu = CCMenu::create();
    backSprite->setScale(1.1);
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSprite, this, menu_selector(MJLayer::onGoBack));
    MJ->setScale(1.2);
    CCMenuItemSpriteExtra* mjBtn = CCMenuItemSpriteExtra::create(MJ, this, menu_selector(MJLayer::selectSong));

    backMenu->setPosition({25.5, 290});


    ripText->setPosition(MJ->getPosition());
    ripText->setPositionY(MJ->getPositionY() + 140);

    dateText->setPosition(MJ->getPosition());
    dateText->setPositionY(MJ->getPositionY() - 140);

    MJMenu->setPosition({screen->getWinSize().width / 2, screen->getWinSize().height / 2});

    backMenu->addChild(backBtn);
    MJMenu->addChild(mjBtn);
    addChild(bg, -1);
    addChild(MJMenu);
    addChild(backMenu);
    addChild(dateText);
    addChild(ripText);

    return true;
}

void MJLayer::selectSong(CCObject*) {

    auto screen = CCDirector::sharedDirector();

    songNum = rand() % 4;

    FMODAudioEngine::sharedEngine()->stopAllActions();
    FMODAudioEngine::sharedEngine()->stopAllMusic();
    FMODAudioEngine::sharedEngine()->stopAllEffects();

    switch (songNum) {
        case 0: FMODAudioEngine::sharedEngine()->playMusic("Billie Jean.mp3"_spr, false, 1, 0); songName = "Billie Jean"; break;
        case 1: FMODAudioEngine::sharedEngine()->playMusic("Beat it.mp3"_spr, false, 1, 0); songName = "Beat It"; break;
        case 2: FMODAudioEngine::sharedEngine()->playMusic("Smooth Criminal.mp3"_spr, false, 1, 0); songName = "Smooth Criminal"; break;
        case 3: FMODAudioEngine::sharedEngine()->playMusic("Thriller.mp3"_spr, false, 1, 0); songName = "Thriller"; break;
    }
    log::debug("{} {}", songNum, songName);

    CCLabelBMFont* playingText = CCLabelBMFont::create(songName.c_str(), "chatFont.fnt");
    CCLabelBMFont* playingNow = CCLabelBMFont::create("Now Playing", "goldFont.fnt");
    CCScale9Sprite* playingBG = CCScale9Sprite::create("GJ_square01.png");
    playingBG->setContentSize(CCSize(160, 110));
    playingBG->setPosition({474, 164});

    playingNow->setPosition({playingBG->getPosition()});
    playingNow->setPositionY(playingNow->getPositionY() + 40);
    playingNow->setScale(0.9);

    playingText->setPosition({playingBG->getPosition()});
    playingText->setScale(1.2);
    playingText->setID("playingText");
    if (getChildByID("playingText")) removeChildByID("playingText"); 
    this->addChild(playingText, 2);
    this->addChild(playingBG);
    this->addChild(playingNow);
}

void MJLayer::keyBackClicked() {
    this->onGoBack(nullptr);
}

void MJLayer::onGoBack(CCObject*) {
    CreatorLayer* cl = CreatorLayer::create();
    CCScene* scene = cl->scene();
    auto transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);
}

MJLayer* MJLayer::create() {
    auto ret = new MJLayer;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MJLayer* MJLayer::scene() {
    auto layer = MJLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    auto transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);

    return layer;
}