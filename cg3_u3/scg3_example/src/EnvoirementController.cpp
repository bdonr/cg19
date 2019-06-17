//
// Created by don on 21.05.19.
//

#include <iostream>
#include "SceneObjetFactory.h"
#include "GameLogic.h"
#include "LightFactory.h"

EnvoirementController* EnvoirementController::controllerInstance;

void EnvoirementController::createStandartScene(ViewerSP &viewer, CameraSP camera, GroupSP scene) {


    // camera controllers
    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);
    GameLogic::logic(camera, insta->createBullet(),viewer);
    camera->addChild(insta->createBullet());
   // controller->setBullet(insta->createBullet());
    scene->addChild(lightFactory->getPrimaryLight());
    scene->addChild(insta->getSky());


    lightFactory->getPrimaryLight()->addChild(insta->getFloor());
    lightFactory->getPrimaryLight()->addChild(lightFactory->getLeftLight());
    lightFactory->getLeftLight()->addChild(lightFactory->getRightLight());
    lightFactory->getRightLight()->addChild(insta->getTower());
    lightFactory->getLeftLight()->addChild(insta->getTower());


    for (int i = 0; i < SceneObjetFactory::getInstance(viewer)->getTargets().size(); i++) {
        lightFactory->getPrimaryLight()->addChild(insta->getTargets().at(i));
    }
    lightFactory->getPrimaryLight()->addChild(camera);
    camera->addChild(insta->getCamObject());

}

void EnvoirementController::createShowScene(ViewerSP &viewer, CameraSP camera, GroupSP scene) {


    camera->translate(glm::vec3(0.f, 1.5f, -9.f))->rotate(180, glm::vec3(0.f, 1.f, 0.f))
            ->dolly(-1.f);


    //Licht in die Scene stellen
    scene->addChild(lightFactory->getVideoLight());

    //camera ins licht stellen
    lightFactory->getVideoLight()->addChild(camera);

    //Transformationen ins licht stellen

    lightFactory->getVideoLight()->addChild(insta->createJetGroup());

    lightFactory->getVideoLight()->addChild(insta->createJetGroup2());
    lightFactory->getVideoLight()->addChild(insta->getVideoFloor());
    lightFactory->getVideoLight()->addChild(insta->getVideoSky());


}

EnvoirementController::EnvoirementController(ViewerSP &viewer) {
    insta = SceneObjetFactory::getInstance(viewer);
    //Licht holen
    lightFactory = LightFactory::getInstance();
}


 EnvoirementController* EnvoirementController::getControllerInstance(ViewerSP &viewer){
    if(controllerInstance== nullptr){
        controllerInstance=new EnvoirementController(viewer);
    }
     return controllerInstance;
}

