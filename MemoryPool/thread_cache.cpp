#include"common.h"
#include"thread_cache.h"
#include"central_cache.h"

//���̻߳��������ڴ�
void* ThreadCache::Allocate(size_t size)
{
	assert(size < MAXBYTES);
	size_t _size = Sizeclass::RoundUp(size);
	size_t _index = Sizeclass::Index(size);
	if (this->_freelist[_index].IsEmpty())
	{
		return FetchFromCentralCache(_index, _size);
	}
	else
	{
		void* list = _freelist[_index].Pop();
		return list;
	}
}


//���̻߳����ͷ��ڴ�
void ThreadCache::Deallocate(void* ptr, size_t size)
{
	size_t _index = Sizeclass::Index(size);
	_freelist[_index].Push(ptr);

	//������������󳬹�һ���������������ʱ
	//��ʼ���ն������Ļ���
	if (_freelist[_index].Size() >= _freelist[_index].MaxSize())
	{
		ListTooLong(&_freelist[_index], size);
	}
}

//�̻߳��������Ļ��������ڴ�
void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
	//����������Ķ���Ĵ�С��Ҫ��Ƶ�ʻ�����������
	size_t num = min(Sizeclass::NumMoveSize(size),this->_freelist->MaxSize());
	void* start, *end;
	size_t fetchnum = CentralCache::GetInstance()->FetchRangeObj(start, end, num, size);
	if (fetchnum > 1)
	{
		_freelist[index].PushRange(GetAddr(start), end, fetchnum - 1);
	}
	if(num==_freelist->MaxSize())
	{
		_freelist->SetMaxSize(num + 1);
	}
	return start;

}


void ThreadCache::ListTooLong(FreeList* freelist, size_t size)
{
	void* start = freelist->clear();
	CentralCache::GetInstance()->ReleaseListToSpans(start, size);
}