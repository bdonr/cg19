//
// Created by don on 21.05.19.
//

#include "ShaderFactory.h"


ShaderCoreSP ShaderFactory::phongwith;
ShaderCoreSP ShaderFactory::phongwithout;
ShaderCoreSP ShaderFactory::gauradwith;
ShaderCoreSP ShaderFactory::gauradwithout;
ShaderCoreSP ShaderFactory::phongreverse;
ShaderCoreSP ShaderFactory::phongreversewithout;
ShaderCoreSP ShaderFactory::color;
ShaderCoreSP ShaderFactory::skybox;
ShaderCoreFactory ShaderFactory::shaderFactory("../scg3/shaders;../../scg3/shaders");

const ShaderCoreSP &ShaderFactory::getPhong(bool texturmode) {

    if (phongwith == nullptr && texturmode) {
        phongwith = create("phong", texturmode);
    }
    if (phongwithout == nullptr && !texturmode) {
        phongwithout = create("phong", texturmode);
    }
    if (texturmode) {
        return phongwith;
    } else {
        return phongwithout;
    }
}

const ShaderCoreSP &ShaderFactory::getGaurad(bool texturmode) {
    if (gauradwith == nullptr && texturmode) {
        gauradwith = create("gouraud", texturmode);
    }
    if (gauradwithout == nullptr && !texturmode) {
        gauradwithout = create("gouraud", texturmode);
    }
    if (texturmode) {
        return gauradwith;
    } else {
        return gauradwithout;
    }
}

const ShaderCoreSP &ShaderFactory::getPhongreverse(bool texturmode) {

    if (phongreverse == nullptr && texturmode) {
        phongreverse = create("phong_inner", texturmode);
    }
    if (phongreversewithout == nullptr && !texturmode) {
        phongreversewithout = create("phong_inner", texturmode);
    }
    if (texturmode) {
        return phongreverse;
    } else {
        return phongreversewithout ;
    }
}
const ShaderCoreSP &ShaderFactory::getColor() {
    return color;
}

const ShaderCoreSP &ShaderFactory::getSkybox(bool texturmode) {
    return create("bump",true);
}

const ShaderCoreSP ShaderFactory::create(const std::string &name, bool texturemode) {
    return shaderFactory.createShaderFromSourceFiles({
                                                             ShaderFile(name+"_vert.glsl", GL_VERTEX_SHADER),
                                                             ShaderFile(name+"_frag.glsl", GL_FRAGMENT_SHADER),
                                                             ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                                                             (texturemode) ? ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER) : ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER)
    });

}
