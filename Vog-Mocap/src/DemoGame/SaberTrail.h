#pragma once
#include <Vog.h>

#include "ColorType.h"

#include <Vog/Graphics/Features/Trail.h>

namespace demo_game
{
	//#define ENABLE_SABER_DEBUG_LOG

	using namespace vog;
	class SaberTrail : public NativeScriptEntity
	{
	public:

		Trail* pTrail = nullptr;

		float m_timer = 0.0f;

	public:
		SaberTrail() = default;
		virtual ~SaberTrail()
		{
			pTrail = nullptr;
		}

		virtual void onStart()
		{
			pTrail = (Trail*)getComponent<NativeScriptComponent>().pUeserData;
		};

		virtual void onDestroy()
		{

		};

		virtual void onUpdate(float dt_)
		{
			if (m_timer > 2.0f)
			{
				//pTrail->addPoint(getComponent<TransformComponent>().translation);
				m_timer = 0.0f;
			}

			pTrail->addPoint(getComponent<TransformComponent>().translation);

			pTrail->update(dt_);

			getComponent<MeshComponent>().indexCount = pTrail->getIndexCount();

			m_timer += dt_;
		};

		virtual void onTriggerEnter(uint32_t handle_) override
		{

		};

		virtual void onTriggerStay(uint32_t handle_) override
		{
			Entity other = { handle_, m_entity };
			//VOG_CORE_LOG_INFO("Move Entity: onTriggerStay!");
		};

		virtual void onTriggerExit(uint32_t handle_) override
		{
			Entity other = { handle_, m_entity };

		};

	private:
	};
}