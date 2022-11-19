#pragma once

#include <thread>
#include <mutex>
#include <memory>

template <typename T>
class TemPlateSingleton
{
private:
	explicit TemPlateSingleton() = default;
	~TemPlateSingleton() = default;
	//禁止拷贝、移动
	explicit TemPlateSingleton(const TemPlateSingleton &tem) = delete;
	TemPlateSingleton &operator=(const TemPlateSingleton &tem) = delete;
	explicit TemPlateSingleton(TemPlateSingleton &&tem) = delete;
	TemPlateSingleton &operator=(TemPlateSingleton &&tem) = delete;

public:
	static std::shared_ptr<T> GetInstance();

private:
	static std::shared_ptr<T> m_pTemplate;
};

template <typename T>
std::shared_ptr<T> TemPlateSingleton<T>::m_pTemplate = nullptr;

template <typename T>
std::shared_ptr<T> TemPlateSingleton<T>::GetInstance()
{
	if (m_pTemplate == nullptr)
	{
		static std::once_flag flag;
		std::call_once(flag, [&]()
					   { m_pTemplate = std::make_shared<T>(); });
	}

	return m_pTemplate;
}
