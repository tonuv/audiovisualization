#pragma once
#include <mfapi.h>
#include <Mftransform.h>
#include <wrl.h>
#include <windows.foundation.diagnostics.h>

// Helper class to log stop event automatically when object goes out of scope
class CLogActivityHelper
{
	Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Diagnostics::ILoggingActivity> m_spActivity;
public:
	CLogActivityHelper(ABI::Windows::Foundation::Diagnostics::ILoggingActivity *pActivity)
	{
		m_spActivity = pActivity;
	}
	// If destructed when going out of scope log stop event with same name
	~CLogActivityHelper()
	{
		using namespace ABI::Windows::Foundation::Diagnostics;
		using namespace Microsoft::WRL;
		ComPtr<ILoggingActivity2> m_spActivity2;
		m_spActivity.As(&m_spActivity2);
		HSTRING hEventName = nullptr;
		m_spActivity->get_Name(&hEventName);
		m_spActivity2->StopActivity(hEventName);
	}
};

class Trace
{
	static Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Diagnostics::ILoggingTarget> g_spLogChannel;
	static HRESULT GetMediaProperties(Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Diagnostics::ILoggingFields> *ppFields,IMFMediaType *);
	static HRESULT CreateLoggingFields(Microsoft::WRL::ComPtr<ABI::Windows::Foundation::Diagnostics::ILoggingFields> *ppFields);
public:
	static HRESULT Initialize();
	static HRESULT Log_ProcessInput(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **pActivity, DWORD dwInputStreamID, IMFSample *pSample, DWORD dwFlags,REFERENCE_TIME presentationTime);
	static HRESULT Log_ProcessOutput(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **pActivity,DWORD dwFlags,DWORD cOutputBufferCount, MFT_OUTPUT_DATA_BUFFER *pBuffer,REFERENCE_TIME presentationTime);
	static HRESULT Log_ProcessEvent(DWORD dwStreamID, IMFMediaEvent *pEvent);
	static HRESULT Log_ProcessMessage(MFT_MESSAGE_TYPE msgType, ULONG_PTR ulParam);
	static HRESULT Log_TestFrame(REFERENCE_TIME presentationTime, REFERENCE_TIME frameStart, REFERENCE_TIME frameEnd);
	static HRESULT Log_SetInputType(DWORD dwStreamID, IMFMediaType *);
	static HRESULT Log_SetOutputType(DWORD dwStreamID, IMFMediaType *);
	static HRESULT Log_ConfigureAnalyzer(UINT32 samplesPerAnalyzerOutputFrame, UINT32 overlap, UINT32 fftLength, HRESULT hConfigureResult);
	static HRESULT Log_StartGetFrame(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **pActivity,REFERENCE_TIME presentationTime,const REFERENCE_TIME *queueTimes,size_t queueSize);
	static HRESULT Log_FrameFound(REFERENCE_TIME start, REFERENCE_TIME duration);
	static HRESULT Log_FrameNotFound();
	static HRESULT Log_StartAnalyzerStep(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity);
	static HRESULT Log_StopAnalyzerStep(ABI::Windows::Foundation::Diagnostics::ILoggingActivity *pActivity,REFERENCE_TIME time,HRESULT hResult);
	static HRESULT Log_BeginAnalysis();
	static HRESULT Log_AnalysisAlreadyRunning();
	static HRESULT Log_Configure(float outFrameRate, float overlapPercentage, unsigned fftLength);
	static HRESULT Log_SetLogFScale(float lowFrequency, float highFrequency, unsigned outElementCount);
	static HRESULT Log_SetLinearScale(size_t numberOfBins);
	static HRESULT Log_StartOutputQueuePush(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity,REFERENCE_TIME time,const REFERENCE_TIME *pTimes, size_t queueSize);

	static HRESULT Log_SA_Start_AppendInput(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity, REFERENCE_TIME sampleTime, size_t sampleCount,size_t samplesInBuffer,void *pWritePtr,void *pReadPtr,long inputSampleOffset, long expectedOffset);
	static HRESULT Log_SA_Stop_AppendInput(ABI::Windows::Foundation::Diagnostics::ILoggingActivity *pActivity, REFERENCE_TIME sampleTime, size_t sampleSize,size_t samplesInBuffer, void *pWritePtr, void *pReadPtr, long expectedOffset);
	static HRESULT Log_SA_ClearInputBuffer();

	static HRESULT Log_StartCopyRBData(ABI::Windows::Foundation::Diagnostics::ILoggingActivity **ppActivity,size_t bufferSize,const void *pReadPtr,const void *pWritePtr);
	static HRESULT Log_StopCopyRBData(ABI::Windows::Foundation::Diagnostics::ILoggingActivity *pActivity, bool bSuccess, size_t bufferSize,const void *pReadPtr,const void *pWritePtr);

	// Func needs to confirm to HRESULT function()
	template<class Func> inline
		static HRESULT TrackActivity(const wchar_t*pwszActivityName, Func f)
	{
		using namespace ABI::Windows::Foundation::Diagnostics;
		using namespace Microsoft::WRL;
		using namespace Microsoft::WRL::Wrappers;
		ComPtr<ILoggingActivity> spActivity;
		auto eventName = HStringReference(pwszActivityName);
		Trace::g_spLogChannel->StartActivity(eventName.Get(), &spActivity);
		HRESULT hr = f();	// Perform the activity
		ComPtr<ILoggingFields> spFields;
		Trace::CreateLoggingFields(&spFields);
		spFields->AddInt32WithFormat(HStringReference("HResult").Get(), hr, LoggingFieldFormat::LoggingFieldFormat_HResult);
		ComPtr<ILoggingActivity2> spActivity2;
		spActivity.As(&spActivity2);
		spActivity2->StopActivityWithFields(eventName.Get(), spFields.Get());
		return hr;
	}

	// Func needs to confirm to HRESULT function(const ILoggingTarget *)
	template<class Func> inline
		static HRESULT TrackActivityWithEvents(const wchar_t*pwszActivityName, Func f)
	{
		using namespace ABI::Windows::Foundation::Diagnostics;
		using namespace Microsoft::WRL;
		using namespace Microsoft::WRL::Wrappers;
		ComPtr<ILoggingActivity> spActivity;
		auto eventName = HStringReference(pwszActivityName);
		Trace::g_spLogChannel->StartActivity(eventName.Get(), &spActivity);
		HRESULT hr = f(Trace::g_spLogChannel.Get());	// Perform the activity
		ComPtr<ILoggingFields> spFields;
		Trace::CreateLoggingFields(&spFields);
		spFields->AddInt32WithFormat(HStringReference("HResult").Get(), hr, LoggingFieldFormat::LoggingFieldFormat_HResult);
		ComPtr<ILoggingActivity2> spActivity2;
		spActivity.As(&spActivity2);
		spActivity2->StopActivityWithFields(eventName.Get(), spFields.Get());
		return hr;
	}
};

