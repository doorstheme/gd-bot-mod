#include <Geode/Geode.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

bool g_botActive = false;
int g_currentFrame = 0;
int g_currentLevelID = 0;

// Кадры для Stereo Madness (ID: 1)
std::vector<int> stereoMadnessFrames = { 65, 120, 240, 310, 450, 520, 610, 750 }; 

// Кадры для Back on Track (ID: 2)
std::vector<int> backOnTrackFrames = { 50, 110, 190, 280, 390, 480, 590, 680 };

class $modify(MyPauseLayer, PauseLayer) {
    void customSetup() {
        PauseLayer::customSetup();

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto skullSprite = CCSprite::createWithSpriteFrameName("GJ_deleteBtn_001.png");
        
        auto botButton = CCMenuItemSpriteExtra::create(
            skullSprite,
            this,
            menu_selector(MyPauseLayer::onBotButtonClicked)
        );

        botButton->setPosition({ -150, 80 }); 

        if (auto menu = this->getChildByID("center-button-menu")) {
            menu->addChild(botButton);
            menu->updateLayout();
        } else {
            auto menu = CCMenu::create();
            menu->addChild(botButton);
            menu->setPosition(winSize.width / 2, winSize.height / 2);
            this->addChild(menu, 100);
        }
    }

    void onBotButtonClicked(CCObject* sender) {
        g_botActive = !g_botActive;
        g_currentFrame = 0;

        if (g_botActive) {
            Notification::create("Бот активирован!", CCNotificationIcon::Success)->show();
            this->onresume(sender);
        } else {
            Notification::create("Бот отключен", CCNotificationIcon::Error)->show();
        }
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2)) return false;
        
        g_currentLevelID = level->m_levelID.value();
        g_currentFrame = 0;
        g_botActive = false;
        
        return true;
    }

    void update(float dt) {
        PlayLayer::update(dt);

        if (g_botActive && this->m_player1) {
            g_currentFrame++;

            std::vector<int> activeFrames;
            if (g_currentLevelID == 1) {
                activeFrames = stereoMadnessFrames;
            } else if (g_currentLevelID == 2) {
                activeFrames = backOnTrackFrames;
            }

            for (int frame : activeFrames) {
                if (g_currentFrame == frame) {
                    auto player = this->m_player1;
                    if (player) {
                        player->pushButton(PlayerButton::Jump);
                        Loader::get()->queueInMainThread([player]() {
                            if (player) player->releaseButton(PlayerButton::Jump);
                        });
                    }
                }
            }
        }
    }
    
    void restartLevel() {
        PlayLayer::restartLevel();
        g_currentFrame = 0;
    }
};
