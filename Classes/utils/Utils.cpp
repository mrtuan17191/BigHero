#include "Utils.h"
using namespace std;

Utils::Utils() {
}

Utils::~Utils() {
}

float Utils::random() {
    float random = (float) rand() / static_cast<double>(RAND_MAX);
    return random;
}

std::string Utils::readFile(std::string fileName, bool writeable, bool override, bool encrypt) {
    std::string path = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    path = FileUtils::sharedFileUtils()->getWritablePath() + fileName;
    FILE* file = fopen(path.c_str(), "rb");
    if (file == NULL || override) {
        ssize_t fileSize = 0;
        unsigned char* pBuffer = FileUtils::getInstance()->getFileData(fileName.c_str(), "rb", &fileSize);
        
        FILE *fpw = fopen(path.c_str(), "wb");
        fwrite(pBuffer, 1, fileSize, fpw);
        fclose(fpw);
    }
    if (file != NULL) {
        fclose(file);
    }
    
#endif
   
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    if (writeable) {
        path = FileUtils::getInstance()->getWritablePath() + fileName;
    } else {
        path = FileUtils::getInstance()->fullPathForFilename(fileName.c_str());
    }
#endif
    
	FILE *fp = fopen(path.c_str(), "rb");
    char* allStringData = NULL;
    
	if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        long fileLength = ftell(fp);
        allStringData = (char*)std::malloc(fileLength * sizeof(char) + 1);
        fseek(fp, SEEK_SET, 0);
        fread(allStringData, fileLength, 1, fp);
        allStringData[fileLength] = '\0';
        fclose(fp);
	}
    
    if (allStringData == NULL) {
        return "";
    }
    
    if (encrypt) {
        std::string decryptData = Utils::decrypt(allStringData);
        return decryptData;
    }
    return allStringData;
}

void Utils::writeFile(std::string data, std::string fileName, bool encrypt) {
    std::string path = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    path = FileUtils::getInstance()->getWritablePath() + fileName;
#endif
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID && CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    path = FileUtils::sharedFileUtils()->fullPathForFilename(fileName.c_str());
#endif
    
    FILE *fp = fopen(path.c_str(), "wb");
    std::string dataToWrite = data;
    if (encrypt) {
        dataToWrite = Utils::encrypt(dataToWrite.data());
    }
    
    if (fp != NULL) {
        fwrite(dataToWrite.data(), sizeof(char), dataToWrite.length(), fp);
        fclose(fp);
    }
}

std::string Utils::encrypt(std::string toEncrypt) {
    std::string keyStr = UG_KEY;
    
    std::string output = toEncrypt;
    for (int i = 0; i < keyStr.length(); i++) {
        char key = keyStr[i];
        for (int j = 0; j < toEncrypt.size(); j++) {
            output[j] = toEncrypt[j] ^ key;
        }
        toEncrypt = output;
    }
    return output;
}

std::string Utils::decrypt(std::string toDecript) {
    std::string keyStr = UG_KEY;
    
    std::string output = toDecript;
    for (int i = keyStr.length() - 1; i >= 0; i--) {
        char key = keyStr[i];
        for (int j = 0; j < toDecript.size(); j++) {
            output[j] = toDecript[j] ^ key;
        }
        toDecript = output;
    }
    return output;
}

__Array* Utils::split(const std::string &s, char delim) {
    __Array* result = __Array::create();
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        result->addObject(CCString::create(item));
    }
    return result;
}

MenuItemSprite* Utils::createButton(char* text, int fontSize, string normalFrameName, string selectedFrameName) {
    Sprite* normalSprite = Sprite::createWithSpriteFrameName(normalFrameName);
    Sprite* selectedSprite = Sprite::createWithSpriteFrameName(selectedFrameName);
    MenuItemSprite* button = MenuItemSprite::create(normalSprite, selectedSprite);
    
    Label* normalText = Label::createWithBMFont(FONT_GAME_SMALL, text);
    normalText->setPosition(Point(button->getContentSize().width / 2, button->getContentSize().height / 2));
    normalText->enableShadow();
    normalSprite->addChild(normalText);
    Label* selectedText = Label::createWithBMFont(FONT_GAME_SMALL, text);
    selectedText->setPosition(Point(button->getContentSize().width / 2, button->getContentSize().height / 2));
    selectedText->enableShadow();
    selectedSprite->addChild(selectedText);
    
    return button;
}

Label* Utils::createLabelBMF(string font, string text) {
    Label* normalText = Label::createWithBMFont(font, text);
    return normalText;
}

MenuItemSprite* Utils::createButton(string normalFrameName, string selectedFrameName) {
    Sprite* normalSprite = Sprite::createWithSpriteFrameName(normalFrameName);
    Sprite* selectedSprite = Sprite::createWithSpriteFrameName(selectedFrameName);
    MenuItemSprite* button = MenuItemSprite::create(normalSprite, selectedSprite);
    
    return button;
}

MenuItemLabel* Utils::createButtonLabel(char* text, int fontSize) {
    Label* textLabel = Utils::createLabel(text, fontSize);
    MenuItemLabel* buttonLabel = MenuItemLabel::create(textLabel);
    return buttonLabel;
}

Label* Utils::createLabel(char* text, int fontSize) {
    Label* label = Label::createWithSystemFont(text, (char*) FONT_ARIAL, fontSize);
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setDimensions(label->getContentSize().width + 20, 0);
    return label;
}

Label* Utils::createLabel(char* text, int fontSize, char* fontName) {
    Label* label = Label::createWithTTF(text, fontName, fontSize);
    label->setHorizontalAlignment(TextHAlignment::CENTER);
    label->setDimensions(label->getContentSize().width + 20, 0);
    return label;
}

bool Utils::b2Interact(Body *body1, Body *body2) {
    if (!body1->isPhysical() || !body2->isPhysical()) {
        return false;
    }
    
    b2Transform s1Transform, s2Transform;
    s1Transform.Set(b2Vec2(body1->getPosition().x, body1->getPosition().y), body1->getRotation() * 3.14 / 180);
    s2Transform.Set(b2Vec2(body2->getPosition().x, body2->getPosition().y), body2->getRotation() * 3.14 / 180);
    
    b2Manifold manifold;
    
    bool body1ShapeType = body1->isRect();
    bool body2ShapeType = body2->isRect();
    
    if (body1ShapeType && !body2ShapeType) {
        b2PolygonShape shape1;
        RectBody* shape1Body = (RectBody*) body1;
        shape1.SetAsBox(shape1Body->getWidth() / 2, shape1Body->getHeight() / 2);
        
        b2CircleShape shape2;
        ArcBody* shape2Body = (ArcBody*) body2;
        shape2.m_radius = shape2Body->getRadius();
        
        b2CollidePolygonAndCircle(&manifold, &shape1, s1Transform, &shape2, s2Transform);
    } else if (!body1ShapeType && body2ShapeType) {
        b2CircleShape shape1;
        ArcBody* shape1Body = (ArcBody*) body1;
        shape1.m_radius = shape1Body->getRadius();
        
        b2PolygonShape shape2;
        RectBody* shape2Body = (RectBody*) body2;
        shape2.SetAsBox(shape2Body->getWidth() / 2, shape2Body->getHeight() / 2);
        
        b2CollidePolygonAndCircle(&manifold, &shape2, s2Transform, &shape1, s1Transform);
    } else if (!body1ShapeType && !body2ShapeType) {
        b2CircleShape shape1;
        ArcBody* shape1Body = (ArcBody*) body1;
        shape1.m_radius = shape1Body->getRadius();
        
        b2CircleShape shape2;
        ArcBody* shape2Body = (ArcBody*) body2;
        shape2.m_radius = shape2Body->getRadius();
        
        b2CollideCircles(&manifold, &shape1, s1Transform, &shape2, s2Transform);
    } else if (body1ShapeType && body2ShapeType) {
        b2PolygonShape shape1;
        RectBody* shape1Body = (RectBody*) body1;
        shape1.SetAsBox(shape1Body->getWidth() / 2, shape1Body->getHeight() / 2);
        
        b2PolygonShape shape2;
        RectBody* shape2Body = (RectBody*) body2;
        shape2.SetAsBox(shape2Body->getWidth() / 2, shape2Body->getHeight() / 2);
        
        b2CollidePolygons(&manifold, &shape1, s1Transform, &shape2, s2Transform);
    }
    
    if (manifold.pointCount > 0) {
        return true;
    }
    
    return false;
}

bool Utils::isPointInRectangle(Point point, Point rectPosition, float width, float height) {
    float x1 = rectPosition.x - width / 2;
	float x2 = rectPosition.x + width / 2;
    float y1 = rectPosition.y - height / 2;
	float y2 = rectPosition.y + height / 2;
    return ((x1 <= point.x) && (point.x <= x2) && (y1 <= point.y) && (point.y <= y2));
}

void Utils::safeRelease(Ref* object) {
    if (object != NULL) {
        if (object->getReferenceCount() > 0) {
            CC_SAFE_RELEASE(object);
        }
    }
}

void Utils::drawObject(RectBody* rectBody, DrawNode* pen) {
    float x = rectBody->getPosition().x;
    float y = rectBody->getPosition().y;
    float width = rectBody->getWidth();
    float height = rectBody->getHeight();
    
    Point* points = new Point[4];
    points[0] = Point(x - width / 2, y + height / 2);
    points[1] = Point(x + width / 2, y + height / 2);
    points[2] = Point(x + width / 2, y - height / 2);
    points[3] = Point(x - width / 2, y - height / 2);

    pen->drawPolygon(points, 4, Color4F(1, 0, 0, 1), 3, Color4F(1, 0, 0, 1));
}
