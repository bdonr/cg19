//
// Created by don on 09.06.19.
//

#include "LightFactory.h"

LightFactory* LightFactory::instance;

const LightSP &LightFactory::getSonne() {
    if (sonne == nullptr) {
        sonne = Light::create();
        sonne->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setAmbient(glm::vec4(.1, .1, .1, 1))->setSpot(glm::vec3(0, 1, 1), 100, 1)
                ->setPosition(glm::vec4(0.f, 20.f, 0, 1.f))
                ->init();
    }
    return sonne;
}

const LightSP &LightFactory::getVideoSonne() {
    if (videosonne == nullptr) {
        videosonne = Light::create();
        videosonne->setSpecular(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setDiffuse(glm::vec4(1.f, 1.f, 1.f, 1.f))
                ->setAmbient(glm::vec4(.1, .1, .1, 1))
                ->setSpot(glm::vec3(0, 1, 1), 100, 1)
                ->setPosition(glm::vec4(0.f, 20.f, 0, 1.f))
                ->init();
    }
    return videosonne;
}



const LightSP &LightFactory::getLinks() {

    if (links == nullptr) {
        links = Light::create();

        links->setSpecular(glm::vec4(1.f, 0.f, .0f, 1.f))
                ->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))
                ->setAmbient(glm::vec4(1, .4, .4, 1))
                ->setPosition(glm::vec4(0, 0, -1, 1))
                ->init();
    }
    LightPositionSP x = LightPosition::create(links);

    return links;
}

const LightSP &LightFactory::getRechts() {
    if (rechts == nullptr) {
        rechts = Light::create();
        glm::vec4 k = glm::vec4(0, 0, 1, 1);
        rechts->setSpecular(glm::vec4(0.f, .8f, .0f, 1.f))
                ->setDiffuse(glm::vec4(.1f, .1f, .1f, .1f))
                ->setAmbient(glm::vec4(.4, 4, .4, 1))
                ->setPosition(k)
                ->init();
    }
    return rechts;
}

LightFactory::LightFactory(){

}

LightFactory *LightFactory::getInstance() {
    if(instance== nullptr){
        instance=new LightFactory();
    }
    return instance;
}


