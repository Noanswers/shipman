#pragma once

template <class T>
class CSingleton
{
public:
	static T* GetInstance()
	{
		if (m_instance == nullptr)
			m_instance = new T();
		return m_instance;
	}

	virtual void DestorySingleton()
	{
		delete this;
	}

protected:
	virtual ~CSingleton()
	{
		delete m_instance;
		m_instance = nullptr;
	}
	CSingleton() = default;

private:
	static T* m_instance;
};

template<class T> T* CSingleton<T>::m_instance = nullptr;