//
// Created by don on 21.05.19.
//

#include "ShaderFactory.h"

/**
 * This class creates diffrent Shader
 * with and without textures and more
 * feel Free to add some of your own shader
 */
ShaderFactory* ShaderFactory::instance;

ShaderFactory::ShaderFactory(){
    shaderFactory= ShaderCoreFactory("../scg3/shaders;../../scg3/shaders");
}
/**
 * get instance of ShaderFactory
 * @return ShaderFactory*
 */
ShaderFactory* ShaderFactory::getInstance() {
    if(instance== nullptr){
        instance=new ShaderFactory();
    }
    return instance;
}
/**
 *  Creates a Phong shader with or without texturemode
 * @param texturmode
 * @return ShaderCoreSP
 */
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

/**
 * Creates a phong based bumb shader
 * @return ShaderCoreSP
 */
const ShaderCoreSP &ShaderFactory::getPhongBumb() {

    if (phongbumb == nullptr) {
        phongbumb = create("bump","bump");
    }
        return phongbumb;
    }

/**
 *  Creates a Gouraud shader with or without texturemode
 * @param texturmode
 * @return ShaderCoreSP
 */
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
/**
 * A Test Shader with invertet Vertex normals
 * @param texturmode
 * @return
 */
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
/**
 * Create a skybox shader
 * @param texturmode
 * @return
 */
const ShaderCoreSP &ShaderFactory::getSkybox(bool texturmode) {
    return create("bump",true);
}
/**
 * Creates Shader by params
 * @param name
 * @param texturemode
 * @return
 */
const ShaderCoreSP ShaderFactory::create(const std::string &name, bool texturemode) {
    return shaderFactory.createShaderFromSourceFiles({
                                                             ShaderFile(name+"_vert.glsl", GL_VERTEX_SHADER),
                                                             ShaderFile(name+"_frag.glsl", GL_FRAGMENT_SHADER),
                                                             ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                                                             (texturemode) ? ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER) : ShaderFile("texture_none.glsl", GL_FRAGMENT_SHADER)
    });

}
/**
 * Creates Shader by Two different params
 * @param name
 * @param texturemode
 * @return
 */
const ShaderCoreSP ShaderFactory::create(const std::string &name1,const std::string &name2) {
    return shaderFactory.createShaderFromSourceFiles({
                                                             ShaderFile(name1+"_vert.glsl", GL_VERTEX_SHADER),
                                                             ShaderFile(name2+"_frag.glsl", GL_FRAGMENT_SHADER),
                                                             ShaderFile("blinn_phong_lighting.glsl", GL_FRAGMENT_SHADER),
                                                             ShaderFile("texture2d_modulate.glsl", GL_FRAGMENT_SHADER)
                                                     });

}