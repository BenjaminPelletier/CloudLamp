#ifndef Atmosphere_h
#define Atmosphere_h

// https://www.scratchapixel.com/lessons/procedural-generation-virtual-worlds/simulating-sky/simulating-colors-of-the-sky

const float kInfinity = std::numeric_limits<float>::max(); 
 
template<typename T> 
class Vec3 
{ 
public: 
    Vec3() : x(0), y(0), z(0) {} 
    Vec3(T xx) : x(xx), y(xx), z(xx) {} 
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {} 
    Vec3 operator * (const T& r) const { return Vec3(x * r, y * r, z * r); } 
    Vec3 operator * (const Vec3<T> &v) const { return Vec3(x * v.x, y * v.y, z * v.z); } 
    Vec3 operator + (const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); } 
    Vec3 operator - (const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); } 
    template<typename U> 
    Vec3 operator / (const Vec3<U>& v) const { return Vec3(x / v.x, y / v.y, z / v.z); } 
    friend Vec3 operator / (const T r, const Vec3& v) 
    { 
        return Vec3(r / v.x, r / v.y, r / v.z); 
    } 
    const T& operator [] (size_t i) const { return (&x)[i]; } 
    T& operator [] (size_t i) { return (&x)[i]; } 
    T length2() const { return x * x + y * y + z * z; } 
    T length() const { return std::sqrt(length2()); } 
    Vec3& operator += (const Vec3<T>& v) { x += v.x, y += v.y, z += v.z; return *this; } 
    Vec3& operator *= (const float& r) { x *= r, y *= r, z *= r; return *this; } 
    friend Vec3 operator * (const float&r, const Vec3& v) 
    { 
        return Vec3(v.x * r, v.y * r, v.z * r); 
    }
    T x, y, z; 
}; 
 
template<typename T> 
void normalize(Vec3<T>& vec) 
{ 
    T len2 = vec.length2(); 
    if (len2 > 0) { 
        T invLen = 1 / std::sqrt(len2); 
        vec.x *= invLen, vec.y *= invLen, vec.z *= invLen; 
    } 
} 
 
template<typename T> 
T dot(const Vec3<T>& va, const Vec3<T>& vb) 
{ 
    return va.x * vb.x + va.y * vb.y + va.z * vb.z; 
} 
 
using Vec3f = Vec3<float>; 

bool solveQuadratic(float a, float b, float c, float& x1, float& x2) 
{ 
    if (b == 0) { 
        // Handle special case where the the two vector ray.dir and V are perpendicular
        // with V = ray.orig - sphere.centre
        if (a == 0) return false; 
        x1 = 0; x2 = sqrtf(-c / a); 
        return true; 
    } 
    float discr = b * b - 4 * a * c; 
 
    if (discr < 0) return false; 
 
    float q = (b < 0.f) ? -0.5f * (b - sqrtf(discr)) : -0.5f * (b + sqrtf(discr)); 
    x1 = q / a; 
    x2 = c / q; 
 
    return true; 
} 

bool raySphereIntersect(const Vec3f& orig, const Vec3f& dir, const float& radius, float& t0, float& t1) 
{ 
    // They ray dir is normalized so A = 1 
    float A = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z; 
    float B = 2 * (dir.x * orig.x + dir.y * orig.y + dir.z * orig.z); 
    float C = orig.x * orig.x + orig.y * orig.y + orig.z * orig.z - radius * radius; 
 
    if (!solveQuadratic(A, B, C, t0, t1)) return false; 
 
    if (t0 > t1) std::swap(t0, t1); 
 
    return true; 
}

class Atmosphere 
{ 
public: 
    Atmosphere( 
        Vec3f sd = Vec3f(0, 1, 0), 
        float er = 6360e3, float ar = 6420e3, 
        float hr = 7994, float hm = 1200) : 
        sunDirection(sd), 
        earthRadius(er), 
        atmosphereRadius(ar), 
        Hr(hr), 
        Hm(hm) 
    {} 
 
    Vec3f computeIncidentLight(const Vec3f& dir) const; 
 
    Vec3f sunDirection;     // The sun direction (normalized) 
    float earthRadius;      // In the paper this is usually Rg or Re (radius ground, eart) 
    float atmosphereRadius; // In the paper this is usually R or Ra (radius atmosphere) 
    float Hr;               // Thickness of the atmosphere if density was uniform (Hr) 
    float Hm;               // Same as above but for Mie scattering (Hm) 
 
    static const Vec3f betaR; 
    static const Vec3f betaM;

    static const Vec3f orig;
    static const float tmin;
    static const float tmax;
}; 
 
const Vec3f Atmosphere::betaR(3.8e-6f, 13.5e-6f, 33.1e-6f); 
const Vec3f Atmosphere::betaM(21e-6f);
const Vec3f Atmosphere::orig(0, 6360e3 + 1, 0);
const float Atmosphere::tmin(0);
const float Atmosphere::tmax(10000);

Vec3f Atmosphere::computeIncidentLight(const Vec3f& dir) const 
{ 
    uint32_t numSamples = 16; 
    uint32_t numSamplesLight = 8; 
    float segmentLength = (tmax - tmin) / numSamples; 
    float tCurrent = tmin; 
    Vec3f sumR(0), sumM(0); // mie and rayleigh contribution 
    float opticalDepthR = 0, opticalDepthM = 0; 
    float mu = dot(dir, sunDirection); // mu in the paper which is the cosine of the angle between the sun direction and the ray direction 
    float phaseR = 3.f / (16.f * M_PI) * (1 + mu * mu); 
    float g = 0.76f; 
    float phaseM = 3.f / (8.f * M_PI) * ((1.f - g * g) * (1.f + mu * mu)) / ((2.f + g * g) * pow(1.f + g * g - 2.f * g * mu, 1.5f)); 
    for (uint32_t i = 0; i < numSamples; ++i) { 
        Vec3f samplePosition = orig + (tCurrent + segmentLength * 0.5f) * dir; 
        float height = samplePosition.length() - earthRadius; 
        // compute optical depth for light
        float hr = exp(-height / Hr) * segmentLength; 
        float hm = exp(-height / Hm) * segmentLength; 
        opticalDepthR += hr; 
        opticalDepthM += hm; 
        // light optical depth
        float t0Light, t1Light; 
        raySphereIntersect(samplePosition, sunDirection, atmosphereRadius, t0Light, t1Light); 
        float segmentLengthLight = t1Light / numSamplesLight, tCurrentLight = 0; 
        float opticalDepthLightR = 0, opticalDepthLightM = 0; 
        uint32_t j; 
        for (j = 0; j < numSamplesLight; ++j) { 
            Vec3f samplePositionLight = samplePosition + (tCurrentLight + segmentLengthLight * 0.5f) * sunDirection; 
            float heightLight = samplePositionLight.length() - earthRadius; 
            if (heightLight < 0) break; 
            opticalDepthLightR += exp(-heightLight / Hr) * segmentLengthLight; 
            opticalDepthLightM += exp(-heightLight / Hm) * segmentLengthLight; 
            tCurrentLight += segmentLengthLight; 
        } 
        if (j == numSamplesLight) { 
            Vec3f tau = betaR * (opticalDepthR + opticalDepthLightR) + betaM * 1.1f * (opticalDepthM + opticalDepthLightM); 
            Vec3f attenuation(exp(-tau.x), exp(-tau.y), exp(-tau.z)); 
            sumR += attenuation * hr; 
            sumM += attenuation * hm; 
        } 
        tCurrent += segmentLength; 
    } 
 
    return (sumR * betaR * phaseR + sumM * betaM * phaseM) * 100; 
}

#endif
