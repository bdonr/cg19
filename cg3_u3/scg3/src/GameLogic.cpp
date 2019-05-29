//
// Created by don on 24.05.19.
//

#include <iostream>
#include "GameLogic.h"

double GameLogic::bulletTravel = 0;

void GameLogic::logic(CameraSP &camera,TransformationSP &bulletTrans, ViewerSP viewer) {
    bulletTrans = Transformation::create();
    auto TransAni = TransformAnimation::create();
    auto camObjectTrans = Transformation::create();
    bulletTrans->translate(glm::vec3(0.02f, -0.08f, -0.2f));
    TransAni->setUpdateFunc(
            [camera, bulletTrans, camObjectTrans](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
/* die projektile bewegen sich die ganze zeit, sie laufen vorwärts und springen dan beim errreichen
 * der maximal distanz zurück, die kugeln sind daurhaft unsichbar nur beim drücken von SPACE werden
 * diese sichtbar und können interagieren
 /            std::cout<<camera->getMatrix()[3][0];
             std::cout<<camera->getMatrix()[3][1]<<;
             std::cout<<camera->getMatrix()[3][2];
 **/               bulletTrans->translate(glm::vec3(0, 0., -0.02));
                bulletTravelAndTest();
                if (bulletTravel > 25) {
                    bulletTrans->translate(glm::vec3(0, 0, 2.5));
/*
 * 0.1 * 5 = 0.5 * 5 = 2.5 rechung für den zurücksprung
 */
                }
/*kolision detection beim rammen des flugzeugs gegen die objekte*/
                glm::mat4 tempCamObjMat = camera->getMatrix();
                tempCamObjMat = glm::translate(tempCamObjMat, glm::vec3(0, -0.08, -0.2));

                glm::vec3 camObjPos = glm::vec3(tempCamObjMat[3][0], tempCamObjMat[3][1], tempCamObjMat[3][2]);


                checkDurchflugZielscheibe(camObjPos, totalTime);


                /*kolision detection beim treffen der projektile auf objecte*/
                glm::mat4 tempBulletMat = camera->getMatrix();
                tempBulletMat *= bulletTrans->getMatrix();

                float bulletX = tempBulletMat[3][0];
                float bulletY = tempBulletMat[3][1];
                float bulletZ = tempBulletMat[3][2];

                glm::vec3 bullObjPos = glm::vec3(bulletX, bulletY, bulletZ);
                checkBulletTreffer(bullObjPos, totalTime);



                /**        if (camera->getPosition().x > 15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().x < -15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().z > 15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().z < -15) {
                            camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                        } else if (camera->getPosition().y < 1.0) {
                            camera->rotate(-15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                        } else if (camera->getPosition().y > 10.0) {
                            camera->rotate(15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                        }
        **/
                /*
                 * interesant: die bewegung der kamera ist spiegelverkehrt wegen dem blick in die negative Z-Achse
                 * also bewegt sich die camera nach -Z
                 * aber
                 * auch die geschwindigkeit der transformation ist anders etwa halb so stark
                 * die rotation skalierung ist gleich allerdings richtet sich die cammere nach den weltcoordinaten aus
                 * und die objekte nach ihren eigenen UND die rations richtung ist spiegelverkehrt.
                 */
                // camera->rotate(-0.7f, glm::vec3(1.0f, 0.0f, 0.0f))->translate(glm::vec3(0, 0, -0.001));


                // std::cout << totalTime << std::endl;

            });
    viewer->addAnimation(TransAni);

}

void GameLogic::checkDurchflugZielscheibe(const glm::vec3 &camObjPos, double time) {
    double rad1 = 0.2;
    double rad2 = 0.3;
    for (int i = 0; i < SceneObjetFactory::getZielscheiben().size(); i++) {
        glm::vec3 kk = glm::vec3(SceneObjetFactory::getZielscheiben()[i]->getMatrix()[3][0],
                                 SceneObjetFactory::getZielscheiben()[i]->getMatrix()[3][1],
                                 SceneObjetFactory::getZielscheiben()[i]->getMatrix()[3][2]);
        float diff1 = sqrt(pow(camObjPos.x - kk.x, 2) + pow(camObjPos.y - kk.y, 2) + pow(camObjPos.z - kk.z, 2));
        if (SceneObjetFactory::getZielscheiben()[i]->isVisible()) {
            if (glm::abs(rad1 - rad2) < diff1 && diff1 < (rad1 + rad2)) {
                SceneObjetFactory::getZielscheiben()[i]->setVisible(false);
                SceneObjetFactory::getZielscheiben()[i + 1]->setVisible(true);
            }
        }

    }
}

void GameLogic::checkBulletTreffer(const glm::vec3 &bullet, double time) {
    double rad1 = 0.1;
    double rad2 = 0.1;
    for (int i = 0; i < SceneObjetFactory::getZielscheiben().size(); i++) {

        glm::vec3 kk = glm::vec3(SceneObjetFactory::getZielscheiben()[i]->getMatrix()[3][0],
                                 SceneObjetFactory::getZielscheiben()[i]->getMatrix()[3][1],
                                 SceneObjetFactory::getZielscheiben()[i]->getMatrix()[3][2]);
        float diff1 = sqrt(pow(bullet.x - kk.x, 2) + pow(bullet.y - kk.y, 2) + pow(bullet.z - kk.z, 2));
        if (SceneObjetFactory::getZielscheiben()[i]->isVisible()) {
            if (glm::abs(rad1 - rad2) < diff1 && diff1 < (rad1 + rad2)) {
                SceneObjetFactory::getZielscheiben()[i]->setVisible(false);
                SceneObjetFactory::getZielscheiben()[i + 1]->setVisible(true);
            }
        }

    }
}

void GameLogic::bulletTravelAndTest() {
    if (bulletTravel > 25) {
        bulletTravel = 0;
    }
    bulletTravel = bulletTravel + 0.2;


}

