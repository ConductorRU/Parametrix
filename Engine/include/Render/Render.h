#pragma once
namespace Led
{
	struct DisplayMode
	{
		uint width;
		uint height;
		uint hz;
		uint format;
	};
	class Viewport
	{
	public:
		virtual void SetX(float value) abstract;
		virtual void SetY(float value) abstract;
		virtual void SetWidth(float value) abstract;
		virtual void SetHeight(float value) abstract;
		virtual void SetMinDepth(float value) abstract;
		virtual void SetMaxDepth(float value) abstract;
		virtual float GetX() abstract;
		virtual float GetY() abstract;
		virtual float GetWidth() abstract;
		virtual float GetHeight() abstract;
		virtual float GetMinDepth() abstract;
		virtual float GetMaxDepth() abstract;
	};
	class Render
	{
	protected:
		vector<DisplayMode> _modes;
		static Render *_this;
	public:
		static Render *Get() { return _this; }
		Render();
		virtual ~Render() {};
		virtual bool Initialize(class Window *window, uint width = 1024, uint height = 768) abstract;
		virtual void Clear() abstract;
		virtual void Begin(const struct Color &clear, float depth) abstract;
		virtual void End() abstract;
		virtual void SetDefaultRenderTarget() abstract;
		float GetAspect() { return (float)GetWidth() / (float)GetHeight(); };
		virtual uint GetWidth() abstract;
		virtual uint GetHeight() abstract;
		virtual void Resize(uint width, uint height) abstract;
		virtual void SetViewport(const Viewport &port) abstract;
		virtual Viewport &GetViewport() abstract;
		void BeginScene(class Scene *scene);
		void RenderScene(class Scene *scene);
	};
}