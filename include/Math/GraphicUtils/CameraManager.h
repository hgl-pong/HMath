#pragma once

#include <Math/GraphicUtils/Camara.h>
#include <memory>
namespace MathLib
{
	namespace GraphicUtils
	{
		class CameraManager
		{
		public:
			CameraManager()
			{
				m_ActiveCamera = nullptr;
				m_CameraLists.clear();
			}
			~CameraManager()
			{
				m_CameraLists.clear();
				m_ActiveCamera = nullptr;
			}
			
			void AddCamera(const char* name, std::shared_ptr<Camera>& camera)
			{
				m_CameraLists.emplace(name, camera);
				if (m_ActiveCamera == nullptr)
				{
					m_ActiveCamera = camera.get();
					m_ActiveCameraName = name;
				}
			}
			void RemoveCamera(const char* name)
			{
				auto result = m_CameraLists.find(name);
				if (result != m_CameraLists.end())
				{
					if (m_ActiveCamera == result->second.get())
					{
						m_ActiveCamera = m_CameraLists.empty() ? nullptr : m_CameraLists.begin()->second.get();
					}
					m_CameraLists.erase(result);
				}
			}
			void SetActiveCamera(const char* name)
			{
				auto result = m_CameraLists.find(name);
				if (result != m_CameraLists.end())
				{
					m_ActiveCamera = result->second.get();
				}
			}

			Camera* GetActiveCamera()
			{
				return m_ActiveCamera;
			}

			std::string GetActiveCameraName()const
			{
				return m_ActiveCameraName;
			}

			void SwitchToNextCamera()
			{
				if (m_ActiveCamera == nullptr || m_CameraLists.size() <= 1)
					return;
				auto it = m_CameraLists.find(m_ActiveCameraName);
				it++;
				if (it == m_CameraLists.end())
					it = m_CameraLists.begin();
				m_ActiveCameraName = it->first;
				m_ActiveCamera = it->second.get();
			}

			void CreateCamrera(const char* name, const HVector3& eye, const HVector3& dir, const MathLib::HReal& aspectRatio, const HReal nearClip = 1.f, const HReal farClip = 10000.f, const HReal fov = 60.f)
			{
				std::shared_ptr<Camera> newCamera = std::make_shared<Camera>(eye, dir, aspectRatio,nearClip,farClip,fov);
				AddCamera(name, newCamera);
			}
			
		private:
			std::string m_ActiveCameraName;
			Camera* m_ActiveCamera = nullptr;
			std::unordered_map<std::string, std::shared_ptr<Camera>> m_CameraLists;
		};
	}
}