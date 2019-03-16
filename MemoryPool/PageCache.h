#pragma once
#include"common.h"

class PageCache
{
public:
	static PageCache* GetInstance()
	{
		return &Inst;
	}

	Span* NewSpan(size_t npage);
	Span* _NewSpan(size_t npage);
	//��ôӶ���span��ӳ��
	Span* MapObjectToSpan(void* obj);
	//�ͷſռ�span��Pagecache�����ϲ����ڵ�span
	void ReleaseSpanToPageCache(Span* span);
private:
	SpanList pagelist[NPAGES];
private:
	static PageCache Inst;
	PageCache() = default;
	PageCache(const PageCache&) = delete;
	PageCache& operator=(const PageCache&) = delete;

	std::mutex _mtx;
	std::map<PageID, Span*> _id_span_map;
};

