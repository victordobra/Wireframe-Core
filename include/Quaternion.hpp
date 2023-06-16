#pragma once

#include "Defines.hpp"

namespace wfe {
	struct Vector3;
	struct Vector4;

	/// @brief A struct that holds a quaternion. Used for representing 3D rotations.
	struct Quaternion {
		/// @brief An identity quaternion, representing no rotation.
		static const Quaternion identity;

		union {
			/// @brief An array containing all of the quaternion's elements.
			float32_t elements[4];

			struct {
				/// @brief The X dimension of the quaternion.
				float32_t x;
				/// @brief The Y dimension of the quaternion.
				float32_t y;
				/// @brief The Z dimension of the quaternion.
				float32_t z;
				/// @brief The W dimension of the quaternion.
				float32_t w;
			};
		};

		/// @brief Creates an identity quaternion.
		Quaternion();
		/// @brief Copies the given quaternion.
		/// @param other The quaternion to copy.
		Quaternion(const Quaternion& other) = default;
		/// @brief Moves the given quaternion's contents.
		/// @param other The quaternion to move.
		Quaternion(Quaternion&& other) noexcept = default;
		/// @brief Creates a quaternion with the given dimensions.
		/// @param x The X dimension of the quaternion.
		/// @param y The Y dimension of the quaternion.
		/// @param z The Z dimension of the quaternion.
		/// @param w The W dimension of the quaternion.
		Quaternion(float32_t x, float32_t y, float32_t z, float32_t w);

		/// @brief Converts the given Vector4 into a quaternion.
		/// @param other The Vector4 to convert.
		explicit Quaternion(const Vector4& other);

		/// @brief Copies the given quaternion's contents into this quaternion.
		/// @param other The quaternion to copy.
		/// @return A reference to this quaternion.
		Quaternion& operator=(const Quaternion& other) = default;
		/// @brief Moves the given quaternion's contents into this quaternion.
		/// @param other The quaternion to move.
		/// @return A reference to this quaternion.
		Quaternion& operator=(Quaternion&& other) noexcept = default;

		/// @brief Compares the two quaternions.
		/// @param other The quaternion to compare with.
		/// @return True if the two quaternions are equal, otherwise false.
		bool8_t operator==(const Quaternion& other) const;
		/// @brief Compares the two quaternions.
		/// @param other The quaternion to compare with.
		/// @return True if the two quaternions are different, otherwise false.
		bool8_t operator!=(const Quaternion& other) const;

		/// @brief Multiplies the two given quaternions.
		/// @param other The quaternion to multiply with.
		/// @return The resulting quaternion.
		Quaternion operator*(const Quaternion& other) const;
		/// @brief Multiplies the given quaternion with this quaternion.
		/// @param other The quaternion to multiply with.
		/// @return A reference to this quaternion.
		Quaternion& operator*=(const Quaternion& other);

		/// @brief Calculates the quaternion's magnitude.
		/// @return The quaternion's magnitude.
		float32_t Magnitude() const;
		/// @brief Calculates the square of the quaternion's magnitude.
		/// @return The square of the quaternion's magnitude.
		float32_t SqrMagnitude() const;
		/// @brief Calculates the inverse of the quaternion's magnitude.
		/// @return The square of the quaternion's magnitude.
		float32_t InvMagnitude() const;

		/// @brief Reduces the quaternion's magnitude to 1 while maintaining its proportions.
		/// @return A reference to this quaternion.
		Quaternion& Normalize();
		/// @brief Calculates a quaternion with a magnitude of 1 and the same proportions as this quaternion.
		/// @return The resulting quaternion.
		Quaternion Normalized() const;
		/// @brief Inverts this quaternion.
		/// @return A reference to this quaternion.
		Quaternion& Invert();
		/// @brief Creates a new quaternion equal to the inverted version of this quaternion.
		/// @return The resulting quaternion.
		Quaternion Inverted() const;

		/// @brief Calculates the dot product of the two quaternions.
		/// @param other The other quaternion.
		/// @return The dot product of the two quaternions.
		float32_t Dot(const Quaternion& other) const;

		/// @brief Deletes this quaternion.
		~Quaternion() = default;

		/// @brief Generates a quaternion that represents the rotation around an axis.
		/// @param axis The axis to rotate around. Must be normalized.
		/// @param angle The angle of rotation, in radians.
		/// @return The resulting quaternion.
		static Quaternion AroundAxis(Vector3 axis, float32_t angle);
		/// @brief Generates a quaternion that represents the rotation of the given euler angles.
		/// @param eulerAngles The euler angles, in radians.
		/// @return The resulting quaternion.
		static Quaternion EulerAngles(Vector3 eulerAngles);
	};
}