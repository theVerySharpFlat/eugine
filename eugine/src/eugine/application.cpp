//
// Created by aiden on 8/2/21.
//

#include "application.h"
#include <eugine/event/applicationEvent.h>
#include <eugine/event/mouseEvent.h>
#include <eugine/log.h>

eg::Application::Application() {

}

eg::Application::~Application() {

}

void eg::Application::run() {
    MouseButtonPressedEvent e(32);

    if(e.isInCategory(EVENT_CATEGORY_INPUT)) {
        EG_TRACE(e);
    }

    while(true);
}
