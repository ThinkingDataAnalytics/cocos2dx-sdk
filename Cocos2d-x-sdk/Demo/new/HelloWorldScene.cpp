/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "./ThinkingAnalytics/Common/ThinkingAnalyticsAPI.h"

#include <thread>

using namespace thinkingdata;
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;


static string platformTag;


const static string APPID_1 = "1b1c1fef65e3482bad5c9d0e6a823356";
const static string APPID_2 = "22e445595b0f42bd8c5fe35bc44b88d6";
int num = 100000000;
int num_1 = 200000000;
double double_num = 12.345;


string getPlatformTag() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    platformTag = "android";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    platformTag = "ios";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    platformTag = "win";
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    platformTag = "mac";
#else
    platformTag = "unknow";
#endif
    return platformTag;
}

TDJSONObject superDylyPropery() {

    vector<string> arr;
    arr.push_back("XX_2");
    
    TDJSONObject obj;
    obj.setString("level", "200");
    obj.setNumber("double", 2.22);
    obj.setString("string", "wangdaji2");
    obj.setBool("bool", true);
    obj.setList("list", arr);
    obj.setDateTime("time","2022-02-02 10:52:52.290");
    return obj;
}

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}


void showInstanceMessage() {
    
    
    string path11 = cocos2d::UserDefault::getInstance()->getXMLFilePath();
    
    CCLOG("@@@@@@@@@@ path @@@@@@@@@@");
    CCLOG("path:%s", path11.c_str());
    
    CCLOG("");
    CCLOG("");
    

    ThinkingAnalyticsAPI::enableTrackLog(true);
    
    Config config1(APPID_1,"http://receiver.ta.thinkingdata.cn");
//    config1.setModel(TD_DEBUG);
//    config1.setName("1");
    ThinkingAnalyticsAPI::init(config1);
    string distinctID1 = ThinkingAnalyticsAPI::getDistinctId();
    string deviceID1 = ThinkingAnalyticsAPI::getDeviceId();
    
    Config config2(APPID_2,"http://receiver.ta.thinkingdata.cn");
    ThinkingAnalyticsAPI::init(config2);
//    config2.setModel(TD_DEBUG);
//    config2.setName("2");
    string distinctID2 = ThinkingAnalyticsAPI::getDistinctId(APPID_2);
    string deviceID2 = ThinkingAnalyticsAPI::getDeviceId();
   
    
    
    CCLOG("@@@@@@@@@@ ins1 @@@@@@@@@@");
    CCLOG("appID:%s", APPID_1.c_str());
    CCLOG("deviceID:%s", deviceID1.c_str());
    CCLOG("distinctID:%s", distinctID1.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    string accountId1 = cocos2d::UserDefault::getInstance()->getStringForKey((char *)("td"+APPID_1+"accountId").data());
    string disEnable1 = cocos2d::UserDefault::getInstance()->getStringForKey((char *)("td"+APPID_1+"disenable").data());
    string optout1 = cocos2d::UserDefault::getInstance()->getStringForKey((char *)("td"+APPID_1+"optout").data());
    CCLOG("accountID:%s", accountId1.c_str());
    CCLOG("enable:%i", disEnable1 != "1");
    CCLOG("optout:%i", optout1 == "1");
#endif
    CCLOG("superProperty:%s", TDJSONObject::toJson(ThinkingAnalyticsAPI::getSuperProperties(APPID_1)).c_str());
    CCLOG("presentProperty:%s", TDJSONObject::toJson(*ThinkingAnalyticsAPI::getPresetProperties(APPID_1)->toEventPresetProperties()).c_str());
    
    
    CCLOG("");
    CCLOG("");
    
    CCLOG("@@@@@@@@@@ ins2 @@@@@@@@@@");
    CCLOG("appID:%s", APPID_2.c_str());
    CCLOG("deviceID:%s", deviceID2.c_str());
    CCLOG("distinctID:%s", distinctID2.c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    string accountId2 = cocos2d::UserDefault::getInstance()->getStringForKey((char *)("td"+APPID_2+"accountId").data());
    string disEnable2 = cocos2d::UserDefault::getInstance()->getStringForKey((char *)("td"+APPID_2+"disenable").data());
    string optout2 = cocos2d::UserDefault::getInstance()->getStringForKey((char *)("td"+APPID_2+"optout").data());
    CCLOG("accountID:%s", accountId2.c_str());
    CCLOG("enable:%i", disEnable2 != "1");
    CCLOG("optout:%i", optout2 == "1");
#endif
    CCLOG("superProperty:%s", TDJSONObject::toJson(ThinkingAnalyticsAPI::getSuperProperties(APPID_2)).c_str());
    CCLOG("presentProperty:%s", TDJSONObject::toJson(*ThinkingAnalyticsAPI::getPresetProperties(APPID_2)->toEventPresetProperties()).c_str());
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    if ( !Scene::init() ) {
        return false;
    }
    
    getPlatformTag();
    showInstanceMessage();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static string lightToken =  ThinkingAnalyticsAPI::createLightInstance();
#endif
    
    auto leftMargin = origin.x + 50;
    auto topMargin = 25;
    auto commonFontSize = 10;
    auto commonMargin = 10;
    auto userLabel = Label::createWithSystemFont("user identiyf", "Arial", 12);
    userLabel->setTextColor(Color4B::ORANGE);
    userLabel->setPosition(Vec2((visibleSize.width -userLabel->getContentSize().width)/2.0 ,
                                origin.y + visibleSize.height - userLabel->getContentSize().height));
    this->addChild(userLabel);


    auto userButton = Button::create();
    userButton->setTitleText("accountID");
    userButton->setTitleFontSize(commonFontSize);
    userButton->setPosition(Vec2(leftMargin,
                                 userLabel->getPositionY() - topMargin));
    userButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::login(("accountid_"+platformTag+"_1").c_str());
            ThinkingAnalyticsAPI::login(("accountid_"+platformTag+"_1").c_str(), APPID_1);
            ThinkingAnalyticsAPI::login(("accountid_"+platformTag+"_2").c_str(), APPID_2);
        }
    });
    this->addChild(userButton);


    auto userButton1 = Button::create();
    userButton1->setTitleFontSize(commonFontSize);
    userButton1->setTitleText("distinctID");
    userButton1->setPosition(Vec2(10 + userButton->getPositionX() + userButton->getContentSize().width,
                                  userButton->getPositionY()));
    userButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::identify(("distinct_"+platformTag+"_1").c_str());
            ThinkingAnalyticsAPI::identify(("distinct_"+platformTag+"_1").c_str(), APPID_1);
            ThinkingAnalyticsAPI::identify(("distinct_"+platformTag+"_2").c_str(), APPID_2);
        }
    });
    this->addChild(userButton1);

    auto userButton2 = Button::create();
    userButton2->setTitleFontSize(commonFontSize);
    userButton2->setTitleText("clearAccID");
    userButton2->setPosition(Vec2(10 + userButton1->getPositionX() + userButton1->getContentSize().width,
                                  userButton->getPositionY()));
    userButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::logout();
            ThinkingAnalyticsAPI::logout(APPID_1);
            ThinkingAnalyticsAPI::logout(APPID_2);
        }
    });
    this->addChild(userButton2);
    
    auto userButton22 = Button::create();
    userButton22->setTitleFontSize(commonFontSize);
    userButton22->setTitleText("disenable");
    userButton22->setPosition(Vec2(10 + userButton2->getPositionX() + userButton22->getContentSize().width,
                                  userButton->getPositionY()));
    userButton22->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::enableTracking(false);
            ThinkingAnalyticsAPI::enableTracking(false, APPID_1);
            ThinkingAnalyticsAPI::enableTracking(false, APPID_2);
        }
    });
    this->addChild(userButton22);
    
    auto userButton33 = Button::create();
    userButton33->setTitleFontSize(commonFontSize);
    userButton33->setTitleText("enable");
    userButton33->setPosition(Vec2(10 + userButton22->getPositionX() + userButton33->getContentSize().width,
                                  userButton->getPositionY()));
    userButton33->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::enableTracking(true);
            ThinkingAnalyticsAPI::enableTracking(true, APPID_1);
            ThinkingAnalyticsAPI::enableTracking(true, APPID_2);
        }
    });
    this->addChild(userButton33);
    
    auto userButton44 = Button::create();
    userButton44->setTitleFontSize(commonFontSize);
    userButton44->setTitleText("optOut");
    userButton44->setPosition(Vec2(10 + userButton33->getPositionX() + userButton44->getContentSize().width,
                                  userButton->getPositionY()));
    userButton44->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::optOutTracking();
            ThinkingAnalyticsAPI::optOutTracking(APPID_1);
            ThinkingAnalyticsAPI::optOutTracking(APPID_2);
        }
    });
    this->addChild(userButton44);
    
    auto userButton55 = Button::create();
    userButton55->setTitleFontSize(commonFontSize);
    userButton55->setTitleText("optIn");
    userButton55->setPosition(Vec2(10 + userButton44->getPositionX() + userButton55->getContentSize().width,
                                  userButton->getPositionY()));
    userButton55->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::optInTracking();
            ThinkingAnalyticsAPI::optInTracking(APPID_1);
            ThinkingAnalyticsAPI::optInTracking(APPID_2);
        }
    });
    this->addChild(userButton55);


    auto eventLabel = Label::createWithSystemFont("eventTrack", "Arial", 12);
    eventLabel->setTextColor(Color4B::ORANGE);
    eventLabel->setPosition(Vec2(userLabel->getPositionX(),
                                 userButton->getPositionY() - topMargin));
    this->addChild(eventLabel);

    auto eventButton = Button::create();
    eventButton->setTitleText("track");
    eventButton->setTitleFontSize(commonFontSize);
    eventButton->setPosition(Vec2(leftMargin,
                                  eventLabel->getPositionY() - topMargin));
    eventButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
        
            ThinkingAnalyticsAPI::track("track");
            ThinkingAnalyticsAPI::track("track", APPID_1);
            ThinkingAnalyticsAPI::track("track", APPID_2);
            
            ThinkingAnalyticsAPI::timeEvent("timeEvent");
            ThinkingAnalyticsAPI::timeEvent("timeEvent", APPID_1);
            ThinkingAnalyticsAPI::timeEvent("timeEvent", APPID_2);
            /**
            for (int i = 0; i < 100; i++) {
//                std::thread threadObj(test1);
//                threadObj.join();
//            }

            */
            
//            ThinkingAnalyticsAPI::optOutTrackingAndDeleteUser();
//            ThinkingAnalyticsAPI::optOutTracking();
//            ThinkingAnalyticsAPI::enableTracking(false);
            
            
//            TDJSONObject obj;
//            obj.setString("334", "100");
//            obj.setNumber("444",2.34);
//            ThinkingAnalyticsAPI::setSuperProperties(obj);
//            ThinkingAnalyticsAPI::track("track");
           
        }
    });
    this->addChild(eventButton);


    auto eventButton1 = Button::create();
    eventButton1->setTitleText("trackAndProperty");
    eventButton1->setTitleFontSize(commonFontSize);
    eventButton1->setPosition(Vec2(30 + userButton->getPositionX() + userButton->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            vector<string> arr;
            arr.push_back("XX_1");
            
            TDJSONObject obj;
            obj.setString("level", "100");
            obj.setNumber("double", 1.11);
            obj.setString("string", "wangdaji");
            obj.setBool("bool", false);
            obj.setList("list", arr);
            obj.setDateTime("time","2021-01-01 10:52:52.290");
            
            ThinkingAnalyticsAPI::track("track_property",obj);
            ThinkingAnalyticsAPI::track("track_property",obj, APPID_1);
            ThinkingAnalyticsAPI::track("track_property",obj, APPID_2);
        }
    });
    this->addChild(eventButton1);

    auto eventButton2 = Button::create();
    eventButton2->setTitleFontSize(commonFontSize);
    eventButton2->setTitleText("timeEvent");
    eventButton2->setPosition(Vec2(commonMargin + eventButton1->getPositionX() + eventButton1->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::track("timeEvent");
            ThinkingAnalyticsAPI::track("timeEvent", APPID_1);
            ThinkingAnalyticsAPI::track("timeEvent", APPID_2);
        }
    });
    this->addChild(eventButton2);

    auto eventButton3 = Button::create();
    eventButton3->setTitleText("firstEvent");
    eventButton3->setTitleFontSize(commonFontSize);
    eventButton3->setPosition(Vec2(commonMargin + eventButton2->getPositionX() + eventButton2->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            TDFirstEvent *firstEvent2 = new TDFirstEvent("firstEvent",jsonObject1);
            ThinkingAnalyticsAPI::track(firstEvent2);
            ThinkingAnalyticsAPI::track(firstEvent2,APPID_1);
            ThinkingAnalyticsAPI::track(firstEvent2,APPID_2);
            
            firstEvent2->setFirstCheckId("firstCheckID");
            ThinkingAnalyticsAPI::track(firstEvent2);
            ThinkingAnalyticsAPI::track(firstEvent2,APPID_1);
            ThinkingAnalyticsAPI::track(firstEvent2,APPID_2);
        }
    });
    this->addChild(eventButton3);

    auto eventButton4 = Button::create();
    eventButton4->setTitleText("updateEvent");
    eventButton4->setTitleFontSize(commonFontSize);
    eventButton4->setPosition(Vec2(commonMargin + eventButton3->getPositionX() + eventButton3->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton4->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            jsonObject1.setNumber("status", 3);
            jsonObject1.setNumber("price", 5);
            TDUpdatableEvent *updatableEvent1 = new TDUpdatableEvent("updateEvent",jsonObject1,"12345");
            ThinkingAnalyticsAPI::track(updatableEvent1);
            ThinkingAnalyticsAPI::track(updatableEvent1,APPID_1);
            ThinkingAnalyticsAPI::track(updatableEvent1,APPID_2);
        }
    });
    this->addChild(eventButton4);



    auto eventButton5 = Button::create();
    eventButton5->setTitleText("overWriteEvent");
    eventButton5->setTitleFontSize(commonFontSize);
    eventButton5->setPosition(Vec2(commonMargin + eventButton4->getPositionX() + eventButton4->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton5->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            TDOverWritableEvent *overWritableEvent1 = new TDOverWritableEvent("overWriteEvent",jsonObject1,"12345");
            ThinkingAnalyticsAPI::track(overWritableEvent1);
            ThinkingAnalyticsAPI::track(overWritableEvent1,APPID_1);
            ThinkingAnalyticsAPI::track(overWritableEvent1,APPID_2);
        }
    });
    this->addChild(eventButton5);



    auto userPropertyLabel = Label::createWithSystemFont("userEvent", "Arial", 12);
    userPropertyLabel->setPosition(Vec2(userLabel->getPositionX(),
                                        eventButton->getPositionY() - topMargin));
    this->addChild(userPropertyLabel);
    userPropertyLabel->setTextColor(Color4B::ORANGE);
    auto userPropertyButton = Button::create();
    userPropertyButton->setTitleFontSize(10);
    userPropertyButton->setTitleText("UserSet");
    userPropertyButton->setPosition(Vec2(leftMargin,
                                         userPropertyLabel->getPositionY() - topMargin));
    userPropertyButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            userProperties1.setString("userSet_key", "userSet_value");
            ThinkingAnalyticsAPI::user_set(userProperties1);
            ThinkingAnalyticsAPI::user_set(userProperties1,APPID_1);
            ThinkingAnalyticsAPI::user_set(userProperties1,APPID_2);
        }
    });
    this->addChild(userPropertyButton);


    auto userPropertyButton1 = Button::create();
    userPropertyButton1->setTitleFontSize(10);
    userPropertyButton1->setTitleText("UserSetOnce");
    userPropertyButton1->setPosition(Vec2(25 + userPropertyButton->getPositionX() + userPropertyButton->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            userProperties1.setString("userSetOnce_key", "userSetOnce_value");
            userProperties1.setNumber("userSetOnce_int",1);
            ThinkingAnalyticsAPI::user_setOnce(userProperties1);
            ThinkingAnalyticsAPI::user_setOnce(userProperties1, APPID_1);
            ThinkingAnalyticsAPI::user_setOnce(userProperties1, APPID_2);
        }
    });
    this->addChild(userPropertyButton1);


    auto userPropertyButton2 = Button::create();
    userPropertyButton2->setTitleFontSize(10);
    userPropertyButton2->setTitleText("UserAdd");
    userPropertyButton2->setPosition(Vec2(25 + userPropertyButton1->getPositionX() + userPropertyButton1->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            userProperties1.setString("userAdd_key","userAdd_value");
            userProperties1.setNumber("userAdd_int",1);
            ThinkingAnalyticsAPI::user_add(userProperties1);
            ThinkingAnalyticsAPI::user_add(userProperties1, APPID_1);
            ThinkingAnalyticsAPI::user_add(userProperties1, APPID_2);
        }
    });
    this->addChild(userPropertyButton2);


    auto userPropertyButton3 = Button::create();
    userPropertyButton3->setTitleFontSize(10);
    userPropertyButton3->setTitleText("UserUnset");
    userPropertyButton3->setPosition(Vec2(25 + userPropertyButton2->getPositionX() + userPropertyButton2->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::user_unset("userUnset_key");
            ThinkingAnalyticsAPI::user_unset("userUnset_key",APPID_1);
            ThinkingAnalyticsAPI::user_unset("userUnset_key",APPID_2);
        }
    });
    this->addChild(userPropertyButton3);


    auto userPropertyButton4 = Button::create();
    userPropertyButton4->setTitleFontSize(10);
    userPropertyButton4->setTitleText("UserDelete");
    userPropertyButton4->setPosition(Vec2(25 + userPropertyButton3->getPositionX() + userPropertyButton3->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton4->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::user_delete();
            ThinkingAnalyticsAPI::user_delete(APPID_1);
            ThinkingAnalyticsAPI::user_delete(APPID_2);
        }
    });
    this->addChild(userPropertyButton4);

    auto userPropertyButton5 = Button::create();
    userPropertyButton5->setTitleFontSize(10);
    userPropertyButton5->setTitleText("UserAppend");
    userPropertyButton5->setPosition(Vec2(25 + userPropertyButton4->getPositionX() + userPropertyButton4->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton5->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject userProperties1;
            vector<string> listValue1;
            listValue1.push_back("XX");
            userProperties1.setList("userAppend_key",listValue1);
            ThinkingAnalyticsAPI::user_append(userProperties1,APPID_1);
        }
    });
    this->addChild(userPropertyButton5);




    auto publicPropertyLabel = Label::createWithSystemFont("commonProperty", "Arial", 12);
    publicPropertyLabel->setTextColor(Color4B::ORANGE);
    publicPropertyLabel->setPosition(Vec2(userLabel->getPositionX(),
                                          userPropertyButton->getPositionY() - topMargin));
    this->addChild(publicPropertyLabel);

    auto publicPropertyButton = Button::create();
    publicPropertyButton->setTitleFontSize(10);
    publicPropertyButton->setTitleText("setStaicProperty");
    publicPropertyButton->setPosition(Vec2(leftMargin,
                                           publicPropertyLabel->getPositionY() - topMargin));
    publicPropertyButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            vector<string> arr;
            arr.push_back("XX_3");
            
            TDJSONObject obj;
            obj.setString("super_key", "super_value");
            obj.setString("level", "300");
            obj.setNumber("double", 3.33);
            obj.setString("string", "wangdaji3");
            obj.setBool("bool", false);
            obj.setList("list", arr);
            obj.setDateTime("time","3033-03-03 10:52:52.290");
            obj.setDateTime("str1","3033-03-03 10:52:52.290;][]{}]");
            
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            
            TDJSONObject innerObj;
            vector<string> innerArr;
            innerArr.push_back("inner_XX_3");
            
            innerObj.setString("inner_level", "300");
            innerObj.setNumber("inner_double", 3.33);
            innerObj.setString("inner_string", "wangdaji3");
            innerObj.setBool("inner_bool", false);
            innerObj.setList("inner_list", innerArr);
            innerObj.setDateTime("inner_time","1993-03-03 10:52:52.290");
            
            obj.setJsonObject("obj", innerObj);
            
            
            TDJSONObject obj1;
            obj1.setString("obj1_level", "100");
            obj1.setNumber("obj1_double", 2.34);
            obj1.setString("obj1_inner_string", "wangdaji");
            obj1.setBool("obj1_bool", false);
            obj1.setList("obj1_list", arr);
            obj1.setDateTime("obj1_time","1993-01-02 10:52:52.290");
            
            TDJSONObject obj2;
            obj2.setString("obj2_level", "100");
            obj2.setNumber("obj3_double", 2.34);
            obj2.setString("obj4_inner_string", "wangdaji");
            obj2.setBool("obj2_bool", false);
            obj2.setList("obj2_list", arr);
            obj2.setDateTime("obj2_time","1993-01-02 10:52:52.290");
            
            vector<TDJSONObject> objs;
            objs.push_back(obj1);
            objs.push_back(obj1);
            
            obj.setList("objs", objs);
#endif
            
            ThinkingAnalyticsAPI::setSuperProperties(obj);
            ThinkingAnalyticsAPI::setSuperProperties(obj, APPID_2);
        }
    });
    this->addChild(publicPropertyButton);


    auto publicPropertyButton1 = Button::create();
    publicPropertyButton1->setTitleFontSize(commonFontSize);
    publicPropertyButton1->setTitleText("setDyldProperty");
    publicPropertyButton1->setPosition(Vec2(25 + publicPropertyButton->getPositionX() + publicPropertyButton->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton1->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::setDynamicSuperProperties(superDylyPropery);
            ThinkingAnalyticsAPI::setDynamicSuperProperties(superDylyPropery, APPID_1);
            ThinkingAnalyticsAPI::setDynamicSuperProperties(superDylyPropery, APPID_2);
        }
    });
    this->addChild(publicPropertyButton1);


    auto publicPropertyButton2 = Button::create();
    publicPropertyButton2->setTitleFontSize(10);
    publicPropertyButton2->setTitleText("clearOneProperty");
    publicPropertyButton2->setPosition(Vec2(25 + publicPropertyButton1->getPositionX() + publicPropertyButton1->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton2->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::unsetSuperProperty("level");
            ThinkingAnalyticsAPI::unsetSuperProperty("level",APPID_1);
            ThinkingAnalyticsAPI::unsetSuperProperty("level",APPID_2);
        }
    });
    this->addChild(publicPropertyButton2);


    auto publicPropertyButton3 = Button::create();
    publicPropertyButton3->setTitleFontSize(10);
    publicPropertyButton3->setTitleText("ClearAllProperty");
    publicPropertyButton3->setPosition(Vec2(25 + publicPropertyButton2->getPositionX() + publicPropertyButton2->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton3->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            ThinkingAnalyticsAPI::clearSuperProperties();
            ThinkingAnalyticsAPI::clearSuperProperties(APPID_1);
            ThinkingAnalyticsAPI::clearSuperProperties(APPID_2);
        }
    });
    this->addChild(publicPropertyButton3);

//    auto flushButton = Button::create();
//    flushButton->setTitleFontSize(10);
//    flushButton->setTitleText("Flush");
//    flushButton->setPosition(Vec2(25 + publicPropertyButton3->getPositionX() + publicPropertyButton3->getContentSize().width,
//                                            publicPropertyButton->getPositionY()));
//    flushButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type){
//        if(type == ui::Widget::TouchEventType::ENDED)
//        {
////            ThinkingAnalyticsAPI::clearSuperProperties();
//            ThinkingAnalyticsAPI::flush();
//        }
//    });
//    this->addChild(flushButton);


//    PresetProperties* presetProperties =  ThinkingAnalyticsAPI::getPresetProperties(APPID_1);
//    string distintID =  ThinkingAnalyticsAPI::getDistinctId();

//    // CCLog();
//     string properties = TDJSONObject::toJson(*(presetProperties->toEventPresetProperties()));
//     printf("%s\n", properties.c_str());
//     printf("BundleID:%s\n",presetProperties->bundleId.c_str());
//     printf("Carrier:%s\n",presetProperties->carrier.c_str());
//     printf("DeviceID:%s\n",presetProperties->deviceId.c_str());
//     printf("DeviceModel:%s\n",presetProperties->deviceModel.c_str());
//     printf("Manufacturer:%s\n",presetProperties->manufacturer.c_str());
//     printf("NetworkType:%s\n",presetProperties->networkType.c_str());
//     printf("ScreenWidth:%i\n",presetProperties->screenWidth);
//     printf("ScreenHeight:%i\n",presetProperties->screenHeight);
//     printf("OS:%s\n",presetProperties->os.c_str());
//     printf("OSVersion:%s\n",presetProperties->osVersion.c_str());
//     printf("ZoneOffset:%f\n",presetProperties->zoneOffset);
//     printf("SystemLanguage:%s\n",presetProperties->systemLanguage.c_str());

    return true;
}




void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

