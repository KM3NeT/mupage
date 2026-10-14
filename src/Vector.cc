/*#############################################################
 *#                                                           #
 *#    Author: C. Distefano                                   #
 *#                                                           #
 *#    Date: 10 May 2021                                      #
 *#    Copied from antcc Geometry.cc                          #
 *#    Allow to compile mupage without antcc                  #
 *#    compiled only if mupage is not linked to antcc         #
 *#                                                           #
 *#############################################################
 */

#include "Vector.hh"
using namespace std;

#ifdef ROOTSUPPORT
ClassImp(Vec3D)
#endif

    Vec3D::Vec3D(double xx, double yy, double zz)
    : x(xx), y(yy), z(zz) {
}
