//
// Created by don on 24.05.19.
//

#include <iostream>
#include "GameLogic.h"


/**
 * Basic Game Logic 
 * @param camera
 * @param bulletTrans
 * @param viewer
 */

double GameLogic::bulletTravel =0;
void GameLogic::logic(CameraSP &camera, TransformationSP &bulletTrans, ViewerSP& viewer) {

    auto TransAni = TransformAnimation::create();
    auto camObjectTrans = Transformation::create();
    bulletTrans->translate(glm::vec3(0.00f, -0.05f, -0.2f));
    TransAni->setUpdateFunc(
            [camera, bulletTrans, camObjectTrans,&viewer](
                    TransformAnimation *anim, double currTime, double diffTime, double totalTime) {
/* die projektile bewegen sich die ganze zeit, sie laufen vorwärts und springen dan beim errreichen
 * der maximal distanz zurück, die kugeln sind daurhaft unsichbar nur beim drücken von SPACE werden
 * diese sichtbar und können interagieren
 /            std::cout<<camera->getMatrix()[3][0];
             std::cout<<camera->getMatrix()[3][1]<<;
             std::cout<<camera->getMatrix()[3][2];
 **/            bulletTrans->translate(glm::vec3(0, 0., -0.02));
                bulletTravelAndTest();
                if (bulletTravel > 24.8) {
                    bulletTrans->translate(glm::vec3(0, 0, 2.5));
                }
 /* 0.1 * 5 = 0.5 * 5 = 2.5 rechung für den zurücksprung
 */
/*kolision detection beim rammen des flugzeugs gegen die objekte*/
                glm::mat4 tempCamObjMat = camera->getMatrix();
                tempCamObjMat = glm::translate(tempCamObjMat, glm::vec3(0, -0.08, -0.2));

                glm::vec3 camObjPos = glm::vec3(tempCamObjMat[3][0], tempCamObjMat[3][1], tempCamObjMat[3][2]);


                checkDurchflugZielscheibe(camObjPos, totalTime,viewer);


                /*kolision detection beim treffen der projektile auf objecte*/
                glm::mat4 tempBulletMat = camera->getMatrix();
                tempBulletMat *= bulletTrans->getMatrix();

                float bulletX = tempBulletMat[3][0];
                float bulletY = tempBulletMat[3][1];
                float bulletZ = tempBulletMat[3][2];

                glm::vec3 bullObjPos = glm::vec3(bulletX, bulletY, bulletZ);
                if(bulletTrans->isVisible()) {
                    checkBulletTreffer(bullObjPos, totalTime, viewer);
                }

                if (camera->getPosition().x > 15) {
                    //camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                    camera->translate(glm::vec3(0.0f, 0.0f, 0.15f));
                } else if (camera->getPosition().x < -15) {
                    //camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                    camera->translate(glm::vec3(0.0f, 0.0f, 0.15f));
                } else if (camera->getPosition().z > 15) {
                    //camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                    camera->translate(glm::vec3(0.0f, 0.0f, 0.15f));
                } else if (camera->getPosition().z < -15) {
                    //camera->rotate(15.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                    camera->translate(glm::vec3(0.0f, 0.0f, 0.15f));
                } else if (camera->getPosition().y < 1.0) {
                    //camera->rotate(-15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                    camera->translate(glm::vec3(0.0f, 0.0f, 0.15f));
                } else if (camera->getPosition().y > 10.0) {
                    //  camera->rotate(15.0f, glm::vec3(1.0f, 0.f, 0.0f));
                    camera->translate(glm::vec3(0.0f, 0.0f, 0.15f));
                }



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
/**
 * Check if jet Flought through the Target
 * @param camObjPos
 * @param time
 */
void GameLogic::checkDurchflugZielscheibe(const glm::vec3 &camObjPos,double time, ViewerSP& viewer) {
    double rad1 = 0.1;
    double rad2 = 0.1401;
    SceneObjetFactory * insta = SceneObjetFactory::getInstance(viewer);
    for (int i = 0; i < insta->getTargets().size(); i++) {
        glm::vec3 kk = glm::vec3(insta->getTargets()[i]->getMatrix()[3][0],
                                 insta->getTargets()[i]->getMatrix()[3][1],
                                 insta->getTargets()[i]->getMatrix()[3][2]);
        float diff1 = sqrt(pow(camObjPos.x - kk.x, 2) + pow(camObjPos.y - kk.y, 2) + pow(camObjPos.z - kk.z, 2));
        if (insta->getTargets()[i]->isVisible()) {
            if (glm::abs(rad1 - rad2) < diff1 && diff1 < (rad1 + rad2)) {
                insta->getTargets()[i]->setVisible(false);
                insta->getTargets()[i + 1]->setVisible(true);
            }
        }

    }
}
/**
 * Check if bullet  Flought through the Target
 * @param camObjPos
 * @param time
 */
void GameLogic::checkBulletTreffer(const glm::vec3 &bullet, double time,ViewerSP& viewer) {
    double rad1 = .1;
    double rad2 = .1;
    SceneObjetFactory * insta = SceneObjetFactory::getInstance(viewer);
    for (int i = 0; i < insta->getTargets().size(); i++) {

        glm::vec3 kk = glm::vec3(insta->getTargets()[i]->getMatrix()[3][0],
                                 insta->getTargets()[i]->getMatrix()[3][1],
                                 insta->getTargets()[i]->getMatrix()[3][2]);
        float diff1 = sqrt(pow(bullet.x - kk.x, 2) + pow(bullet.y - kk.y, 2) + pow(bullet.z - kk.z, 2));
        if (insta->getTargets()[i]->isVisible()) {
            if (glm::abs(rad1 - rad2) < diff1 && diff1 < (rad1 + rad2)) {
                insta->getTargets()[i]->setVisible(false);
                insta->getTargets()[i + 1]->setVisible(true);
            }
        }

    }
}

void GameLogic::bulletTravelAndTest() {
    if (bulletTravel > 24.8) {
        bulletTravel = 0;
    }
    bulletTravel = bulletTravel + 0.2;


}

