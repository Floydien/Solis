#pragma once
#include "solisdevice.h"
#include "guiimage.h"
#include <functional>

class GUIButton {
public:
	GUIButton(const std::string& name, const Rectangle &rect = Rectangle()) :
		name(std::move(name)),
		transform(rect) {};

	void init(Scene *);
	void update(std::shared_ptr<SolisDevice>);
	void draw(const VideoDriver *) const;

	inline void setCallback(std::function<void()> cb) { callback = cb; };
private:
	std::string name;
	Rectangle transform;
	std::unique_ptr<GUIImage> image;

	std::function<void()> callback;
};