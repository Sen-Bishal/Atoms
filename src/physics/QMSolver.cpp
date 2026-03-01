#include "QMSolver.h"
#include <cmath>
#include <vector>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static std::mt19937 rng(std::random_device{}());
static std::uniform_real_distribution<double> uniform(0.0, 1.0);

double QMSolver::assocLaguerre(int n, int k, double x) {
    if (n == 0) return 1.0;
    if (n == 1) return 1.0 + k - x;
    double Lm1 = 1.0, L = 1.0 + k - x;
    for (int i = 2; i <= n; i++) {
        double Lp = ((2*i - 1 + k - x) * L - (i - 1 + k) * Lm1) / i;
        Lm1 = L; L = Lp;
    }
    return L;
}

double QMSolver::assocLegendre(int l, int m, double x) {
    m = abs(m);
    double pmm = 1.0;
    if (m > 0) {
        double sx = sqrt(1.0 - x*x);
        for (int i = 1; i <= m; i++) pmm *= -(2*i - 1) * sx;
    }
    if (l == m) return pmm;
    double pmm1 = x * (2*m + 1) * pmm;
    if (l == m + 1) return pmm1;
    double pll = 0;
    for (int ll = m+2; ll <= l; ll++) {
        pll = ((2*ll - 1) * x * pmm1 - (ll + m - 1) * pmm) / (ll - m);
        pmm = pmm1; pmm1 = pll;
    }
    return pll;
}

double QMSolver::radialWaveFunction(int n, int l, double r) {
    double a0  = 1.0;
    double rho = 2.0 * r / (n * a0);
    double norm = sqrt(pow(2.0/(n*a0), 3) *
                  tgamma(n-l) / (2.0*n * tgamma(n+l+1)));
    return norm * exp(-rho/2.0) * pow(rho, l) * assocLaguerre(n-l-1, 2*l+1, rho);
}

double QMSolver::sphericalHarmonic(int l, int m, double theta, double phi) {
    double norm = sqrt((2*l+1)/(4.0*M_PI) * tgamma(l-abs(m)+1)/tgamma(l+abs(m)+1));
    double leg  = assocLegendre(l, m, cos(theta));
    if      (m > 0) return norm * sqrt(2.0) * cos( m * phi) * leg;
    else if (m < 0) return norm * sqrt(2.0) * sin(-m * phi) * leg;
    else            return norm * leg;
}

double QMSolver::psi2(const QNumbers& q, double r, double theta, double phi) {
    double R = radialWaveFunction(q.n, q.l, r);
    double Y = sphericalHarmonic(q.l, q.m, theta, phi);
    return R * R * Y * Y * r * r * sin(theta);
}

double QMSolver::sampleRadius(int n, int l) {
    const int BINS = 1000;
    double rMax = n * n * 5.0;
    double dr   = rMax / BINS;
    std::vector<double> cdf(BINS);
    double sum = 0;
    for (int i = 0; i < BINS; i++) {
        double r = (i + 0.5) * dr;
        double R = radialWaveFunction(n, l, r);
        sum += R * R * r * r * dr;
        cdf[i] = sum;
    }
    for (auto& v : cdf) v /= sum;
    double u = uniform(rng);
    for (int i = 0; i < BINS; i++)
        if (cdf[i] >= u) return (i + 0.5) * dr;
    return rMax;
}

double QMSolver::sampleTheta(int l, int m) {
    const int BINS = 500;
    std::vector<double> cdf(BINS);
    double sum = 0;
    for (int i = 0; i < BINS; i++) {
        double theta = (i + 0.5) * M_PI / BINS;
        double Y = sphericalHarmonic(l, m, theta, 0.0);
        sum += Y * Y * sin(theta) * (M_PI / BINS);
        cdf[i] = sum;
    }
    for (auto& v : cdf) v /= sum;
    double u = uniform(rng);
    for (int i = 0; i < BINS; i++)
        if (cdf[i] >= u) return (i + 0.5) * M_PI / BINS;
    return M_PI;
}

glm::vec3 QMSolver::sampleOrbitalPosition(const QNumbers& q) {
    double r     = sampleRadius(q.n, q.l);
    double theta = sampleTheta(q.l, q.m);
    double phi   = uniform(rng) * 2.0 * M_PI;
    return glm::vec3(
        (float)(r * sin(theta) * cos(phi)),
        (float)(r * cos(theta)),
        (float)(r * sin(theta) * sin(phi))
    );
}