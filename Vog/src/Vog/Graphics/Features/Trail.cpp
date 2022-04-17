#include "vogpch.h"
#include "Trail.h"

#include <Vog/Resources/AssetManager.h>

namespace vog {
	Trail::~Trail()
	{
		destroy();
	}

	void Trail::init()
	{
		/*m_pVertices = ScopePtr<TrailVertexLayout[]>(new TrailVertexLayout[s_max_vertices_count]);
		m_itVertices = m_pVertices.get();*/

		m_pVertices = new TrailVertexLayout[s_max_vertices_count];
		m_itVertices = m_pVertices;

		pVertexBuffer = VertexBuffer::create(sizeof(TrailVertexLayout) * s_max_vertices_count);
		pVertexBuffer->setVertexLayout(
			VertexLayout{
				{ VertexDataType::Float3, "a_position", },
				{ VertexDataType::Float2, "a_texCoord", }
			}
		);
		uint32_t* pIndices = new uint32_t[s_max_indices_count];
		for (uint32_t i = 0, offset = 0; i < s_max_indices_count; i += 6)
		{
			pIndices[i + 0] = 0 + offset;
			pIndices[i + 1] = 2 + offset;
			pIndices[i + 2] = 1 + offset;

			pIndices[i + 3] = 3 + offset;
			pIndices[i + 4] = 1 + offset;
			pIndices[i + 5] = 2 + offset;

			offset += 2;
		}

		pIndexBuffer = IndexBuffer::create(pIndices, s_max_indices_count);
		delete[] pIndices;
		pIndices = nullptr;

		pShader = AssetManager::getShader("Trail");

		nodes.reserve(s_max_node_count);
	}

	void Trail::destroy()
	{
		if (m_pVertices)
		{
			delete[] m_pVertices;
			m_pVertices = nullptr;
			m_itVertices = nullptr;
		}

		pShader.reset();
		pVertexBuffer.reset();
		pIndexBuffer.reset();
	}

	void Trail::update(float dt_)
	{
		setupMesh();

		draw();

		for (size_t i = 0; i < nodes.size(); i++)
		{
			nodes[i].lifeTime += dt_;
		}

		nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
			[&](const TrailNode& node_)
			{
				return node_.lifeTime > lifeTime;
			}), nodes.end());

		VOG_CORE_LOG_INFO("Node size: {0}", nodes.size());

	}

	void Trail::split(const TrailNode& node_)
	{
	}

	void Trail::setupMesh()
	{
		for (int i = 1; i < nodes.size(); i++)
		{
			int v0_index = (i - 1) * 2 + 0;
			int v1_index = (i - 1) * 2 + 1;

			int v2_index = i * 2 + 0;
			int v3_index = i * 2 + 1;

			if (!(v0_index < s_max_vertices_count
				&& v1_index < s_max_vertices_count
				&& v2_index < s_max_vertices_count
				&& v3_index < s_max_vertices_count))
			{
				int a = 0;
			}

			VOG_CORE_ASSERT(v0_index < s_max_vertices_count
						 && v1_index < s_max_vertices_count
						 && v2_index < s_max_vertices_count
						 && v3_index < s_max_vertices_count, "");

			auto& v0 = m_itVertices[v0_index];
			auto& v1 = m_itVertices[v1_index];

			auto& v2 = m_itVertices[v2_index];
			auto& v3 = m_itVertices[v3_index];

			const auto& first_center = nodes[i - 1].position;
			const auto& second_center = nodes[i].position;

			Vector3f direction = Vector3f(0.0f, 1.0f, 0.0f) * width;
			v0.position = first_center - direction;
			v0.uv = Vector2f(0.0f);

			v1.position = first_center + direction;
			v1.uv = Vector2f(0.0f, 1.0f);

			v2.position = second_center - direction;
			v2.uv = Vector2f(1.0f, 0.0f);

			v3.position = second_center + direction;
			v3.uv = Vector2f(1.0f);
		}

		if (nodes.size() > 1)
		{
			VOG_CORE_ASSERT(m_pVertices, "");
			pVertexBuffer->setData(m_pVertices, (uint32_t)(sizeof(TrailVertexLayout) * nodes.size() * 2));
		}
	}

	void Trail::draw()
	{
		
	}

	void Trail::addPoint(Vector3f point_)
	{
		if (nodes.size() < s_max_node_count)
		{
			TrailNode node;
			node.position = point_;
			nodes.push_back(node);
		}
	}
}
