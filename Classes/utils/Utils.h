#ifndef UTILS_H
#define UTILS_H

#include "cocos2d.h"
#include "Resources.h"
#include "Box2D/Box2D.h"
#include "bodies/RectBody.h"
#include "bodies/ArcBody.h"
#include "../AppMacros.h"

USING_NS_CC;
using namespace std;

class Utils {
private:
    Utils();
    ~Utils();
public:
    static float random();
    static string readFile(std::string fileName, bool writeable, bool override, bool encrypt);
    static void writeFile(std::string data, std::string fileName, bool encrypt);
    static std::string encrypt(std::string toEncrypt);
    static std::string decrypt(std::string toDecrypt);
    static MenuItemSprite* createButton(char* text, int fontSize, string normalFrameName, string selectedFrameName);
    static MenuItemSprite* createButton(string normalFrameName, string selectedFrameName);
    static MenuItemLabel* createButtonLabel(char* text, int fontSize);
    static __Array* split(const std::string &s, char delim);
    static Label* createLabel(char* text, int fontSize);
    static Label* createLabel(char* text, int fontSize, char* fontName);
    static Label* createLabelBMF(string font, string text);
    static bool b2Interact(Body *body1, Body *body2);
    static bool isPointInRectangle(Point point, Point rectPosition, float width, float height);
    static void safeRelease(Ref* object);
    static void drawObject(RectBody* rectBody, DrawNode* pen);
};

#endif
