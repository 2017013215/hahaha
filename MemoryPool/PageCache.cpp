#include"PageCache.h"
PageCache PageCache::Inst;



Span* PageCache::NewSpan(size_t npage)
{
	//����ݹ����
	//std::unique_lock<std::mutex> lock(_mtx);
	//��λ���������ڴ�
	_mtx.lock();
	Span* span = _NewSpan(npage);
	_mtx.unlock();
	return span;
}

Span* PageCache::_NewSpan(size_t npage)
{
	//������ҳ����NPAGES�ֱ࣬������һ����Ӧ��С��span����
	if (npage >= NPAGES)
	{
		void* ptr = VirtualAlloc(NULL, (npage) << PAGE_SHIFT, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		Span* newspan = new Span;
		newspan->id = (PageID)ptr >> PAGE_SHIFT;
		newspan->npage = npage;
		newspan->objsize = npage << PAGE_SHIFT;
		
		for (size_t i = 0; i < npage; i++)
		{
			_id_span_map[newspan->id + i] = newspan;
		}
		return newspan;
	}

	if (!pagelist[npage].Empty())
	{
		return pagelist->PopFront();
	}

	//���±������ҵ����ڴ��λ�ã����ڴ�����ط���
	for (size_t i = npage + 1; i < NPAGES; i++)
	{
		if (!pagelist[i].Empty())
		{
			Span* span = pagelist[i].PopFront();
			Span* newspan = new Span;
			newspan->id = span->id + span->npage - npage;
			newspan->npage = npage;
			span->npage -= npage;
			for (size_t i = 0; i < newspan->npage; i++)
			{
				_id_span_map[newspan->id + i] = newspan;
			}
			pagelist[span->npage].PushFront(span);
			//std::cout << "fengehou" << span->id<<std::endl;

			return newspan;
		}
	}

	//û�п����ڴ���Ҫ��ϵͳ�����ڴ� 
	void* ptr = VirtualAlloc(NULL, (NPAGES - 1) << PAGE_SHIFT, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (ptr == nullptr)
	{
		throw std::bad_alloc();
	}

	Span* largespan = new Span;
	largespan->id = (PageID)ptr >> PAGE_SHIFT;
	largespan->npage = NPAGES - 1;
	//����ӳ���ϵ
	for (size_t i = 0; i < NPAGES; i++)
	{
		_id_span_map[largespan->id + i] = largespan;
	}

	pagelist[NPAGES - 1].PushFront(largespan);
	//std::cout << "diyicihuoqu" << largespan->id<<std::endl;
	return _NewSpan(npage);
}


Span* PageCache::MapObjectToSpan(void* obj)
{
	//ͨ����ַ���ҳ��,һ��ҳռ4k
	PageID pageid = (PageID)obj >> PAGE_SHIFT;
	//ͨ��ҳ���ҵ�span����
	auto it = _id_span_map.find(pageid);
	assert(it != _id_span_map.end());

	return it->second;
}


void PageCache::ReleaseSpanToPageCache(Span* span)
{
	std::unique_lock<std::mutex> lock(_mtx);
	if (span->npage >= NPAGES)
	{
		void* ptr = (void*)(span->id << PAGE_SHIFT);
		VirtualFree(ptr,0,MEM_RELEASE);
		_id_span_map.erase(span->id);
		delete span;
		return;
	}


	//�ҵ�����ǰһ��span
	auto previt = _id_span_map.find(span->id - 1);
	while (previt != _id_span_map.end())
	{
		Span* prevspan = previt->second;
		//�������õ�ֱ������
		if (prevspan->usecount != 0)
			break;

		//�ϲ�����NPAGESҳ��spanҲ���к���
		if (prevspan->npage + span->npage >= NPAGES)
			break;

		pagelist[prevspan->npage].Erase(prevspan);
		prevspan->npage += span->npage;
		delete span;
		span = prevspan;
		//����һ��span
		previt = _id_span_map.find(span->id - 1);
	}
	//�Ҹ�span��ַ���������span
	auto nextit = _id_span_map.find(span->id + span->npage);
	while (nextit != _id_span_map.end())
	{
		Span* nextspan = nextit->second;
		if (nextspan->usecount != 0)
			break;
		if (nextspan->npage + span->npage >= NPAGES)
			break;
		
		pagelist[nextspan->npage].Erase(nextspan);
		span->npage += nextspan->npage;
		delete nextspan;
		//����һ��span
		nextit = _id_span_map.find(span->id+span->npage);
	}
	//�����µ�ӳ���ϵ
	for (size_t i = 0; i < span->npage; ++i)
	{
		_id_span_map[span->id + i] = span;
	}
	//std::cout << "hebinghou" << span->id << std::endl;
	pagelist[span->npage].PushFront(span);
}

