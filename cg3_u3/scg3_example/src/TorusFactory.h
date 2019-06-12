// TorusFactory.h
#ifndef TORUSFACTORY_H
#define TORUSFACTORY_H
#include "src/GeometryCoreFactory.h"
#include "src/GeometryCore.h"
#include "src/scg_utilities.h"

namespace scg {

class TorusFactory: public GeometryCoreFactory {
public:

	TorusFactory();

	/**
	 * Creates a Torus with given parameters
	 * @param torusRadius
	 * @param innerRadius
	 * @param torusTeile
	 * @param kreisTeile
	 * @return
	 */
	GeometryCoreSP createTorus(GLfloat torusRadius, GLfloat innerRadius, int torusTeile, int kreisTeile);

	virtual ~TorusFactory();

private:
};
}

#endif
