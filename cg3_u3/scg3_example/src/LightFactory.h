//
// Created by don on 09.06.19.
//

#ifndef CG3_U3_LIGHTFACTORY_H
#define CG3_U3_LIGHTFACTORY_H

#include "scg3.h"
using namespace scg;

class LightFactory {
private:
    LightSP sonne;
    LightSP videosonne;
    LightSP links;
    LightSP rechts;
    LightSP videoSonne2;

    static LightFactory *instance;

public:
    const LightSP& getRechts();
    const LightSP& getLinks();
    const LightSP& getVideoSonne();
    const LightSP& getSonne();
    const LightSP &getVideoSonne2();

private:
public:
    LightFactory();

    static LightFactory *getInstance();
};


#endif //CG3_U3_LIGHTFACTORY_H
