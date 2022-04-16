#include <Vog.h>
#include "Vog/Core/StartupEntry.h"

#include <ImGui/imgui.h>

#include "EditorLayer.h"

class VogEditor : public vog::Application
{
public:
	VogEditor()
		:
		Application("Vog Editor")
	{
		pushLayer(new vog::EditorLayer("Editor Layer"));
	}
private:

};

vog::Application* vog::createApplication()
{
	return new VogEditor();
}