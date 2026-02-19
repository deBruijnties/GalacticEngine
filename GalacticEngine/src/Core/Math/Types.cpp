#include <pch.h>
#include "types.h"
#include "Math.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace GalacticEngine
{

    // local conversion helpers

    static glm::vec3 ToGLM(const Vector3& v) { return { v.x, v.y, v.z }; }
    static Vector3 FromGLM(const glm::vec3& v) { return { v.x, v.y, v.z }; }

    static glm::quat ToGLM(const Quaternion& q) { return { q.w, q.x, q.y, q.z }; }
    static Quaternion FromGLM(const glm::quat& q) { return { q.x, q.y, q.z, q.w }; }

    Quaternion Quaternion::Identity()
    {
        return Quaternion(0, 0, 0, 1);
    }

    Quaternion Quaternion::Normalized() const
    {
        glm::quat q = glm::normalize(ToGLM(*this));
        return FromGLM(q);
    }

    Quaternion Quaternion::FromEuler(const Vector3& euler)
    {
        glm::quat q = glm::quat(glm::vec3(euler.x, euler.y, euler.z));
        return FromGLM(q);
    }

    Quaternion Quaternion::quatLookAtRH(const Vector3& dir, const Vector3& up)
    {
        glm::quat q = glm::quatLookAtRH(ToGLM(dir), ToGLM(up));
        return FromGLM(q);
    }

}
