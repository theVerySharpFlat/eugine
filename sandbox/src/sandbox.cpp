//
// Created by aiden on 8/2/21.
//

#include <eugine.h>
#include <iostream>


class Sandbox : public eg::Application{
public:
    Sandbox() {
    }

    ~Sandbox(){

    }
};

eg::Application* eg::createApplication() {
    return new eg::Application();
}
