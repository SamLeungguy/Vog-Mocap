#pragma once
#include <Vog.h>
#include "RhythmCube.h"

#include "RhythmCubeManager.h"

#include "LightManager.h"

namespace demo_game
{
#define ENABLE_SABER_DEBUG_LOG

	using namespace vog;
	class Saber : public NativeScriptEntity
	{
	public:
		Vector3f prev_position = {0.0f, 0.0f, 0.0f};

	public:
		Saber() = default;
		virtual ~Saber() = default;

		virtual void onStart()
		{
			//VOG_CORE_LOG_INFO("Move Entity: onStart!");

		};

		virtual void onDestroy()
		{

		};

		virtual void onUpdate(float dt_)
		{
			prev_position = getComponent<TransformComponent>().translation;

		};

		virtual void onTriggerEnter(uint32_t handle_) override
		{
#ifdef ENABLE_SABER_DEBUG_LOG
			VOG_LOG_INFO("==Start==");

			VOG_LOG_INFO("prev_position");
			VOG_LOG_GLM(prev_position);

			VOG_LOG_INFO("current_position");
			VOG_LOG_GLM(getComponent<TransformComponent>().translation);
#endif // ENABLE_SABER_DEBUG_LOG

			Entity other = { handle_, m_entity };
			if (!other.hasComponent<NativeScriptComponent>())
				return;

			RhythmCube* pCube = other.getComponent<NativeScriptComponent>().get<RhythmCube>();
			if (!pCube)
				return;
			auto& cubeTransform = pCube->getComponent<TransformComponent>();
			auto cube_up = MyMath::toQuaternion(cubeTransform.rotation)* Vector3f(0.0f, 1.0f, 0.0f);

			auto dir = getComponent<TransformComponent>().translation - prev_position;
			dir = MyMath::normalize(dir);

			auto cos_angle = MyMath::dot(dir, cube_up);
			auto angle = MyMath::acos(cos_angle);

			if (angle > MyMath::radians(130.0f))
			{
			}

			RhythmCubeManager::get().destroyRythm(other);

#ifdef ENABLE_SABER_DEBUG_LOG
			VOG_LOG_INFO("cube_up");
			VOG_LOG_GLM(cube_up);
			VOG_LOG_INFO("angle: {0}", angle);

			if (angle > MyMath::radians(130.0f))
			{
				VOG_LOG_INFO("Saber hitted RhythmCube!");
			}
			VOG_LOG_INFO("==End==");
#endif // ENABLE_SABER_DEBUG_LOG
		};

		virtual void onTriggerStay(uint32_t handle_) override
		{
			Entity other = { handle_, m_entity };
			//VOG_CORE_LOG_INFO("Move Entity: onTriggerStay!");
		};

		virtual void onTriggerExit(uint32_t handle_) override
		{
			Entity other = { handle_, m_entity };


#if 0
			const std::string& my_name = m_entity.getComponent<TagComponent>().tag;
			const std::string& other_name = other.getComponent<TagComponent>().tag;

			VOG_CORE_LOG_INFO("{0} with {1}: onTriggerExit!", my_name, other_name);
#endif // 0


		};
	};
}