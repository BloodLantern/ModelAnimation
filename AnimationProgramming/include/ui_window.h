#pragma once

#include <thread>

class UiWindow
{
private:
	std::thread m_Thread;
	void Main();
	void StartThread();

public:
	UiWindow();

	void* operator new(size_t count) = delete;
	void operator delete(void* ptr) = delete;

	std::thread& GetThread();
};
