#pragma once
#include <Vog.h>

#include "RhythmCube.h"

namespace demo_game
{
	using namespace vog;
	class LightManager : public NativeScriptEntity
	{
	public:
		static constexpr int s_max_count = 50;
		std::vector<uint32_t> m_lightHandles;

	public:
		LightManager()
		{
			VOG_ASSERT(!s_pInstance, "");
		}
		virtual ~LightManager()
		{
			s_pInstance = nullptr;
		}

		LightManager& get() { VOG_ASSERT(s_pInstance, ""); return *s_pInstance; }

		virtual void onAwake() override;

		virtual void onStart() override
		{
			//VOG_CORE_LOG_INFO("Move Entity: onStart!");

			//m_lightHandles.add

		};

		virtual void onDestroy() override
		{

		};

		virtual void onUpdate(float dt_) override
		{
		};

		void setColor(ColorType::Color type_)
		{

		}

		virtual void onTriggerEnter(uint32_t handle_) override
		{
			Entity other = { handle_, m_entity };
		};

		virtual void onTriggerStay(uint32_t handle_) override
		{
			Entity other = { handle_, m_entity };
		};

		virtual void onTriggerExit(uint32_t handle_) override
		{
			Entity other = { handle_, m_entity };
		};

	private:
		static LightManager* s_pInstance;
	};
}