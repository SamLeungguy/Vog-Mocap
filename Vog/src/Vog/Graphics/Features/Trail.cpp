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

	void Trail::update(float dt_, const Vector3f& point_, const Vector3f& right_)
	{
		updateNode(dt_, point_, right_);

		setupMesh();

		/*draw();

		for (size_t i = 0; i < nodes.size(); i++)
		{
			nodes[i].lifeTime += dt_;
		}

		nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
			[&](const TrailNode& node_)
			{
				return node_.lifeTime > lifeTime;
			}), nodes.end());

		VOG_CORE_LOG_INFO("Node size: {0}", nodes.size());*/

	}

	void Trail::split(TrailNode& leftNode_, TrailNode& rightNode_, int depth_)
	{
		/*static int max_depth = 6;
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
		}*/
	}

	void Trail::setupMesh()
	{
		int n = nodes.size() - m_currentNode;
		if (n <= 0)
			return;

		for (int i = 0; i < n; i++)
		{
			const auto& node = nodes[m_currentNode + i];
			auto& v0 = m_itVertices[2 * i];
			auto& v1 = m_itVertices[2 * i + 1];

			v0.position = node.position0;
			v0.uv = Vector2f(node.lifeTime, 0.0f);

			v1.position = node.position1;
			v1.uv = Vector2f(node.lifeTime, 1.0f);
		}

		VOG_CORE_ASSERT(m_pVertices, "");
		int count = (nodes.size() <= s_max_node_count) ? n : s_max_node_count;
		pVertexBuffer->setData(m_pVertices, (uint32_t)(sizeof(TrailVertexLayout) * count * 2));
	}

	void Trail::addPoint(const Vector3f& point_, const Vector3f& right_)
	{
		TrailNode node;
		node.position0 = point_ /*- right_ * width*/;
		node.position1 = point_ + right_ * width;

		if (nodes.size() == 0)
		{
			nodes.push_back(node);
		}
		else/* if (nodes.size() < s_max_node_count)*/
		{
			addNode(node, 0);
		}
	}

	void Trail::addNode(TrailNode& node_, int depth_)
	{
		constexpr int max_depth = 8;
		if (depth_ > max_depth)
			return;

		const auto& last = nodes.back();
		auto mid0 = (node_.position0 + node_.position0) / 2.0f;
		auto mid1 = (node_.position1 + node_.position1) / 2.0f;

		float distance = MyMath::distance(mid1, mid0);
		float error = MyMath::absf(distance - width);
		if (error < tolerance)
		{
			nodes.push_back(node_);
			return;
		}

		TrailNode mid_node;
		mid_node.isBreakdown = true;
		mid_node.lifeTime = (last.lifeTime + node_.lifeTime) / 2.0f;
		mid_node.position0 = mid0;
		mid_node.position1 = mid0 + MyMath::normalize(mid1 - mid0) * width;

		addNode(mid_node, depth_ + 1);
		addNode(node_, depth_ + 1);
	}

	void Trail::updateNode(float dt_, const Vector3f& point_, const Vector3f& right_)
	{
		int n = nodes.size();
		for (int i = m_currentNode; i < n; i++)
		{
			nodes[i].lifeTime += dt_;
			if (nodes[i].lifeTime < lifeTime)
				m_currentNode = i + 1;
		}

		if (m_currentNode >= nodes.size())
		{
			nodes.clear();
			m_currentNode = 0;
		}
		else
		{
			int used = nodes.size() - m_currentNode;
			if (nodes.size() > s_max_node_count && m_currentNode > used * 4)
			{
				nodes.erase(nodes.begin(), nodes.begin() + m_currentNode);
				m_currentNode = 0;
			}
		}
		addPoint(point_, right_);
	}

	void Trail::onImGuiRender()
	{
		ImGuiLibrary::drawDragFloat("width", width);
		ImGuiLibrary::drawDragFloat("lifeTime", lifeTime);
		ImGuiLibrary::drawDragFloat("tolerance", tolerance);

		ImGuiLibrary::drawTextWithValue("Node size", nodes.size());
		ImGuiLibrary::drawTextWithValue("current Node", m_currentNode);

	}

}
