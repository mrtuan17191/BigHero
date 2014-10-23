#ifndef BUSINESS_HANDLER_H
#define BUSINESS_HANDLER_H

#include "mvc/include/mvc.h"
#include "controllers/include/controllers.h"
#include "views/include/views.h"
#include "models/include/models.h"
#include "events/include/events.h"

class BusinessHandler: public GameHandler {	
public:
    BusinessHandler();
    virtual ~BusinessHandler();
    virtual void processEvent(GameEvent* event);
};

#endif
