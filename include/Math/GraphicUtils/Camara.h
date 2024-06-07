#pragma once
#include <Math/Math.h>

namespace MathLib
{
	namespace GraphicUtils
	{
		inline HMatrix4 LookAt(const HVector3& eye, const HVector3& target, const HVector3& up) {
			const HVector3 backward = (eye - target).normalized();
			const HVector3 right = up.cross(backward).normalized();
			const HVector3 realUp = backward.cross(right);
			HMatrix4 matrix;
			matrix << right[0], right[1], right[2], 0,
				realUp[0], realUp[1], realUp[2], 0,
				backward[0], backward[1], backward[2], 0,
				eye[0], eye[1], eye[2], 1;
			return matrix;
		}

		inline HMatrix4 Perspective(HReal fov, HReal aspect, HReal near, HReal far) {
			float S = 1.0f / std::tanf(fov / 2 * MathLib::H_PI / 180.f);

			HMatrix4 m = HMatrix4::Zero();
			m(0, 0) = S / aspect;
			m(1, 1) = S;
			m(2, 2) = -(far + near) / (far - near);
			m(2, 3) = -1.0f;
			m(3, 2) = -(2.0f * far * near) / (far - near);
			m(3, 3) = 0.0f;

			return m;
		}

		class Camera
		{
		public:
			Camera(const HVector3& eye, const HVector3& dir, const MathLib::HReal& aspectRatio,const HReal nearClip=1.f,const HReal farClip=10000.f,const HReal fov=60.f) 
				: mMouseX(0), 
				mMouseY(0), 
				mSpeed(2.0f),
				mAspectRatio(aspectRatio),
				mNearClip(nearClip),
				mFarClip(farClip),
				mFOV(fov)
			{
				mEye = eye;
				mDir = dir.normalized();
				_UpdateMatrix();
			}
			void HandleMouse(int button, int state, int x, int y)
			{
				(void)state;
				(void)button;
				mMouseX = x;
				mMouseY = y;
			}
			bool HandleKey(unsigned char key, int x, int y, MathLib::HReal speed = 0.5f)
			{
				(void)x;
				(void)y;

				const HVector3 viewY = mDir.cross(HVector3(0, 1, 0)).normalized();
				switch (toupper(key))
				{
				case 'W':   mEye += mDir * mSpeed * speed; break;
				case 'S':   mEye -= mDir * mSpeed * speed; break;
				case 'A':   mEye -= viewY * mSpeed * speed; break;
				case 'D':   mEye += viewY * mSpeed * speed; break;
				case 'Q':   mEye += HVector3(0, 1, 0) * mSpeed * speed; break;
				case 'E':   mEye -= HVector3(0, 1, 0) * mSpeed * speed; break;
				default:                                return false;
				}
				_UpdateMatrix();
				return true;
			}
			void HandleAnalogMove(int x, int y)
			{
				HVector3 viewY = mDir.cross(HVector3(0, 1, 0)).normalized();
				mEye += mDir * y;
				mEye += viewY * x;
				_UpdateMatrix();
			}
			void HandleMotion(MathLib::HReal x, MathLib::HReal y)
			{
				const int dx = mMouseX - x;
				const int dy = mMouseY - y;

				const HVector3 viewY = mDir.cross(HVector3(0, 1, 0)).normalized();

				const MathLib::HReal Sensitivity = H_PI * 0.5f / 180.0f;

				const MathLib::HAngleAxis qx(Sensitivity * dx, HVector3(0, 1, 0));
				mDir = qx * mDir;
				const MathLib::HAngleAxis qy(Sensitivity * dy, viewY);
				mDir = qy * mDir;

				mDir.normalize();

				mMouseX = x;
				mMouseY = y;

				_UpdateMatrix();
			}

			HVector3 GetEye() const
			{
				return mEye;
			}
			HVector3 GetDir() const
			{
				return mDir;
			}
			HTransform3 GetTransform()
			{
				HVector3 viewY = mDir.cross(HVector3(0, 1, 0));

				if (viewY.norm() < 1e-6f)
					return HTransform3(MathLib::HTranslation3(mEye));

				const HMatrix3 m = (HMatrix3() << mDir.cross(viewY), viewY, -mDir).finished();
				return HTransform3(MathLib::HTranslation3(mEye) * HQuaternion(m));
			}
			HMatrix4 GetViewMatrix()
			{
				return mViewMatrix;
			}

			HMatrix4 GetProjectMatrix()
			{
				return mProjectMatrix;
			}

			HMatrix4 GetViewProjectMatrix()
			{
				return mViewProjectMatrix;
			}

			HReal GetSpeed() const { return mSpeed; }
			HReal GetAspectRatio() const { return mAspectRatio; }
			HReal GetNearClip() const { return mNearClip; }
			HReal GetFarClip() const { return mFarClip; }
			HReal GetFOV() const { return mFOV; }

			void SetPose(const HVector3& eye, const HVector3& dir)
			{
				mViewMatrix = LookAt(mEye, mEye + mDir, HVector3(0, 1, 0));
				mProjectMatrix = Perspective(mFOV, mAspectRatio, mNearClip, mFarClip);
				mViewProjectMatrix = (mProjectMatrix.transpose() * mViewMatrix.transpose()).transpose();
			}

			void SetSpeed(MathLib::HReal speed)
			{
				mSpeed = speed;
			}

		private:
			void _UpdateMatrix()
			{
				mViewMatrix = LookAt(mEye, mEye + mDir, HVector3(0, 1, 0));
				mProjectMatrix = Perspective(mFOV, mAspectRatio, mNearClip, mFarClip);
				mViewProjectMatrix = (mProjectMatrix.transpose() * mViewMatrix.transpose()).transpose();
			}

		private:
			HVector3 mEye;
			HVector3 mDir;
			MathLib::HReal mAspectRatio;
			MathLib::HReal mNearClip = 1.0f;
			MathLib::HReal mFarClip = 10000.0f;
			MathLib::HReal mFOV = 60.0f;
			int mMouseX;
			int mMouseY;
			MathLib::HReal mSpeed;
			MathLib::HMatrix4 mViewMatrix;
			MathLib::HMatrix4 mProjectMatrix;
			MathLib::HMatrix4 mViewProjectMatrix;
		};

	} // namespace GraphicTool
} // namespace MathLib