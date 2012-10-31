// Author: Zander Clucas
// 2013 Capstone Project
// input.h
// Handles input
#ifndef ZEUS_INPUT_H
#define ZEUS_INPUT_H

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	void Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool m_keys[256];
};

#endif