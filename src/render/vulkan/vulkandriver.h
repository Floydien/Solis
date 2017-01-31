#pragma once
#include "videodriver.h"
#include <vulkan/vulkan.h>
#include "vulkanbuffer.h"
#include "vulkanqueue.h"
#include "vulkanswapchain.h"
#include "vulkanrenderpass.h"
#include "vulkanshader.h"
#include "vulkandescriptorpool.h"
#include "vulkanpipelinelayout.h"
#include "vulkangraphicspipeline.h"

class VulkanDevice;

class VulkanDriver: public VideoDriver {
public:
	class VBLinkVk : public VBLink{
	public:
		VBLinkVk(std::unique_ptr<VulkanBuffer> _vertexBufferLink, std::unique_ptr<VulkanBuffer> _indexBufferLink, std::shared_ptr<const VertexBuffer> vertexBuffer) :
				VBLink(vertexBuffer), vertexBufferLink(std::move(_vertexBufferLink)), indexBufferLink(std::move(_indexBufferLink)) {};
		~VBLinkVk(){};

		std::unique_ptr<VulkanBuffer> vertexBufferLink;
		std::unique_ptr<VulkanBuffer> indexBufferLink;
	};

	VulkanDriver(SolisDevice *);
	~VulkanDriver();

	VBLink *getBufferLink(std::shared_ptr<const VertexBuffer>) const;
	VBLink *createBuffer(std::shared_ptr<const VertexBuffer>) 		;
	void drawVertexBuffer(std::shared_ptr<const VertexBuffer>) const 	{};
	void drawBufferLink(VBLink *) const{};
	void deleteBuffer(VBLink *){};

	void addShaderFromFile(const std::string &)			{};
	void bindShader(const std::string &)				{};
	Shader *getActiveShader() const						{ return nullptr; };
	void deleteShader(const std::string &)				{};

	Texture *getTexture(const std::string &)			{ return nullptr; };
	void bindTexture(Texture *)	const					{};

	inline void clearScreenBuffer() 					{};

private:
};