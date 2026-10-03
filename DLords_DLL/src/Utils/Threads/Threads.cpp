#include "precompiled.hpp"

static DWORD HandleException(_EXCEPTION_POINTERS* e)
{
	//MessageBoxA(0, "CRASH", "", MB_SYSTEMMODAL);
	return EXCEPTION_EXECUTE_HANDLER;
}

static DWORD WINAPI ThreadWrapper(const thread_t& data)
{
	DWORD result = 0;

	__try
	{
		if (!data.info.isWorking)
		{
			ChronoMeter WaitForThread;

			while (!data.info.isWorking)
			{
				std::this_thread::sleep_for(100ms);

				if (WaitForThread.has_elapsed(60s))
				{
					assert(!"Waited for 60s and exited");
					return 0;
					break;
				}
			}

			result = data.fnFunction(data.info);
		}
		else
		{
			result = data.fnFunction(data.info);
		}
	}
	__except (HandleException(GetExceptionInformation()))
	{
		//assert(false);

		LOG_ERROR("[Thread] Unhandler exception in thread [{}]", data.info.sName);
	}

	return result;
}

CThread::CThread(const std::string& sName, const fnThreadFunc_t& fnFunction, bool isWorking) :
	m_data(sName, fnFunction, isWorking)
{
	m_data.thread = std::thread(ThreadWrapper, std::reference_wrapper(m_data));
	m_data.thread.detach();

	GetThreads().push_back(this);
}

void CThread::Start()
{
	if (!m_data.info.isWorking)
		m_data.info.isWorking = true;
}

void CThread::Stop()
{
	if (m_data.info.isWorking)
		m_data.info.isWorking = false;
}

std::vector<CThread*>* gThreads;

const thread_t& CThread::GetData() const
{
	return m_data;
}

std::vector<CThread*>& CThread::GetThreads()
{
	if (!gThreads)
		gThreads = new std::vector<CThread*>();

	return *gThreads;
}