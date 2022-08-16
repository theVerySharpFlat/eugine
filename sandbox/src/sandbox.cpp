//
// Created by aiden on 8/2/21.
//

#include <eugine.h>
#include <imgui/imgui.h>

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
        // eg::trace(event);
    }

    void onImGuiRender() override {
    }

};


class Sandbox : public eg::Application{
public:
    Sandbox() {
    }

    void init() override {
        pushLayer(new TestLayer);
    }

    ~Sandbox(){

    }
};

eg::Application* eg::createApplication() {
    return new Sandbox();
}
