#pragma once
#include <glm/glm.hpp>

struct QNumbers { int n, l, m; };

class QMSolver {
public:
    static glm::vec3 sampleOrbitalPosition(const QNumbers& q);
    static double psi2(const QNumbers& q, double r, double theta, double phi);

private:
    static double radialWaveFunction(int n, int l, double r);
    static double assocLaguerre(int n, int k, double x);
    static double assocLegendre(int l, int m, double x);
    static double sphericalHarmonic(int l, int m, double theta, double phi);
    static double sampleRadius(int n, int l);
    static double sampleTheta(int l, int m);
};