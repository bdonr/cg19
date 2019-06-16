//
// Created by don on 21.05.19.
//

#include "MatFactory.h"



/**
 * Material Factory to create different Cores for Material
 * properties . Feel Free to add new Material Cores like you want.
 * Börries Don Rieger
 *
 * /
MatFactory* MatFactory::instance;

/**
 * Constructor
 * @return
 */

MatFactory* MatFactory::instance;
MatFactory* MatFactory::getInstance(){
    if(instance== nullptr){
        instance=new MatFactory();
    }
    return instance;
}

MatFactory::MatFactory() {

}


/**
 *
 * @param ambient
 * @param specular
 * @param diffuse
 * @param shine
 * @return MaterialCoreSP
 */
MaterialCoreSP
MatFactory::createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine) {
    auto mat = MaterialCore::create();
    mat->setAmbient(ambient)->setDiffuse(diffuse)
            ->setSpecular(specular)
            ->setShininess(shine)
            ->init();
    return mat;
}

/**
 *
 * @param ambient
 * @param specular
 * @param diffuse
 * @param shine
 * @param emission
 * @return MaterialCoreSP
 */
MaterialCoreSP
MatFactory::createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine,const glm::vec4& emission) {
    auto mat = MaterialCore::create();
    mat->setAmbient(ambient)->setDiffuse(diffuse)
            ->setSpecular(specular)
            ->setShininess(shine)->setEmission(emission)
            ->init();
    return mat;
}

/**
 * Red Material
 * @return MaterialCoreSP
 */
const MaterialCoreSP &MatFactory::getRed() {
    if(red.get()== nullptr){
        red = createMat(glm::vec4(1.f, 0.f, 0.f, 1.f), glm::vec4(1.f, 0.f, 0.f, 1.f),
                  glm::vec4(1.f, 1.f, 1.f, 1.f), .25);
    }
    return red;
}

/**
 * White Material
 * @return MaterialCoreSP
 */
const MaterialCoreSP &MatFactory::getWhite() {
    if(white.get()== nullptr){
        white = createMat(glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f),
                          glm::vec4(1.f, 1.f, 1.f, 1.f), 20.0),glm::vec4(1,1,1,1);
    }
    return white;
}
/**
 * City Material
 * @return MaterialCoreSP
 */
const MaterialCoreSP &MatFactory::getStadt() {
    if(stadt.get()== nullptr){
        stadt = createMat(glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f),
                          glm::vec4(1.f, 1.f, 1.f, 1.f), 20000.0);
    }
    return stadt;
}



const MaterialCoreSP &MatFactory::getBlack() {
    if(black.get()== nullptr){
        black = createMat(glm::vec4(0.1, 0.1, 0.1, 1), glm::vec4(0.1, 0.1, 0.1, 1), glm::vec4(1.f, 1.f, 1.f, 1.f),
                          .40);
    }
    return black;
}


/**
 * Cyan Material from http://devernay.free.fr/cours/opengl/materials.html
 * @return
 */
const MaterialCoreSP &MatFactory::getCyan() {
    if(cyan.get()== nullptr){
        cyan = createMat(glm::vec4(0.0,0.1,0.06, 1), glm::vec4(0.0,0.50980392,0.50980392, 1), glm::vec4(0.50196078,0.50196078,0.50196078, 1.f),
                          .25);
    }
    return cyan;
}

const MaterialCoreSP &MatFactory::getGold() {
    if(gold.get()== nullptr){
        gold = createMat(glm::vec4(0.24725 ,0.1995 ,0.0745, 1), glm::vec4(0.75164,0.60648,0.22648, 1), glm::vec4(0.628281,0.555802,0.366065, 1.f),
                         .4,glm::vec4(0.75164,0.60648,0.22648,1));
    }
    return gold;
}
