#pragma once
#include "videodriver.h"
#include "vulkancontext.h"
#include "vulkanswapchain.h"
#include "vulkanrenderpass.h"

class VulkanDriver: public VideoDriver {
public:
	class VBLinkVk : public VBLink{
		public:
		VBLinkVk(const VertexBuffer *buffer) : VBLink(buffer) {};

	};

	VulkanDriver(SolisDevice *);
	~VulkanDriver();

	VBLink *getBufferLink(const VertexBuffer *) const 	{ return nullptr; };
	VBLink *createBuffer(const VertexBuffer *)			{ return nullptr; };
	void drawVertexBuffer(const VertexBuffer *) const	{};
	void drawBufferLink(VBLink *) const					{};
	void deleteBuffer(VBLink *)							{};

	void addShaderFromFile(const std::string &)			{};
	void bindShader() const								{};
	void deleteShader()									{};

	Texture *getTexture(const std::string &)			{ return nullptr; };
	void bindTexture(Texture *)							{};

	inline void clearScreenBuffer() 					{};

private:
	VulkanContext context;
	VulkanSwapchain swapchain;
	VulkanRenderPass renderpass;
};