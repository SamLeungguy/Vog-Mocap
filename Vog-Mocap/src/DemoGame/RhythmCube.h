#pragma once
#include <Vog.h>

#include "ColorType.h"

namespace demo_game
{
	using namespace vog;
	class RhythmCube : public NativeScriptEntity
	{
	public:
		RhythmCube() = default;
		virtual ~RhythmCube() = default;

		static std::array<std::string, 2> s_names;

		virtual void onStart()
		{
		};
		virtual void onDestroy()
		{
		};


		virtual void onUpdate(float dt_)
		{
		};

		virtual void onTriggerEnter(uint32_t handle_) override
		{
			//Entity other = { handle_, m_entity };

#if 0
			const std::string& my_name = m_entity.getComponent<TagComponent>().tag;
			const std::string& other_name = other.getComponent<TagComponent>().tag;

			VOG_CORE_LOG_INFO("{0} with {1}: onTriggerEnter!", my_name, other_name);
#endif // 0
		};

		virtual void onTriggerStay(uint32_t handle_) override
		{
			//Entity other = { handle_, m_entity };
			//VOG_CORE_LOG_INFO("Move Entity: onTriggerStay!");
		};

		virtual void onTriggerExit(uint32_t handle_) override
		{
			//Entity other = { handle_, m_entity };


#if 0
			const std::string& my_name = m_entity.getComponent<TagComponent>().tag;
			const std::string& other_name = other.getComponent<TagComponent>().tag;

			VOG_CORE_LOG_INFO("{0} with {1}: onTriggerExit!", my_name, other_name);
#endif // 0


		};

		void create()
		{
			VOG_LOG_INFO("RhythmCube: create!");
		}
	};
}