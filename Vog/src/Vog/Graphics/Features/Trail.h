#pragma once
#include "Vog/Core/Core.h"

#include "Vog/Utilities/MyCommon.h"

#include "Vog/Math/MyMath.h"

#include "Vog/Graphics/RenderPass/RenderPass.h"
#include "Vog/Graphics/Renderer/Renderer.h"

// References:
// https://github.com/SimpleTalkCpp/workshop-2021-07-unity-shader/tree/main/Assets/Week002/Week002%20-%20Trail

namespace vog {

	struct TrailVertexLayout
	{
		Vector3f position;
		Vector2f uv;
	};

	struct TrailNode
	{
		Vector3f position0 = Vector3f(0.0f);
		Vector3f position1 = Vector3f(0.0f);

		float lifeTime = 0.0f;
		bool isBreakdown = false;
	};

	class VOG_API Trail : public NonCopyable
	{
	public:
		Trail() = default;
		~Trail();

		void init();
		void destroy();

		void update(float dt_, const Vector3f& point_, const Vector3f& right_);

		void split(TrailNode& leftNode_, TrailNode& rightNode_, int depth_);

		void setupMesh();

		void addPoint(const Vector3f& point_, const Vector3f& right_);
		void addNode(TrailNode& node_, int depth_);
		void updateNode(float dt_, const Vector3f& point_, const Vector3f& right_);

		void onImGuiRender();

		uint32_t getIndexCount();

	public:
		
		uint32_t verticesCount = 0;

		RefPtr<VertexBuffer> pVertexBuffer;
		RefPtr<IndexBuffer> pIndexBuffer;
		RefPtr<Shader> pShader;

		std::vector<TrailNode> nodes;


		float width = 2.0f;
		float lifeTime = 5.0f;
		float tolerance = 0.1f;

	private:
		static constexpr int s_max_node_count = 30;
		static constexpr int s_max_vertices_count = s_max_node_count * 2;
		static constexpr int s_max_indices_count = s_max_vertices_count * 6;

		//ScopePtr<TrailVertexLayout[]> m_pVertices = nullptr;
		TrailVertexLayout* m_itVertices = nullptr;
		TrailVertexLayout* m_pVertices = nullptr;

		int m_currentNode = 0;
	};

	inline uint32_t Trail::getIndexCount() { return 6 * (nodes.size() - m_currentNode - 1); }

}