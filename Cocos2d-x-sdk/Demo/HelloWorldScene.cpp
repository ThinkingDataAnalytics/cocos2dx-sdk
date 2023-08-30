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
#include "SimpleAudioEngine.h"
#include <ctime>

using namespace thinkingdata::analytics;

#include "ui/CocosGUI.h"
using namespace cocos2d::ui;

//#include "editor-support/spine/Json.h"

void sampleCode() {
    //Init SDK
    string appId = "your-app-id";
    string serverUrl = "https://your.server.url";
    TDAnalytics::init(appId, serverUrl);
    //Login SDK
    TDAnalytics::login("Tiki");
    //Set Super Properties
    TDJSONObject superProperties;
    superProperties.setString("channel", "Apple Store");
    superProperties.setNumber("vip_level", 10);
    superProperties.setBool("is_vip", true);
    TDAnalytics::setSuperProperties(superProperties);
    //Track Event
    TDJSONObject eventProperties;
    eventProperties.setString("product_name", "Majin Taito");
    eventProperties.setNumber("product_price", 6);
    eventProperties.setBool("is_on_sale", true);
    TDAnalytics::track("product_buy", eventProperties);
    //Track User Properties
    TDJSONObject userProperties;
    userProperties.setString("email", "tiki@thinkingdata.cn");
    userProperties.setNumber("diamond", 888);
    userProperties.setBool("is_svip", true);
    TDAnalytics::userSet(userProperties);
}

TDJSONObject superDylyPropery() {

    vector<TDJSONObject> objs;
    
    vector<string> arr;
    arr.push_back("XX_1");
    arr.push_back("XX_2");
    arr.push_back("XX_3");
    
    TDJSONObject obj1;
    obj1.setNumber("doubleNum_dyld", 3.1415926);
    obj1.setNumber("intNum_dyld", 33333);
    obj1.setNumber("int64Num_dyld", 898989898);
    obj1.setString("string_dyld", "string");
    obj1.setBool("bool1_dyld", false);
    obj1.setBool("bool2_dyld", true);
    obj1.setList("list_dyld", arr);
    obj1.setDateTime("time_dyld","3033-03-03 10:52:52.290");
    
    TDJSONObject obj2;
    obj2.setNumber("doubleNum_dyld", 3.1415926);
    obj2.setNumber("intNum_dyld", 33333);
    obj2.setNumber("int64Num_dyld", 898989898);
    obj2.setString("string_dyld", "string");
    obj2.setBool("bool1_dyld", false);
    obj2.setBool("bool2_dyld", true);
    obj2.setList("list_dyld", arr);
    obj2.setDateTime("time_dyld","3033-03-03 10:52:52.290");
    
    TDJSONObject obj3;
    obj3.setNumber("doubleNum_dyld", 3.1415926);
    obj3.setNumber("intNum_dyld", 33333);
    obj3.setNumber("int64Num_dyld", 898989898);
    obj3.setString("string_dyld", "string");
    obj3.setBool("bool1_dyld", false);
    obj3.setBool("bool2_dyld", true);
    obj3.setList("list_dyld", arr);
    obj3.setDateTime("time_dyld","3033-03-03 10:52:52.290");
    
    objs.push_back(obj1);
    objs.push_back(obj1);
    
    TDJSONObject obj;
    obj.setList("objs_dyld", objs);
    obj.setJsonObject("object_dyld", obj3);

    return obj;
}

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

//    auto label = Label::createWithTTF("Hello World", "fonts/arial.ttf", 24);
//    if (label == nullptr)
//    {
//        problemLoading("'fonts/arial.ttf'");
//    }
//    else
//    {
//        // position the label on the center of the screen
//        label->setPosition(Vec2(origin.x + visibleSize.width/2,
//                                origin.y + visibleSize.height - label->getContentSize().height));
//
//        // add the label as a child to this layer
//        this->addChild(label, 1);
//    }

    // add "HelloWorld" splash screen"
//    auto sprite = Sprite::create("HelloWorld.png");
//    if (sprite == nullptr)
//    {
//        problemLoading("'HelloWorld.png'");
//    }
//    else
//    {
//        // position the sprite on the center of the screen
//        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//
//        // add the sprite as a child to this layer
//        this->addChild(sprite, 0);
//    }
    
    this->initUIElement(visibleSize, origin);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

TDJSONObject HelloWorld::parseJson(Json *json) {
    TDJSONObject jsonObject;
    Json * jsonChild = json->child;
    while (jsonChild != NULL) {
        if (jsonChild->type == Json_False) {
            jsonObject.setBool(jsonChild->name, false);
        } else if (jsonChild->type == Json_True) {
            jsonObject.setBool(jsonChild->name, true);
        } else if (jsonChild->type == Json_Number) {
            jsonObject.setNumber(jsonChild->name, jsonChild->valueInt);
        } else if (jsonChild->type == Json_String) {
            jsonObject.setString(jsonChild->name, jsonChild->valueString);
        } else if (jsonChild->type == Json_Array) {
            vector<string> stringArray;
            vector<TDJSONObject> objectArray;
            Json *arrayChild = jsonChild->child;
            while (arrayChild != NULL) {
                if (arrayChild->type == Json_String) {
                    stringArray.push_back(arrayChild->valueString);
                } else if (arrayChild->type == Json_Object) {
                    objectArray.push_back(HelloWorld::parseJson(arrayChild));
                }
                arrayChild = arrayChild->next;
            }
            if (stringArray.size() > 0) {
                jsonObject.setList(jsonChild->name, stringArray);
            } else if (objectArray.size() > 0) {
                jsonObject.setList(jsonChild->name, objectArray);
            }
        } else if (jsonChild->type == Json_Object) {
            jsonObject.setJsonObject(jsonChild->name, HelloWorld::parseJson(jsonChild));
        } else {
            
        }
        jsonChild = jsonChild->next;
    }
    return jsonObject;
}

void HelloWorld::initUIElement(Size visibleSize, Vec2 origin) {
    
//    //开启日志打印
//    TDAnalytics::enableTrackLog(true);
//
//    //在主线程中初始化 SDK
//    string TA_APP_ID = "51c052720f254e3a847d24a279382a39";
//    string TA_SERVER_URL = "https://receiver-ta-preview.thinkingdata.cn";
//    //初始化-1
//    //TDAnalytics::init(TA_APP_ID, TA_SERVER_URL);
//    //初始化-2
//    Config config(TA_APP_ID, TA_SERVER_URL);
//    config.setModel(TDMode::TD_DEBUG);
//    TDAnalytics::init(config);
//    TDAnalytics::enableAutoTrack(TA_APP_ID);

    auto leftMargin = origin.x + 50;
    auto topMargin = 25;
    auto commonFontSize = 10;
    auto commonMargin = 10;
    
    const auto inputTextField = EditBox::create(Size(200, 100), "CloseNormal.png");
    inputTextField->setFontSize(commonFontSize);
    inputTextField->setFontColor(Color3B::GRAY);
    inputTextField->setFontName("Helvetica");
    inputTextField->setPlaceholderFontSize(commonFontSize);
    inputTextField->setPlaceHolder("Input Json Text");
    inputTextField->setPlaceholderFontColor(Color3B::GRAY);
    inputTextField->setPlaceholderFontName("Helvetica");
    inputTextField->setPosition(Vec2((visibleSize.width - inputTextField->getContentSize().width),
                                     origin.y + visibleSize.height - (inputTextField->getContentSize().height)/2.0));
    this->addChild(inputTextField);
//    inputTextField->setText("{ \
//        \"timeZone\": \"\",\
//        \"appId\": \"debug-appId\",\
//        \"serverUrl\": \"https://receiver-ta-dev.thinkingdata.cn\",\
//        \"autoTrackName\": [\"ta_app_install\", \"ta_app_start\", \"ta_app_end\", \"ta_app_crash\", \"ta_app_view\"],\
//        \"model\": \"Debug\"\
//    }");

    //    TTFConfig commonConfig;
    //    commonConfig.fontFilePath = "fonts/arial.ttf";
    //    commonConfig.fontSize = commonFontSize;
    //    initButton->getTitleRenderer()->setTTFConfig(commonConfig);

    auto initButton = Button::create();
    initButton->setTitleText("Init SDK");
    initButton->setTitleFontName("fonts/arial.ttf");
    initButton->setTitleFontSize(commonFontSize);
    initButton->setPosition(Vec2(leftMargin, origin.y + visibleSize.height - initButton->getContentSize().height));
    initButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"appId\":\"22e445595b0f42bd8c5fe35bc44b88d6\",\"serverUrl\":\"https://receiver-ta-dev.thinkingdata.cn\"}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string appId = Json_getString(json, "appId", "22e445595b0f42bd8c5fe35bc44b88d6");
                string serverUrl = Json_getString(json, "serverUrl", "https://receiver.ta.thinkingdata.cn");
                string model = Json_getString(json, "model", "Normal");
                TDConfig config(appId, serverUrl);
                if (model == "Debug") {
                    config.setModel(TDMode::TD_DEBUG);
                }
                TDAnalytics::init(config);
                //开启日志打印
                TDAnalytics::enableTrackLog(true);
                Json_dispose(json);
                return;
            }

            //开启日志打印
            TDAnalytics::enableTrackLog(true);

            //在主线程中初始化 SDK
            string TA_APP_ID = "51c052720f254e3a847d24a279382a39";
            string TA_SERVER_URL = "https://receiver-ta-preview.thinkingdata.cn";
            //初始化-1
            //TDAnalytics::init(TA_APP_ID, TA_SERVER_URL);
            //初始化-2
            TDConfig config(TA_APP_ID, TA_SERVER_URL);
            config.setModel(TDMode::TD_NORMAL);
            TDAnalytics::init(config);
            
            //开启日志打印
            TDAnalytics::enableTrackLog(true);
        }
    });
    this->addChild(initButton);

    auto continuousButton = Button::create();
    continuousButton->setTitleFontName("fonts/arial.ttf");
    continuousButton->setTitleFontSize(commonFontSize);
    continuousButton->setTitleText("ContinuousTrack");
    continuousButton->setPosition(Vec2(commonMargin + initButton->getPositionX() + continuousButton->getContentSize().width,
                                       initButton->getPositionY()));
    continuousButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            clock_t last_clock = clock();
            printf("%ld\n", last_clock/CLOCKS_PER_SEC);
            while (true) {
                clock_t now_clock = clock();
                if (now_clock - last_clock >= CLOCKS_PER_SEC) {
                    TDAnalytics::track("ContinuousTrack");
                    TDJSONObject properties;
                    properties.setString("ContinuousUser", "abc");
                    TDAnalytics::userSet(properties);
                    last_clock = now_clock;
                }
            }
        }
    });
    this->addChild(continuousButton);

    
    auto userLabel = Label::createWithTTF("UserIdentify", "fonts/arial.ttf", 12);
    userLabel->setTextColor(Color4B::ORANGE);
    userLabel->setPosition(Vec2((visibleSize.width -userLabel->getContentSize().width)/2.0 ,
                                origin.y + visibleSize.height - userLabel->getContentSize().height));
    this->addChild(userLabel);
//    userLabel->getFontAtlas()->setAliasTexParameters();
//    userLabel->setFontAtlas(NULL);

    auto userButton = Button::create();
    userButton->setTitleText("Login");
    userButton->setTitleFontName("fonts/arial.ttf");
    userButton->setTitleFontSize(commonFontSize);
    userButton->setPosition(Vec2(leftMargin,
                                 userLabel->getPositionY() - topMargin));
    userButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"name\":\"cocos2d-x-accountid\"}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string name = Json_getString(json, "name", "cocos2d-x-accountid");
                TDAnalytics::login(name);
                Json_dispose(json);
                return;
            }
            
            TDAnalytics::login("cocos2d-x-accountid");
        }
    });
    this->addChild(userButton);


    auto userButton1 = Button::create();
    userButton1->setTitleFontName("fonts/arial.ttf");
    userButton1->setTitleFontSize(commonFontSize);
    userButton1->setTitleText("SetDistinctId");
    userButton1->setPosition(Vec2(commonMargin + userButton->getPositionX() + userButton1->getContentSize().width,
                                  userButton->getPositionY()));
    userButton1->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"name\":\"cocos2d-x-identify\"}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string name = Json_getString(json, "name", "cocos2d-x-identify");
                TDAnalytics::setDistinctId(name);
                Json_dispose(json);
                return;
            }

            TDAnalytics::setDistinctId(("cocos2d-x-identify"));
        }
    });
    this->addChild(userButton1);

    auto userButton2 = Button::create();
    userButton2->setTitleFontName("fonts/arial.ttf");
    userButton2->setTitleFontSize(commonFontSize);
    userButton2->setTitleText("Logout");
    userButton2->setPosition(Vec2(commonMargin + userButton1->getPositionX() + userButton2->getContentSize().width,
                                  userButton->getPositionY()));
    userButton2->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::logout();
        }
    });
    this->addChild(userButton2);
    
    auto userButton22 = Button::create();
    userButton22->setTitleFontName("fonts/arial.ttf");
    userButton22->setTitleFontSize(commonFontSize);
    userButton22->setTitleText("Disable");
    userButton22->setPosition(Vec2(commonMargin + userButton2->getPositionX() + userButton22->getContentSize().width,
                                  userButton->getPositionY()));
    userButton22->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::enableTracking(false);
        }
    });
    this->addChild(userButton22);
    
    auto userButton33 = Button::create();
    userButton33->setTitleFontName("fonts/arial.ttf");
    userButton33->setTitleFontSize(commonFontSize);
    userButton33->setTitleText("Enable");
    userButton33->setPosition(Vec2(commonMargin + userButton22->getPositionX() + userButton33->getContentSize().width,
                                  userButton->getPositionY()));
    userButton33->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::enableTracking(true);
        }
    });
    this->addChild(userButton33);
    
    auto userButton44 = Button::create();
    userButton44->setTitleFontName("fonts/arial.ttf");
    userButton44->setTitleFontSize(commonFontSize);
    userButton44->setTitleText("OptOut");
    userButton44->setPosition(Vec2(commonMargin + userButton33->getPositionX() + userButton44->getContentSize().width,
                                  userButton->getPositionY()));
    userButton44->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::optOutTracking();
        }
    });
    this->addChild(userButton44);
    
    auto userButton55 = Button::create();
    userButton55->setTitleFontName("fonts/arial.ttf");
    userButton55->setTitleFontSize(commonFontSize);
    userButton55->setTitleText("OptIn");
    userButton55->setPosition(Vec2(commonMargin + userButton44->getPositionX() + userButton55->getContentSize().width,
                                  userButton->getPositionY()));
    userButton55->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::optInTracking();
        }
    });
    this->addChild(userButton55);


    auto eventLabel = Label::createWithTTF("EventTrack", "fonts/arial.ttf", 12);
    eventLabel->setTextColor(Color4B::ORANGE);
    eventLabel->setPosition(Vec2(userLabel->getPositionX(),
                                 userButton->getPositionY() - topMargin));
    this->addChild(eventLabel);

    auto eventButton = Button::create();
    eventButton->setTitleFontName("fonts/arial.ttf");
    eventButton->setTitleText("Track");
    eventButton->setTitleFontSize(commonFontSize);
    eventButton->setPosition(Vec2(leftMargin,
                                  eventLabel->getPositionY() - topMargin));
    eventButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"event_name\":\"TA\"}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string event_name = Json_getString(json, "event_name", "TA");
                TDAnalytics::track(event_name);
                Json_dispose(json);
                return;
            }

            TDAnalytics::track("TA");
        }
    });
    this->addChild(eventButton);


    auto eventButton1 = Button::create();
    eventButton1->setTitleText("TrackProperties");
    eventButton1->setTitleFontName("fonts/arial.ttf");
    eventButton1->setTitleFontSize(commonFontSize);
    eventButton1->setPosition(Vec2(commonMargin + eventButton->getPositionX() + eventButton1->getContentSize().width ,
                                   eventButton->getPositionY()));
    eventButton1->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"event_name\":\"TA\",\"properties\":{\"ta_key_1\":1,\"ta_key_2\":\"2\",\"ta_key_3\":true,\"ta_key_4\":[\"1\",\"2\",\"3\"],\"ta_key_5\":{\"ta_sub_key\":\"abc\"}},\"time\":\"\",\"timeZone\":\"\"}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string event_name = Json_getString(json, "event_name", "TA");
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDAnalytics::track(event_name, properties);
                Json_dispose(json);
                return;
            }

            vector<TDJSONObject> objs;
            
            vector<string> arr;
            arr.push_back("XX_1");
            arr.push_back("XX_2");
            arr.push_back("XX_3");
            
            TDJSONObject obj1;
            obj1.setNumber("doubleNum", 3.1415926);
            obj1.setNumber("intNum", 33333);
            obj1.setNumber("int64Num", 898989898);
            obj1.setString("string", "string");
            obj1.setBool("bool1", false);
            obj1.setBool("bool2", true);
            obj1.setList("list", arr);
            obj1.setDateTime("time","3033-03-03 10:52:52.290");
            
            TDJSONObject obj2;
            obj2.setNumber("doubleNum", 3.1415926);
            obj2.setNumber("intNum", 33333);
            obj2.setNumber("int64Num", 898989898);
            obj2.setString("string", "string");
            obj2.setBool("bool1", false);
            obj2.setBool("bool2", true);
            obj2.setList("list", arr);
            obj2.setDateTime("time","3033-03-03 10:52:52.290");
            
            TDJSONObject obj3;
            obj3.setNumber("doubleNum", 3.1415926);
            obj3.setNumber("intNum", 33333);
            obj3.setNumber("int64Num", 898989898);
            obj3.setString("string", "string");
            obj3.setBool("bool1", false);
            obj3.setBool("bool2", true);
            obj3.setList("list", arr);
            obj3.setDateTime("time","3033-03-03 10:52:52.290");
            
            objs.push_back(obj1);
            objs.push_back(obj1);
            
            TDJSONObject obj;
            obj.setList("objs", objs);
            obj.setJsonObject("obj", obj3);

            TDAnalytics::track("track_property",obj);
        }
    });
    this->addChild(eventButton1);

    auto eventButton2 = Button::create();
    eventButton2->setTitleFontName("fonts/arial.ttf");
    eventButton2->setTitleFontSize(commonFontSize);
    eventButton2->setTitleText("TimeEvent");
    eventButton2->setPosition(Vec2(commonMargin + eventButton1->getPositionX() + eventButton2->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton2->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"event_name\":\"TA\",\"properties\":{\"ta_key_1\":1,\"ta_key_2\":\"2\",\"ta_key_3\":true,\"ta_key_4\":[\"1\",\"2\",\"3\"],\"ta_key_5\":{\"ta_sub_key\":\"abc\"}},\"time\":\"\",\"timeZone\":\"\"}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string event_name = Json_getString(json, "event_name", "TA");
                TDAnalytics::timeEvent(event_name);
                Json_dispose(json);
                return;
            }

            TDAnalytics::timeEvent("TA");
        }
    });
    this->addChild(eventButton2);

    auto eventButton3 = Button::create();
    eventButton3->setTitleFontName("fonts/arial.ttf");
    eventButton3->setTitleText("FirstEvent");
    eventButton3->setTitleFontSize(commonFontSize);
    eventButton3->setPosition(Vec2(commonMargin + eventButton2->getPositionX() + eventButton3->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton3->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"event_name\":\"mutable_event\",\"event_id\":\"event_id_10001\",\"properties\":{\"mutable_key\":1}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string event_name = Json_getString(json, "event_name", "TA");
                string event_id = Json_getString(json, "event_id", "");
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDFirstEventModel *eventModel = new TDFirstEventModel(event_name, properties);
                if (event_id.length() > 0) {
                    eventModel->setFirstCheckId(event_id);
                }
                TDAnalytics::track(eventModel);
                Json_dispose(json);
                return;
            }
            
            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            TDFirstEventModel *firstEvent2 = new TDFirstEventModel("firstEvent",jsonObject1);
            TDAnalytics::track(firstEvent2);
            
            firstEvent2->setFirstCheckId("firstCheckID");
            TDAnalytics::track(firstEvent2);
        }
    });
    this->addChild(eventButton3);

    auto eventButton4 = Button::create();
    eventButton4->setTitleFontName("fonts/arial.ttf");
    eventButton4->setTitleText("UpdatableEvent");
    eventButton4->setTitleFontSize(commonFontSize);
    eventButton4->setPosition(Vec2(commonMargin + eventButton3->getPositionX() + eventButton4->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton4->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"event_name\":\"mutable_event\",\"event_id\":\"event_id_10001\",\"properties\":{\"mutable_key\":1}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string event_name = Json_getString(json, "event_name", "TA");
                string event_id = Json_getString(json, "event_id", "");
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDUpdatableEventModel *eventModel = new TDUpdatableEventModel(event_name, properties, event_id);
                TDAnalytics::track(eventModel);
                Json_dispose(json);
                return;
            }

            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            jsonObject1.setNumber("status", 3);
            jsonObject1.setNumber("price", 5);
            TDUpdatableEventModel *updatableEvent1 = new TDUpdatableEventModel("updateEvent",jsonObject1,"12345");
            TDAnalytics::track(updatableEvent1);
        }
    });
    this->addChild(eventButton4);



    auto eventButton5 = Button::create();
    eventButton5->setTitleFontName("fonts/arial.ttf");
    eventButton5->setTitleText("OverWritableEvent");
    eventButton5->setTitleFontSize(commonFontSize);
    eventButton5->setPosition(Vec2(commonMargin + eventButton4->getPositionX() + eventButton5->getContentSize().width,
                                   eventButton->getPositionY()));
    eventButton5->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"event_name\":\"mutable_event\",\"event_id\":\"event_id_10001\",\"properties\":{\"mutable_key\":1}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                string event_name = Json_getString(json, "event_name", "TA");
                string event_id = Json_getString(json, "event_id", "");
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDOverwritableEventModel *eventModel = new TDOverwritableEventModel(event_name, properties, event_id);
                TDAnalytics::track(eventModel);
                Json_dispose(json);
                return;
            }

            TDJSONObject jsonObject1;
            jsonObject1.setString("test","test");
            TDOverwritableEventModel *overWritableEvent1 = new TDOverwritableEventModel("overWriteEvent",jsonObject1,"12345");
            TDAnalytics::track(overWritableEvent1);
        }
    });
    this->addChild(eventButton5);



    auto userPropertyLabel = Label::createWithTTF("UserProperties", "fonts/arial.ttf", 12);
    userPropertyLabel->setPosition(Vec2(userLabel->getPositionX(),
                                        eventButton->getPositionY() - topMargin));
    this->addChild(userPropertyLabel);
    userPropertyLabel->setTextColor(Color4B::ORANGE);
    
    auto userPropertyButton = Button::create();
    userPropertyButton->setTitleFontName("fonts/arial.ttf");
    userPropertyButton->setTitleFontSize(commonFontSize);
    userPropertyButton->setTitleText("UserSet");
    userPropertyButton->setPosition(Vec2(leftMargin,
                                         userPropertyLabel->getPositionY() - topMargin));
    userPropertyButton->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"properties\":{\"name\":\"Tiki\",\"age\":20}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDAnalytics::userSet(properties);
                Json_dispose(json);
                return;
            }

            TDJSONObject userProperties1;
            userProperties1.setString("userSet_key", "userSet_value");
            TDAnalytics::userSet(userProperties1);
        }
    });
    this->addChild(userPropertyButton);


    auto userPropertyButton1 = Button::create();
    userPropertyButton1->setTitleFontName("fonts/arial.ttf");
    userPropertyButton1->setTitleFontSize(commonFontSize);
    userPropertyButton1->setTitleText("UserSetOnce");
    userPropertyButton1->setPosition(Vec2(commonMargin + userPropertyButton->getPositionX() + userPropertyButton1->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton1->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"properties\":{\"name\":\"Tiki\",\"age\":20}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDAnalytics::userSetOnce(properties);
                Json_dispose(json);
                return;
            }

            TDJSONObject userProperties1;
            userProperties1.setString("userSetOnce_key", "userSetOnce_value");
            userProperties1.setNumber("userSetOnce_int",1);
            TDAnalytics::userSetOnce(userProperties1);
        }
    });
    this->addChild(userPropertyButton1);


    auto userPropertyButton2 = Button::create();
    userPropertyButton2->setTitleFontName("fonts/arial.ttf");
    userPropertyButton2->setTitleFontSize(commonFontSize);
    userPropertyButton2->setTitleText("UserAdd");
    userPropertyButton2->setPosition(Vec2(commonMargin + userPropertyButton1->getPositionX() + userPropertyButton2->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton2->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"properties\":{\"name\":\"Tiki\",\"age\":20}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDAnalytics::userAdd(properties);
                Json_dispose(json);
                return;
            }

            TDJSONObject userProperties1;
            userProperties1.setNumber("userAdd_int",1);
            TDAnalytics::userAdd(userProperties1);
        }
    });
    this->addChild(userPropertyButton2);


    auto userPropertyButton3 = Button::create();
    userPropertyButton3->setTitleFontName("fonts/arial.ttf");
    userPropertyButton3->setTitleFontSize(commonFontSize);
    userPropertyButton3->setTitleText("UserUnset");
    userPropertyButton3->setPosition(Vec2(commonMargin + userPropertyButton2->getPositionX() + userPropertyButton3->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton3->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"property\":[\"user_age\",\"user_none\"]}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                Json * propertiesJson = Json_getItem(json, "property");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                Json *jsonChild = propertiesJson->child;
                while (jsonChild != NULL) {
                    TDAnalytics::userUnset(jsonChild->valueString);
                    jsonChild = jsonChild->next;
                }
                Json_dispose(json);
                return;
            }

            TDAnalytics::userUnset("userUnset_key");
        }
    });
    this->addChild(userPropertyButton3);


    auto userPropertyButton4 = Button::create();
    userPropertyButton4->setTitleFontName("fonts/arial.ttf");
    userPropertyButton4->setTitleFontSize(commonFontSize);
    userPropertyButton4->setTitleText("UserDelete");
    userPropertyButton4->setPosition(Vec2(commonMargin + userPropertyButton3->getPositionX() + userPropertyButton4->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton4->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::userDelete();
        }
    });
    this->addChild(userPropertyButton4);

    auto userPropertyButton5 = Button::create();
    userPropertyButton5->setTitleFontName("fonts/arial.ttf");
    userPropertyButton5->setTitleFontSize(commonFontSize);
    userPropertyButton5->setTitleText("UserAppend");
    userPropertyButton5->setPosition(Vec2(commonMargin + userPropertyButton4->getPositionX() + userPropertyButton5->getContentSize().width,
                                          userPropertyButton->getPositionY()));
    userPropertyButton5->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"properties\":{\"name\":\"Tiki\",\"age\":20}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDAnalytics::userAppend(properties);
                TDAnalytics::userUniqAppend(properties);
                Json_dispose(json);
                return;
            }

            TDJSONObject userProperties1;
            vector<string> listValue1;
            listValue1.push_back("XX");
            userProperties1.setList("userAppend_key",listValue1);
            TDAnalytics::userAppend(userProperties1);
            TDAnalytics::userUniqAppend(userProperties1);
        }
    });
    this->addChild(userPropertyButton5);
    

    auto publicPropertyLabel = Label::createWithTTF("CommonProperties", "fonts/arial.ttf", 12);
    publicPropertyLabel->setTextColor(Color4B::ORANGE);
    publicPropertyLabel->setPosition(Vec2(userLabel->getPositionX(),
                                          userPropertyButton->getPositionY() - topMargin));
    this->addChild(publicPropertyLabel);

    auto publicPropertyButton = Button::create();
    publicPropertyButton->setTitleFontName("fonts/arial.ttf");
    publicPropertyButton->setTitleFontSize(commonFontSize);
    publicPropertyButton->setTitleText("SetSuperProperties");
    publicPropertyButton->setPosition(Vec2(leftMargin,
                                           publicPropertyLabel->getPositionY() - topMargin));
    publicPropertyButton->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"properties\":{\"super_name\":\"Tiki\",\"super_age\":20}}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                Json * propertiesJson = Json_getItem(json, "properties");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                TDAnalytics::setSuperProperties(properties);
                Json_dispose(json);
                return;
            }

            vector<TDJSONObject> objs;
            
            vector<string> arr;
            arr.push_back("XX_1");
            arr.push_back("XX_2");
            arr.push_back("XX_3");
            
            TDJSONObject obj1;
            obj1.setNumber("doubleNum_static", 3.1415926);
            obj1.setNumber("intNum_static", 33333);
            obj1.setNumber("int64Num_static", 898989898);
            obj1.setString("string_static", "string");
            obj1.setBool("bool1_static", false);
            obj1.setBool("bool2_static", true);
            obj1.setList("list_static", arr);
            obj1.setDateTime("time_static","3033-03-03 10:52:52.290");
            
            TDJSONObject obj2;
            obj2.setNumber("doubleNum_static", 3.1415926);
            obj2.setNumber("intNum_static", 33333);
            obj2.setNumber("int64Num_static", 898989898);
            obj2.setString("string_static", "string");
            obj2.setBool("bool1_static", false);
            obj2.setBool("bool2_static", true);
            obj2.setList("list_static", arr);
            obj2.setDateTime("time_static","3033-03-03 10:52:52.290");
            
            TDJSONObject obj3;
            obj3.setNumber("doubleNum_static", 3.1415926);
            obj3.setNumber("intNum_static", 33333);
            obj3.setNumber("int64Num_static", 898989898);
            obj3.setString("string_static", "string");
            obj3.setBool("bool1_static", false);
            obj3.setBool("bool2_static", true);
            obj3.setList("list_static", arr);
            obj3.setDateTime("time_static","3033-03-03 10:52:52.290");
            
            objs.push_back(obj1);
            objs.push_back(obj1);
            
            TDJSONObject obj;
            obj.setList("objs_static", objs);
            obj.setJsonObject("obj_static", obj3);
            obj.setList("list_static", arr);
            
            TDAnalytics::setSuperProperties(obj);
        }
    });
    this->addChild(publicPropertyButton);


    auto publicPropertyButton1 = Button::create();
    publicPropertyButton1->setTitleFontName("fonts/arial.ttf");
    publicPropertyButton1->setTitleFontSize(commonFontSize);
    publicPropertyButton1->setTitleText("SetDynamicProperties");
    publicPropertyButton1->setPosition(Vec2(commonMargin + publicPropertyButton->getPositionX() + publicPropertyButton1->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton1->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::setDynamicSuperProperties(superDylyPropery);
        }
    });
    this->addChild(publicPropertyButton1);


    auto publicPropertyButton2 = Button::create();
    publicPropertyButton2->setTitleFontName("fonts/arial.ttf");
    publicPropertyButton2->setTitleFontSize(commonFontSize);
    publicPropertyButton2->setTitleText("UnsetSuperProperty");
    publicPropertyButton2->setPosition(Vec2(commonMargin + publicPropertyButton1->getPositionX() + publicPropertyButton2->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton2->addTouchEventListener([inputTextField, this](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const char * jsonText = inputTextField->getText();
            // jsonText = "{\"property\":[\"super_age\",\"super_none\"]}";
            Json* json = Json_create(jsonText);
            if (json != NULL) {
                Json * propertiesJson = Json_getItem(json, "property");
                TDJSONObject properties = HelloWorld::parseJson(propertiesJson);
                Json *jsonChild = propertiesJson->child;
                while (jsonChild != NULL) {
                    TDAnalytics::unsetSuperProperty(jsonChild->valueString);
                    jsonChild = jsonChild->next;
                }
                Json_dispose(json);
                return;
            }

            TDAnalytics::unsetSuperProperty("obj_static");
        }
    });
    this->addChild(publicPropertyButton2);


    auto publicPropertyButton3 = Button::create();
    publicPropertyButton3->setTitleFontName("fonts/arial.ttf");
    publicPropertyButton3->setTitleFontSize(commonFontSize);
    publicPropertyButton3->setTitleText("ClearSuperProperties");
    publicPropertyButton3->setPosition(Vec2(commonMargin + publicPropertyButton2->getPositionX() + publicPropertyButton3->getContentSize().width,
                                            publicPropertyButton->getPositionY()));
    publicPropertyButton3->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::clearSuperProperties();
        }
    });
    this->addChild(publicPropertyButton3);

    auto flushButton = Button::create();
    flushButton->setTitleFontName("fonts/arial.ttf");
    flushButton->setTitleFontSize(commonFontSize);
    flushButton->setTitleText("Flush");
    flushButton->setPosition(Vec2(leftMargin,
                                  publicPropertyButton3->getPositionY() - topMargin));
    flushButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDAnalytics::flush();
        }
    });
    this->addChild(flushButton);


    auto deviceIdButton = Button::create();
    deviceIdButton->setTitleFontName("fonts/arial.ttf");
    deviceIdButton->setTitleFontSize(commonFontSize);
    deviceIdButton->setTitleText("GetDeviceId");
    deviceIdButton->setPosition(Vec2(commonMargin + flushButton->getPositionX() + deviceIdButton->getContentSize().width,
                                     flushButton->getPositionY()));
    deviceIdButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            string deviceId = TDAnalytics::getDeviceId();
            inputTextField->setText(("DeviceId = " + deviceId).c_str());
        }
    });
    this->addChild(deviceIdButton);
    
    auto distinctIdButton = Button::create();
    distinctIdButton->setTitleFontName("fonts/arial.ttf");
    distinctIdButton->setTitleFontSize(commonFontSize);
    distinctIdButton->setTitleText("GetDistinctId");
    distinctIdButton->setPosition(Vec2(commonMargin + deviceIdButton->getPositionX() + distinctIdButton->getContentSize().width,
                                     flushButton->getPositionY()));
    distinctIdButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            string distinctId = TDAnalytics::getDistinctId();
            inputTextField->setText(("DistinctId = " + distinctId).c_str());
        }
    });
    this->addChild(distinctIdButton);


    auto superPropertiesButton = Button::create();
    superPropertiesButton->setTitleFontName("fonts/arial.ttf");
    superPropertiesButton->setTitleFontSize(commonFontSize);
    superPropertiesButton->setTitleText("GetSuperProperties");
    superPropertiesButton->setPosition(Vec2(commonMargin + distinctIdButton->getPositionX() + superPropertiesButton->getContentSize().width,
                                     flushButton->getPositionY()));
    superPropertiesButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject superProperties = TDAnalytics::getSuperProperties();
            inputTextField->setText(("SuperProperties = " + TDJSONObject::toJson(superProperties)).c_str());
        }
    });
    this->addChild(superPropertiesButton);

    auto presetPropertiesButton = Button::create();
    presetPropertiesButton->setTitleFontName("fonts/arial.ttf");
    presetPropertiesButton->setTitleFontSize(commonFontSize);
    presetPropertiesButton->setTitleText("GetPresetProperties");
    presetPropertiesButton->setPosition(Vec2(commonMargin + superPropertiesButton->getPositionX() + presetPropertiesButton->getContentSize().width,
                                     flushButton->getPositionY()));
    presetPropertiesButton->addTouchEventListener([inputTextField](Ref* sender, Widget::TouchEventType type){
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            TDJSONObject *presetProperties = TDAnalytics::getPresetProperties()->toEventPresetProperties();
            inputTextField->setText(("PresetProperties = " + TDJSONObject::toJson(*presetProperties)).c_str());
        }
    });
    this->addChild(presetPropertiesButton);

    
//    PresetProperties* presetProperties =  TDAnalytics::getPresetProperties(APPID_1);
//    string distintID =  TDAnalytics::getDistinctId();

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

}

