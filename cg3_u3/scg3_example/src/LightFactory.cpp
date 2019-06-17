//
// Created by don on 09.06.19.
//

#include "LightFactory.h"

LightFactory *LightFactory::instance;

const LightSP &LightFactory::getPrimaryLight() {
    if (primary == nullptr) {
        primary = Light::create();
        primary->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setAmbient(glm::vec4(.1, .1, .1, 1))->setSpot(glm::vec3(0, 1, 1), 100, 1)
                ->setPosition(glm::vec4(0.f, 20.f, 0, 1.f))
                ->init();
    }
    return primary;
}

const LightSP &LightFactory::getVideoLight() {
    if (videosun == nullptr) {
        videosun = Light::create();
        videosun->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setAmbient(glm::vec4(.1, .1, .1, 1))
                ->setSpot(glm::vec3(0, 1, 1), 100, 1)
                ->setPosition(glm::vec4(20.f, 20.f, 0, 1.f))
                ->init();
    }
    return videosun;
}


const LightSP &LightFactory::getLeftLight() {

    if (left == nullptr) {
        left = Light::create();

        left->setSpecular(glm::vec4(1.f, 0.f, .0f, 1.f))
                ->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))
                ->setAmbient(glm::vec4(1, .4, .4, 1))
                ->setPosition(glm::vec4(-3, 0, 0, 1))
                ->init();
    }
    LightPositionSP x = LightPosition::create(left);

    return left;
}

const LightSP &LightFactory::getRightLight() {
    if (right == nullptr) {
        right = Light::create();
        glm::vec4 k = glm::vec4(3, 0, 0, 1);
        right->setSpecular(glm::vec4(0.f, .08f, .0f, 1.f))
                ->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))
                ->setAmbient(glm::vec4(.4, .4, .4, 1))
                ->setPosition(k)
                ->init();
    }
    return right;
}

LightFactory::LightFactory() {

}

LightFactory *LightFactory::getInstance() {
    if (instance == nullptr) {
        instance = new LightFactory();
    }
    return instance;
}


