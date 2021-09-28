//
// Created by aiden on 8/2/21.
//

#include <eugine.h>
#include <iostream>

class TestLayer : public eg::Layer {

public:
    TestLayer() : eg::Layer("TestLayer") {

    }

    ~TestLayer() override {

    }

    void onAttach() override {

    }

    void onDetach() override {

    }

    void onUpdate() override {
        //EG_INFO("TestLayer -> ON UPDATE");

    }

    void onEvent(eg::Event& event) override {
        //EG_TRACE("{0}", event);
    }

};


class Sandbox : public eg::Application{
public:
    Sandbox() {
    }

    void init() override {
        pushLayer(new TestLayer);
        pushOverlay(new eg::ImGuiLayer);
    }

    ~Sandbox(){

    }
};

eg::Application* eg::createApplication() {
    return new Sandbox();
}
