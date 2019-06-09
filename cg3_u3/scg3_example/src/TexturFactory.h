//
// Created by don on 21.05.19.
//

#ifndef CG3_U3_TEXTURFACTORY_H
#define CG3_U3_TEXTURFACTORY_H
#include "scg3.h"

using namespace scg;
class TexturFactory {
private:

     Texture2DCoreSP wood;
     Texture2DCoreSP brick;
     Texture2DCoreSP stadt;
    CubeMapCoreSP himmel;
     Texture2DCoreSP mauer;
     TextureCoreFactory textureCoreFactory;
     static TexturFactory* instance;
    TexturFactory();
public:
    static TexturFactory* getInstance();
     const Texture2DCoreSP &getWood();

     const Texture2DCoreSP &getBrick();

     const Texture2DCoreSP &getStadt();

     const CubeMapCoreSP & getHimmel();
     const Texture2DCoreSP &getMauer();

private:
     Texture2DCoreSP createTexture(const std::string &name);
     Texture2DCoreSP createTextureBumb(const std::string &name,const std::string &normalFileName);

};


#endif //CG3_U3_TEXTURFACTORY_H
