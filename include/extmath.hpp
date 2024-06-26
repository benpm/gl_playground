#pragma once

#include <filesystem>
#include <random>
#include <array>
#include <sstream>
#include <optional>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#ifdef PLATFORM_WINDOWS
    #include <cstdint>
    #define uint uint32_t
    #undef near
    #undef far
#endif
#include <cyVector.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

using namespace Eigen;

using Transform3f = Transform<float, 3, Affine>;

namespace fs = std::filesystem;

constexpr float tau = 6.283185307179586476925286766559f;
constexpr float tau2 = tau / 2.0f;
constexpr float tau4 = tau / 4.0f;

void glCheckError_(const char *file, int line);

#ifdef BUILD_RELEASE
    #define $gl_err() {}
#else
    #define $gl_err() glCheckError_(__FILE__, __LINE__) 
#endif

template <typename TVector> struct EigenVectorFormatter
{
    const IOFormat ioFormat = IOFormat(4, DontAlignCols, ", ", ", ", "", "", "<", ">", ' ');

    std::string strFormat(const TVector& input)
    {
        std::stringstream ss;
        ss << input.format(ioFormat);
        return ss.str();
    }
};

// fmt overload for Vector
template <typename TValue, int _size> struct fmt::formatter<Vector<TValue, _size>>: formatter<string_view> {
    auto format(const Vector<TValue, _size>& i, format_context& ctx) const {
        return formatter<string_view>::format(EigenVectorFormatter<Vector<TValue, _size>>().strFormat(i), ctx);
    }
};

// fmt overload for std::vector
template <typename T> struct fmt::formatter<std::vector<T>> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        return ctx.end();
    }

    template <typename FormatContext>
    auto format(const std::vector<T>& input, FormatContext& ctx) -> decltype(ctx.out()) {
        std::string out = "[";
        for (const auto& item : input) {
            out += fmt::format("{}", item);
            out += ", ";
        }
        out += "]";
        return format_to(ctx.out(), "{}", out);
    }
};

// Plane
class Plane
{
public:
    Vector3f origin = {0.0f, 0.0f, 0.0f};
    Vector3f normal = {0.0f, 0.0f, 0.0f};

    Plane() = default;
    Plane(const Vector3f& origin, const Vector3f& normal);
};

// Ray
class Ray
{
public:
    Vector3f origin;
    Vector3f direction;

    Ray(const Vector3f& origin, const Vector3f& direction);

    std::optional<Vector3f> intersect(const Plane& plane) const;
    // Returns transformed ray
    Ray transformed(const Matrix4f& transform) const;
};

// Axis aligned bounding box
class AABB
{
public:
    static const std::array<Vector3f, 6u> faceNormals;

    Vector3f min;
    Vector3f max;

    AABB(const Vector3f& min, const Vector3f& max);

    std::array<Vector3f, 4> cornersXY() const;
    std::array<Vector3f, 8> corners() const;
    // Returns width (x span) of AABB
    float width() const;
    // Changes width (x span) of AABB, relative to current center
    void width(float width);
    // Returns height (y span) of AABB
    float height() const;
    // Changes height (y span) of AABB, relative to current center
    void height(float height);
    // Returns height (z span) of AABB
    float depth() const;
    // Changes depth (z span) of AABB, relative to current center
    void depth(float depth);
    // Makes AABB a square with given width / heighy, using current center
    void size(float size);
    // Returns extents
    Vector3f extents() const;
    // Returns center of AABB
    Vector3f center() const;
    // Places AABB by given center and size
    void place(const Vector3f& center, const Vector3f& size);
    void place(const Vector2f& center, const Vector2f& size);
    // Returns 3D volume of AABB
    float volume() const;
    // Returns vertices of AABB
    std::array<Vector3f, 8> vertices() const;
    // Returns the intersection point between this AABB and a ray
    std::optional<Vector3f> intersect(const Ray& ray) const;
    // Returns if point is inside AABB
    bool contains(const Vector3f& point, float eps = 1e-6f) const;

    AABB operator*(const Vector3f& v) const;
};

// Random number generation helper class
class RNG
{
public:
    const uint32_t seed;

private:
    std::minstd_rand gen;
    std::uniform_real_distribution<float> rdist;
    std::uniform_int_distribution<int> idist;
public:
    //Default constructor, uses specified seed for number generation
    RNG(uint32_t seed);
    //Generate a random number between 0 and 1, returns if this number is less than given probability value
    bool test(float probability);
    //Random range from a to b, inclusive
    int range(int a, int b);
    //Random range from a to b, inclusive
    float range(float a, float b);
    //Random vector3
    Vector3f vec(const Vector3f& min, const Vector3f& max);
    //Random vector3
    Vector3f vec(const AABB& bounds);
    //Random vector3 with min 0,0,0
    Vector3f vec(const Vector3f& max);
    //Random euler angles
    Vector3f rotation();
    //Random vector between two vectors
    template <class T> Vector2<T> vecRange(T a, T b) {
        return {this->range(a, b),
                this->range(a, b)};
    };
    //Choose random from list of items
    template <class T> T choose(const std::initializer_list<T> items) {
        auto it = items.begin();
        std::advance(it, range(0, items.size() - 1));
        return *it;
    };
};

// Linear interpolate
template <typename T> T lerp(const T& a, const T& b, float t) {
    return a + (b - a) * t;
}
// Normalized atan2
float angle2D(const Vector2f& v);
// Convert radians to degrees
float degrees(float radians);
/**
 * @brief Convert HSV to RGB
 * 
 * @param hsv hue(0-360deg), saturation(0-1), value(0-1)
 * @return RGB
 */
Vector3f hsvToRgb(const Vector3f& hsv);
// Return a new identity transform
Transform3f identityTransform();
// Convert cy::Vec3f to Vector3f
Vector3f toEigen(const cy::Vec3f& vec);
// Euler angles to quaternion
Quaternionf euler(const Vector3f& axisAngles);
/**
 * @brief Convert spherical to cartesian coordinates (+Y up) ...
 *        (theta=0, phi=0) -> (x=0, y=0, z=1)
 * 
 *        distance from origin is always 1
 * 
 * @param phi Horizontal angle
 * @param theta Vertical angle
 * @return Cartesian coordinate
 */
Vector3f spherePoint(float phi, float theta);
Vector3f spherePoint(const Vector2f& point);
// Convert cartesian to spherical coordinates (+Y up)
Vector2f pointSphere(const Vector3f& point);
// Converts a direction to an euler angles rotation
Vector3f dirToRot(const Vector3f& dir);
// Returns a 3x3 skew-symmetric matrix from given vec3
Matrix3f skew(const Vector3f& v);
// Rotates a vector by given euler angles
Vector3f rotate(const Vector3f& v, const Vector3f& axisAngles);
// Gets direction corresponding to given euler angles
Vector3f direction(const Vector3f& axisAngles);
// Angle from a to b
Vector3f towards(const Vector3f& a, const Vector3f& b);
// Perspective projection matrix
Matrix4f perspective(float fov, float aspect, float near, float far);
// Perspective projection matrix
Matrix4f perspective(const Vector4f& view, float near, float far);
// Orthographic projection matrix
Matrix4f orthographic(const Vector2f& size, float near, float far);
// Project vector a onto b
Vector3f project(const Vector3f& a, const Vector3f& b);
// Creates a transformation matrix from given translation, rotation, and scale
Matrix4f transform(const Vector3f& translation, const Vector3f& axisAngles, const Vector3f& scale={1.0f, 1.0f, 1.0f});
Matrix4f transform(const Vector3f& translation, const Matrix3f& rotMatrix, const Vector3f& scale={1.0f, 1.0f, 1.0f});
// Applies transformation to a point (applies translation, doesn't normalize)
Vector3f transformPoint(const Vector3f& point, const Matrix4f& transform);
// Applies transformation to a direction vector (ignores translation, normalizes)
Vector3f transformDir(const Vector3f& direction, const Matrix4f& transform);

Vector3f vec3(float v[3]);
Vector3f vec3(const Vector2f& v, float z=0.0f);
Vector3f vec3(float xyz);
Vector2f vec2(const Vector3f& v);
Vector4f vec4(const Vector3f& v, float w=1.0f);
Vector4f vec4(const Vector2f& v, float z, float w);

// Cantor's pairing function
uint64_t cantor(uint32_t x, uint32_t y);