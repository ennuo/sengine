#ifndef SENGINE_JUMP_KING_REF_HPP
#define SENGINE_JUMP_KING_REF_HPP

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glad.h>

#include <crossguid/guid.hpp>

#include <map>
#include <string>
#include <memory>
#include <array>
#include <vector>
#include <utility>

using u64 = std::uint64_t;
using u32 = std::uint32_t;
using u16 = std::uint16_t;
using u8 = std::uint8_t;

using s64 = std::int64_t;
using s32 = std::int32_t;
using s16 = std::int16_t;
using s8 = std::int8_t;

using glm::quat;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;

using std::string;
using std::wstring;

using xg::Guid;

using Buffer = std::vector<unsigned char>;

template<typename T, size_t N>
using Array = std::array<T, N>;

template<typename T>
using Vec = std::vector<T>;

template<typename K, typename V>
using Map = std::map<K, V>;

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

#endif //SENGINE_JUMP_KING_REF_HPP
