#include "vogpch.h"
#include "Trail.h"

#include <Vog/Resources/AssetManager.h>

#include "Vog/ImGui/ImGuiLibrary.h"

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

	void Trail::split(TrailNode& leftNode_, TrailNode& rightNode_, int depth_)
	{
		static int max_depth = 6;
		if (depth_ > max_depth)
			return;

		float total_width = width * 2;
		float cos_angle = MyMath::dot((rightNode_.position1 - rightNode_.position0) / total_width, (leftNode_.position1 - leftNode_.position0) / total_width);

		TrailNode new_node;

		if (1 - cos_angle > spiltThreshold)
		{
			new_node.position0 = (leftNode_.position0 + rightNode_.position0) / 2.0f;
			new_node.position1 = (leftNode_.position1 + rightNode_.position1) / 2.0f;
			new_node.lifeTime = rightNode_.lifeTime;

			split(leftNode_, new_node, depth_ + 1);
			split(new_node, rightNode_, depth_ + 1);
		}
		else
			new_node = rightNode_;

		if (!new_node.isAdded)
		{
			nodes.push_back(new_node);
			new_node.isAdded = true;
		}
	}

	void Trail::setupMesh()
	{
		for (int i = 0; i < nodes.size() && i < s_max_node_count; i++)
		{
			const auto& node = nodes[i];
			auto& v0 = m_itVertices[2 * i];
			auto& v1 = m_itVertices[2 * i + 1];

			v0.position = node.position0;
			v0.uv = Vector2f(node.lifeTime, 0.0f);

			v1.position = node.position1;
			v1.uv = Vector2f(node.lifeTime, 1.0f);
		}

		if (nodes.size() > 1)
		{
			VOG_CORE_ASSERT(m_pVertices, "");
			int count = (nodes.size() <= s_max_node_count) ? nodes.size() : s_max_node_count;
			pVertexBuffer->setData(m_pVertices, (uint32_t)(sizeof(TrailVertexLayout) * count * 2));
		}
	}

	void Trail::draw()
	{
		
	}

	void Trail::addPoint(const Vector3f& point_, const Vector3f& right_)
	{
		TrailNode node;
		node.position0 = point_ - right_ * width;
		node.position1 = point_ + right_ * width;

		if (nodes.size() == 0)
		{
			node.isAdded = true;
			nodes.push_back(node);
		}
		else if (nodes.size() < s_max_node_count)
		{
			split(nodes[nodes.size() - 1], node, 0);
		}
	}

	void Trail::onImGuiRender()
	{
		ImGuiLibrary::drawDragFloat("width", width);
		ImGuiLibrary::drawDragFloat("lifeTime", lifeTime);
		ImGuiLibrary::drawDragFloat("spiltThreshold", spiltThreshold);
	}

}
