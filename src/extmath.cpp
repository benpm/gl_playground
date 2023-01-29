#include <extmath.hpp>

Transform3f identityTransform() {
    return Transform3f::Identity();
}

Vector3f toEigen(const cy::Vec3f& vec) {
    return Vector3f(vec.x, vec.y, vec.z);
}

Quaternionf euler(const Vector3f& axisAngles) {
    return 
        AngleAxisf(axisAngles.x(), Vector3f::UnitX()) *
        AngleAxisf(axisAngles.y(), Vector3f::UnitY()) *
        AngleAxisf(axisAngles.z(), Vector3f::UnitZ());
}

Vector3f direction(const Vector3f &axisAngles) {
    return {
        std::cos(axisAngles.y()) * std::cos(axisAngles.x()),
        std::sin(axisAngles.y()),
        std::cos(axisAngles.y()) * std::sin(axisAngles.x())
    };
}

Vector3f towards(const Vector3f &a, const Vector3f &b) {
    // Euler angles of the rotation from a to b
    return {
        std::atan2(b.z() - a.z(), b.x() - a.x()),
        std::atan2(b.y() - a.y(), std::sqrt(std::pow(b.x() - a.x(), 2) + std::pow(b.z() - a.z(), 2))),
        0.0f
    };
}

Matrix4f lookAt(const Vector3f& eye, const Vector3f& center, const Vector3f& up) {
    const Vector3f f = (center - eye).normalized();
    const Vector3f s = f.cross(up).normalized();
    const Vector3f u = s.cross(f);

    Matrix4f m = Matrix4f::Identity();
    m(0, 0) = s.x();
    m(1, 0) = s.y();
    m(2, 0) = s.z();
    m(0, 1) = u.x();
    m(1, 1) = u.y();
    m(2, 1) = u.z();
    m(0, 2) = -f.x();
    m(1, 2) = -f.y();
    m(2, 2) = -f.z();
    m(3, 0) = -s.dot(eye);
    m(3, 1) = -u.dot(eye);
    m(3, 2) = f.dot(eye);
    return m;
}

Matrix4f perspective(float fov, float aspect, float near, float far) {
    Matrix4f m = Matrix4f::Zero();
    m(0, 0) = 1.0f / (aspect * tanf(fov / 2.0f));
    m(1, 1) = 1.0f / tanf(fov / 2.0f);
    m(2, 2) = -(far + near) / (far - near);
    m(2, 3) = -2.0f * far * near / (far - near);
    m(3, 2) = -1.0f;
    return m;
}

Matrix4f orthographic(const Vector2f& size, float near, float far) {
    Matrix4f m = Matrix4f::Identity();
    m(0, 0) = 2.0f / size.x();
    m(1, 1) = 2.0f / size.y();
    m(2, 2) = -2.0f / (far - near);
    m(3, 2) = -(far + near) / (far - near);
    return m;
}