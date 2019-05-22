//
// Created by don on 21.05.19.
//

#include "TexturHelper.h"

Texture2DCoreSP TexturHelper::wood;
Texture2DCoreSP TexturHelper::stadt;
Texture2DCoreSP TexturHelper::brick;
Texture2DCoreSP TexturHelper::himmel;
TextureCoreFactory TexturHelper::textureFactory("../scg3/textures;../../scg3/textures");


Texture2DCoreSP TexturHelper::createTexture(const std::string &name) {
    auto texWood = TexturHelper::textureFactory.create2DTextureFromFile(
            name, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}

Texture2DCoreSP TexturHelper::createTextureBumb(const std::string &name,const std::string &normalFileName) {
    auto texWood = TexturHelper::textureFactory.createBumpMapFromFiles(name,normalFileName,GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}

const Texture2DCoreSP &TexturHelper::getWood() {
    if(wood== nullptr) {
        wood= createTexture("wood_256.png");
    }
    return wood;
}

const Texture2DCoreSP &TexturHelper::getBrick() {
    if(brick== nullptr) {
        brick= createTexture("neu.png");
    }
    return brick;
}

const Texture2DCoreSP &TexturHelper::getStadt() {
    if(stadt== nullptr) {
        stadt = createTextureBumb("ct-map.png", "ct-map-bumb.png");
    }
    return stadt;
}

const Texture2DCoreSP &TexturHelper::getHimmel() {
    if(himmel== nullptr){
        himmel= createTexture("panorama-fake-sky.png");
    }
    return himmel;
}
