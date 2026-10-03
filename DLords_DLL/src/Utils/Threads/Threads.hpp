#pragma once

struct thread_info_t
{
	thread_info_t(const std::string& sName, bool isWorking) :
		isWorking(isWorking), sName(sName)
	{
	}

	bool isWorking;

	std::string sName;
};

using fnThreadFunc_t = std::function<DWORD(const thread_info_t&)>;

struct thread_t
{
	thread_t(const std::string& sName, const fnThreadFunc_t& fnFunction, bool isWorking) :
		info(sName, isWorking),
		fnFunction(fnFunction), thread{}
	{
	}

	thread_info_t info;

	fnThreadFunc_t fnFunction;
	std::thread thread;
};

class CThread
{
private:
	thread_t m_data;

public:
	CThread() = default;
	~CThread() = default;

	explicit CThread(const std::string& sName, const fnThreadFunc_t& fnFunction, bool isWorking = true);

	void Start();
	void Stop();

	const thread_t& GetData() const;
	static std::vector<CThread*>& GetThreads();
};