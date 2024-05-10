#pragma once

using namespace geode::prelude;

class MJLayer : public cocos2d::CCLayer {
private:
    int songNum = -1; // must init with same value for best results
    int currentSong = -1; // must init with same value for best results
    std::string songName;
    std::vector<std::string> songNames = {
        "Billie Jean",
        "Beat It",
        "Smooth Criminal",
        "Thriller"
    };
public:
    static MJLayer* create();
    static MJLayer* scene();
    
    bool init();
    void keyBackClicked();

    void onClose(CCObject* sender);
    void selectSong(CCObject* sender);
};

bool MJLayer::init() {
    if (!CCLayer::init()) { return false; }

    FMODAudioEngine::sharedEngine()->playMusic("heehee.mp3"_spr, false, 1, 0);
    selectSong(nullptr);

    CCDirector* screen = CCDirector::sharedDirector();
    
    CCSprite* bg = CCSprite::create("game_bg_13_001.png");
    bg->setColor({0,4,24});
    bg->setScale(2.5);
    bg->setID("background"_spr);
    
    CCSprite* mjSprite = CCSprite::create("mj.png"_spr);
    mjSprite->setScale(1.2);
    mjSprite->setID("michael-jackson-sprite"_spr);
    
    CCMenuItemSpriteExtra* mjBtn = CCMenuItemSpriteExtra::create(mjSprite, this, menu_selector(MJLayer::selectSong));
    mjBtn->setID("michael-jackson-button"_spr);
    
    CCMenu* mjMenu = CCMenu::create();
    mjMenu->setID("michael-jackson-menu"_spr);
    mjMenu->setPosition({screen->getWinSize().width / 2, screen->getWinSize().height / 2});
    mjMenu->addChild(mjBtn);
    
    CCSprite* backSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    backSprite->setScale(1.1);
    backSprite->setID("back-sprite"_spr);
    
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSprite, this, menu_selector(MJLayer::onClose));
    backBtn->setID("back-button"_spr);
    
    CCMenu* backMenu = CCMenu::create();
    backMenu->setID("back-menu"_spr);
    backMenu->setPosition({25.5, 290});
    backMenu->addChild(backBtn);
    
    CCLabelBMFont* ripText = CCLabelBMFont::create("RIP MICHAEL JACKSON", "goldFont.fnt");
    ripText->setPosition(mjSprite->getPosition());
    ripText->setPositionY(mjSprite->getPositionY() + 140);
    ripText->setID("rip-text"_spr);

    CCLabelBMFont* dateText = CCLabelBMFont::create("1958 - 2009", "goldFont.fnt");
    dateText->setPosition(mjSprite->getPosition());
    dateText->setPositionY(mjSprite->getPositionY() - 140);
    dateText->setID("date-text"_spr);
    
    addChild(bg, -1);
    addChild(mjMenu);
    addChild(backMenu);
    addChild(dateText);
    addChild(ripText);

    return true;
}

void MJLayer::selectSong(CCObject* sender) {
    while (songNum == currentSong) {
        songNum = rand() % 4;
    }
    currentSong = songNum;

    FMODAudioEngine* fmod = FMODAudioEngine::sharedEngine();
    
    fmod->stopAllActions();
    fmod->stopAllMusic();
    fmod->stopAllEffects();

    songName = songNames[songNum];
    
    std::string formattedSong = fmt::format(fmt::runtime("{}.mp3"_spr), songName);
    fmod->playMusic(formattedSong, false, 1, 0);
    
    log::debug("{} {}", songNum, songName);
    
    if (CCLabelBMFont* currSongLabel = typeinfo_cast<CCLabelBMFont*>(getChildByID("curr-song-label"_spr))) {
        currSongLabel->setString(songName.c_str());
    } else {
        CCLabelBMFont* playingText = CCLabelBMFont::create(songName.c_str(), "chatFont.fnt");
        CCLabelBMFont* playingNow = CCLabelBMFont::create("Now Playing", "goldFont.fnt");
        CCScale9Sprite* playingBG = CCScale9Sprite::create("GJ_square01.png");
        
        playingBG->setContentSize(CCSize(160, 110));
        playingBG->setPosition({474, 164});
        playingBG->setID("playing-bg"_spr);
    
        playingNow->setPosition({playingBG->getPosition()});
        playingNow->setPositionY(playingNow->getPositionY() + 40);
        playingNow->setScale(0.9);
        playingNow->setID("now-playing-label"_spr);
    
        playingText->setPosition({playingBG->getPosition()});
        playingText->setScale(1.2);
        playingText->setID("curr-song-label"_spr);
        
        addChild(playingText, 2);
        addChild(playingBG);
        addChild(playingNow);
    }
}

void MJLayer::keyBackClicked() {
    log::info("goback");
    onClose(nullptr);
}

void MJLayer::onClose(CCObject* sender) {
    CCScene* scene = CCScene::create();
    scene->addChild(CreatorLayer::create());
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
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
    MJLayer* layer = MJLayer::create();
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionFade* transition = CCTransitionFade::create(0.5f, scene);
    CCDirector::sharedDirector()->replaceScene(transition);

    return layer;
}