//
// Created by don on 21.05.19.
//

#include "MatFactory.h"

MaterialCoreSP MatFactory::red= nullptr;
MaterialCoreSP MatFactory::white= nullptr;
MaterialCoreSP MatFactory::black= nullptr;
MaterialCoreSP MatFactory::tag= nullptr;
MaterialCoreSP MatFactory::stadt= nullptr;

MaterialCoreSP
MatFactory::createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine) {
    auto mat = MaterialCore::create();
    mat->setAmbient(ambient)->setDiffuse(diffuse)
            ->setSpecular(specular)
            ->setShininess(shine)
            ->init();
    return mat;
}
MaterialCoreSP
MatFactory::createMat(const glm::vec4 &ambient, const glm::vec4 &specular, const glm::vec4 &diffuse, const float &shine,const glm::vec4& emission) {
    auto mat = MaterialCore::create();
    mat->setAmbient(ambient)->setDiffuse(diffuse)
            ->setSpecular(specular)
            ->setShininess(shine)->setEmission(emission)
            ->init();
    return mat;
}

const MaterialCoreSP &MatFactory::getRed() {
    if(red.get()== nullptr){
        red = createMat(glm::vec4(1.f, 0.5f, 0.5f, 1.f), glm::vec4(1.f, 0.5f, 0.5f, 1.f),
                  glm::vec4(1.f, 1.f, 1.f, 1.f), 20.0);
    }
    return red;
}


const MaterialCoreSP &MatFactory::getWhite() {
    if(white.get()== nullptr){
        white = createMat(glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f),
                          glm::vec4(1.f, 1.f, 1.f, 1.f), 20.0),glm::vec4(1,1,1,1);
    }
    return white;
}const MaterialCoreSP &MatFactory::getStadt() {
    if(stadt.get()== nullptr){
        stadt = createMat(glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f),
                          glm::vec4(1.f, 1.f, 1.f, 1.f), 20.0);
    }
    return stadt;
}


const MaterialCoreSP &MatFactory::getBlack() {
    if(black.get()== nullptr){
        black = createMat(glm::vec4(0, 0, 0, 0), glm::vec4(0, 0, 0, 0), glm::vec4(1.f, 1.f, 1.f, 1.f),
                          20.0);
    }
    return black;
}


const MaterialCoreSP &MatFactory::getTag() {
    if(tag.get()== nullptr){
        tag = createMat(glm::vec4(0.f, .7f, 1.f, 1), glm::vec4(1.f, 1.f, 1.f, 1.f), glm::vec4(1.f, 1.f, 1.f, 1.f),
                        200000,glm::vec4(0.f,0.f,.0f,.1));
    }
    return tag;
}
