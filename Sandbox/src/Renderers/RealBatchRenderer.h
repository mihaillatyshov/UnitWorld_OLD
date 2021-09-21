#pragma once

#include "Renderer.h"
#include "Core/Shader.h"
#include "Core/Material.h"

#include <condition_variable>
#include <iostream>
#include <random>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <memory>

#include "ThreadPool/ThreadPool.h"

//struct SceneInfo
//{
//
//};

#define MAX_TRIANGLES 100000

#define MAX_VERTICES (MAX_TRIANGLES * 3)

#define USE_MULTI_DRAW_ELEMENTS false
#define USE_SUB_DATA false

#define USE_MT false
#define USE_MT_QUEUE true

#define TEST_MT false

class Renderable;

struct FlushThreadedData
{
	bool						Done = false;
	std::mutex					LockQueue;
	std::condition_variable		QueueCheck;


	bool						Notified = false;
};


struct ThreadedData
{
	bool						Done = false;
	std::mutex					LockQueue;
	std::condition_variable		QueueCheck;


	bool						Notified = false;
	Renderable*					AddData;
	short						FoundedMaterailId;
	glm::mat4					Transform;
};

using Cb = std::function<void(void)>;

struct ThreadedDataQueue
{
	bool						Done = false;
	std::mutex					LockQueue;
	std::condition_variable		QueueCheck;


	bool						Notified = false;
	std::vector<Cb>				FooQueue;
};

struct IndicesBuffer // Create without using multiDrawArrays
{
#if USE_MULTI_DRAW_ELEMENTS
	IndicesBuffer();
	~IndicesBuffer() { }
	void Add(const std::vector<uint32_t> &indices, uint32_t VerticesCount);
	void Reset();
	void StartScene() { }
	void EndScene() { }
	void Bind() { }

	std::vector<std::vector<uint32_t>> m_IndicesVector;
	std::vector<void*> m_Indices;
	std::vector<int> m_Counts;
	uint32_t m_IndicesVectorId;
	uint32_t m_Size;
	int m_MaxIndex;
	int m_ObjectsCount;
#else
	IndicesBuffer();
	~IndicesBuffer();

	void Bind();
	void StartScene();
	void EndScene();
	void Add(const std::vector<uint32_t> &indices, uint32_t VerticesCount);
	void Reset();

	uint32_t m_BufferID;
	uint32_t* m_Buffer;

	int m_ObjectsCount;
	uint32_t m_MaxIndex;
	int m_Size;

#endif
};

class SubBuffer
{
public:
	SubBuffer() = default;
	virtual ~SubBuffer() = default;

	virtual void StartScene() { };
	virtual void EndScene() { };
	virtual bool AddObject(Renderable* renderable, const glm::mat4& transform) = 0;
	const IndicesBuffer& GetIndicesBuffer() { return m_IndexBuffer; }

	virtual void BindVAO();
	void BindIBO();
	void BindTransformTexture(int slot = 0);
	void SetMaterials(Shader* shader);
	void SetTextures();
	void ClearBuffers();
protected:

	uint32_t m_VAO;

	uint32_t m_VBO_MaterialId;
	uint32_t m_VBO_TransformId;

	short* m_MaterialsBuffer;
	short* m_TransformsBuffer;

	uint32_t m_TransfortionTexture;
	uint32_t m_MatrixTextureBuffer;
	glm::mat4* m_MatrixBuffer;

	std::vector<const Material*> m_Materials; // std::vector<uint32_t> m_Materials; /* Use with Material manager!!! */
	std::vector<MaterialInfo> m_MaterialsInfo;
	std::vector<uint32_t> m_Texures;
	std::vector<std::shared_ptr<Texture>> m_TexturePointers;

	IndicesBuffer m_IndexBuffer;


	//bool IsWriting();
	//
	//glGenBuffers(1, &m_MatrixBuffer);
	//glBindBuffer(GL_TEXTURE_BUFFER, m_MatrixBuffer);
	//glBufferData(GL_TEXTURE_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STATIC_DRAW);
	//
	//glGenTextures(1, &m_TextureBuffer);
	////glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_BUFFER, m_TextureBuffer);
	//glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_MatrixBuffer);
};

class SimpleSubBuffer : public SubBuffer
{
protected:
public:
	SimpleSubBuffer();
	virtual ~SimpleSubBuffer();
	
	virtual void StartScene() override;
	virtual void EndScene() override;
	virtual bool AddObject(Renderable* renderable, const glm::mat4& transform) override;
	std::future<void> UpdateBuffers();

	void TestAdding() { }
protected:

	uint32_t m_VBO_Positions;
	uint32_t m_VBO_UVs;
	uint32_t m_VBO_Normals;
	uint32_t m_VBO_Tangents;

	std::vector<uint32_t> m_FMID;
	std::vector<glm::mat4> m_Transforms;
	std::vector<Renderable*> m_Objects;

	glm::vec3 *m_PositionsBuffer;
	glm::vec2 *m_UVsBuffer;
	glm::vec3 *m_NormalsBuffer;
	glm::vec3 *m_TangentsBuffer;

#if USE_MT
#if USE_MT_QUEUE

	std::thread t1;
	ThreadedDataQueue t1queue;
	std::thread t2;
	ThreadedDataQueue t2queue;
	std::thread t3;
	ThreadedDataQueue t3queue;
	std::thread t4;
	ThreadedDataQueue t4queue;
	std::thread t5;
	ThreadedDataQueue t5queue;

#else

	std::thread BufferThread;
	ThreadedData BufferTD;

#endif
#endif
};


class SimpleBufferManager
{
public:
	SimpleBufferManager();
	~SimpleBufferManager();

	void AddObject(Renderable* renderable, const glm::mat4& transform);
	void AddBuffer();

	void DrawBuffer(const Camera *camera, const Projection &projection, Lights* lights);

	void StartScene();
	void EndScene();
	void ClearBuffers();
	void EnableShader(const Camera *camera, const Projection &projection, Lights* lights);
	void BindBuffer(uint32_t id);
	inline size_t GetBufferSize() { return m_Buffers.size(); }
	inline size_t GetObjectsCount(uint32_t BufferId) { return m_Buffers[BufferId]->GetIndicesBuffer().m_ObjectsCount; }
	inline int GetIndicesCount(uint32_t BufferId) { return m_Buffers[BufferId]->GetIndicesBuffer().m_Size; }
	
	void UpdateBuffers();
	
public:
	//static inline std::mutex s_Mtx;
protected:
	Shader* m_Shader;
	std::vector<SimpleSubBuffer*> m_Buffers;
};


class RealBatchRenderer : public Renderer
{
public:
	RealBatchRenderer();
	virtual ~RealBatchRenderer();

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;

	virtual void PushTransform(glm::mat4 *transform, bool override = false) override;
	virtual void PopTransform() override;
	
	void BeginScene(Camera *camera, const Projection &projection, Lights* lights) override;
protected:
	void Submit(Renderable* renderable) override; // Make this protected (and friend to renderable)
	void Submit(AnimatedRenderable* renderable) override { } // TODO
	void Flush() override;
protected:
	Camera* m_Camera;
	Projection m_Projection;
	Lights* m_Lights;

	//Shader *m_Shader;
	SimpleBufferManager* m_SimpleBuffer;
	
	std::vector<glm::mat4> m_TransformationStack;
	glm::mat4* m_TransformationBack;
public:
	static inline int s_MaxTexturesToDraw = 0;
	static inline int s_NotSimpleTextures = 0;

#if TEST_MT
	static ThreadPool s_ThreadPool;
#endif
	//SimpleSubBuffer m_Objects;

	// TEST TRANSFORM TREE 
};