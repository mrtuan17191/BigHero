#ifndef DATA_EVENT_H
#define DATA_EVENT_H

#include "../mvc/include/mvc.h"
#include "../models/objects/Obj.h"

class DataEvent : public GameEvent {
    UG_PROPERTY_FULL(int, argumentInt, ArgumentInt);
    UG_PROPERTY_FULL(float, argumentFloat, ArgumentFloat);
    UG_PROPERTY_FULL(float, argumentFloat2, ArgumentFloat2);
    UG_PROPERTY_FULL(Point, argumentPoint, ArgumentPoint);
    UG_PROPERTY_FULL(void*, argumentReference, ArgumentReference);
    UG_PROPERTY_FULL(Obj*, argumentObject, ArgumentObject);
public:
    DataEvent();
    ~DataEvent();
    
};

#endif
