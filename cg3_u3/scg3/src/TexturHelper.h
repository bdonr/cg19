//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_TEXTURHELPER_H
#define CG3_U3_TEXTURHELPER_H
#include "../scg3/scg3.h"
using namespace scg;
class TexturHelper {
private:

    static Texture2DCoreSP wood;
    static Texture2DCoreSP brick;
    static Texture2DCoreSP stadt;
    static Texture2DCoreSP himmel;
    static TextureCoreFactory textureFactory;

public:
    static const Texture2DCoreSP &getWood();

    static const Texture2DCoreSP &getBrick();

    static const Texture2DCoreSP &getStadt();

    static const Texture2DCoreSP &getHimmel();

private:
    static Texture2DCoreSP createTexture(const std::string &name);
    static Texture2DCoreSP createTextureBumb(const std::string &name,const std::string &normalFileName);

};


#endif //CG3_U3_TEXTURHELPER_H
