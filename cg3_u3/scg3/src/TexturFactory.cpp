//
// Created by don on 21.05.19.
//
/**
 * This class Creates Textures
 * Feel free to add more Textures
 */
#include "TexturFactory.h"

TexturFactory *TexturFactory::instance;

/**
 * get Instance of TexturFactory
 * @return TexturFactory *
 */
TexturFactory *TexturFactory::getInstance() {
    if (instance == nullptr) {
        instance = new TexturFactory();
    }
    return instance;
}

/**
 * Constructor
 */
TexturFactory::TexturFactory() {
    textureFactory = TextureCoreFactory("../scg3/textures;../../scg3/textures");
}
/**
 * Creates Textur of given Name
 * @param name
 * @return Texture2DCoreSP
 */
Texture2DCoreSP TexturFactory::createTexture(const std::string &name) {
    auto texWood = textureFactory.create2DTextureFromFile(
            name, GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}

/**
 * Creates Bumb-Mapping with the Help of a NormalMap Picture
 * @param name
 * @param normalFileName
 * @return Texture2DCoreSP
 */
Texture2DCoreSP TexturFactory::createTextureBumb(const std::string &name, const std::string &normalFileName) {
    auto texWood = textureFactory.createBumpMapFromFiles(name, normalFileName, GL_REPEAT, GL_REPEAT,
                                                         GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    return texWood;
}

/**
 * Wood Texture
 * @return Texture2DCoreSP
 */
const Texture2DCoreSP &TexturFactory::getWood() {
    if (wood == nullptr) {
        wood = createTexture("wood_256.png");
    }
    return wood;
}
/**
 * Brick Texture
 * @return Texture2DCoreSP
 */
const Texture2DCoreSP &TexturFactory::getBrick() {
    if (brick == nullptr) {
        brick = createTexture("neu.png");
    }
    return brick;
}

/**
 * City Enviroment Texture
 * @return Texture2DCoreSP
 */
const Texture2DCoreSP &TexturFactory::getStadt() {
    if (stadt == nullptr) {
        stadt = createTextureBumb("riesenBild.png", "riesenBild-bumb2.png");
    }
    return stadt;
}
/**
 * Wall Texture
 * @return Texture2DCoreSP
 */
const Texture2DCoreSP &TexturFactory::getMauer() {
    if (mauer == nullptr) {
        mauer = createTextureBumb("brick_texture.png", "brick_normal.png");
    }
    return mauer;
}
/**
 * Heaven Texture
 * @return Texture2DCoreSP
 */
const Texture2DCoreSP &TexturFactory::getHimmel() {
    if (himmel == nullptr) {
        himmel = createTexture("panorama-fake-sky.png");
    }
    return himmel;
}
