//
// Created by don on 21.05.19.
//

#include "TexturFactory.h"

TexturFactory *TexturFactory::instance;

TexturFactory *TexturFactory::getInstance() {
    if (instance == nullptr) {
        instance = new TexturFactory();
    }
    return instance;
}

TexturFactory::TexturFactory() {
    textureFactory = TextureCoreFactory("../scg3/textures;../../scg3/textures");
}

Texture2DCoreSP TexturFactory::createTexture(const std::string &name) {
    auto texWood = textureFactory.create2DTextureFromFile(
            name, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}

Texture2DCoreSP TexturFactory::createTextureBumb(const std::string &name, const std::string &normalFileName) {
    auto texWood = textureFactory.createBumpMapFromFiles(name, normalFileName, GL_REPEAT, GL_REPEAT,
                                                         GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}

const Texture2DCoreSP &TexturFactory::getWood() {
    if (wood == nullptr) {
        wood = createTexture("wood_256.png");
    }
    return wood;
}

const Texture2DCoreSP &TexturFactory::getBrick() {
    if (brick == nullptr) {
        brick = createTexture("neu.png");
    }
    return brick;
}

const Texture2DCoreSP &TexturFactory::getStadt() {
    if (stadt == nullptr) {
        stadt = createTextureBumb("riesenBild.png", "riesenBild-bumb2.png");
    }
    return stadt;
}

const Texture2DCoreSP &TexturFactory::getMauer() {
    if (mauer == nullptr) {
        mauer = createTextureBumb("brick_texture.png", "brick_normal.png");
    }
    return mauer;
}

const Texture2DCoreSP &TexturFactory::getHimmel() {
    if (himmel == nullptr) {
        himmel = createTexture("panorama-fake-sky.png");
    }
    return himmel;
}
