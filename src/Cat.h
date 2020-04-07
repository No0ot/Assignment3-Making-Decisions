#pragma once
#ifndef __CAT__
#define __CAT__

#include "DisplayObject.h"

class Cat : public DisplayObject
{
public:
	Cat();
	~Cat();

	// DisplayObject LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
private:
	int m_currentFrame;
	int m_currentRow;
};

#endif /* defined (__CAT__) */