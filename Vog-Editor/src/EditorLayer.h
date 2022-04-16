#pragma once
#include <Vog.h>
#include <ImGui/imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Vog/ImGui/ImGuiLibrary.h"
#include "Vog/Editor/ScenePanel.h"

namespace vog
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer(const std::string& name_);

		virtual ~EditorLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(float timestep_) override;
		virtual void onEvent(Event& event_) override;
		virtual void onImGuiRender() override;
		virtual void onDebugRender() override;

		void onScenePlay();
		void onSceneStop();

		void renderColliderOutline();
		void renderLightOutline();

		Vector2f getViewportSize();
		Vector2f getViewportMousePosition();

		bool onMouseButtonPressed(MouseButtonPressedEvent& event_);
		bool onKeyPressed(KeyPressedEvent& event_);

	private:
		CameraController m_cameraController;

		Vector2f m_viewportSize = { 0.0f , 0.0f };
		bool m_isViewportFocused = false, m_isViewportHovered = false;

		//Scene
		RefPtr<Scene> m_pEditorScene;
		RefPtr<Scene> m_pActiveScene;

		enum class EditorSceneState
		{
			Edit = 0, Play,
		};
		EditorSceneState m_sceneState = EditorSceneState::Edit;

		ScenePanel m_scenePanel;
		bool m_isEnableColliderOutline = true;

		struct ray
		{
			Vector3f origin;
			Vector3f direction;
		};
		ray m_ray;

		Entity m_moveEntity;

	};
}