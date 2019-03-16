#pragma once
#include"common.h"

//�����̰߳�ȫ�����õ���ģʽ
class CentralCache
{
public:
	static CentralCache* GetInstance()
	{
		return &Inst;
	}
	//�����Ļ���ȡ��һ�����ڴ���̻߳���
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte);
	
	//��һ�������ڴ��ͷŵ�span���
	void ReleaseListToSpans(void* strat, size_t byte_size);

	Span* GetOneSpan(SpanList* spanlist, size_t size);
private:
	SpanList spanlist[NLIST]; //

private:
	static CentralCache Inst;

	CentralCache() = default;
	CentralCache(const CentralCache&) = delete;
	CentralCache& operator= (const CentralCache&) = delete;
};
