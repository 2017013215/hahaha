#pragma once
#include"common.h"

class ThreadCache
{
public:
	void* Allocate(size_t size);
	void Deallocate(void* ptr, size_t size);


	//�����Ļ����ȡ�������
	void* FetchFromCentralCache(size_t index, size_t size);
	//�����ж���̫�࣬��ʼ�����ڴ�
	void ListTooLong(FreeList* freelist,size_t size);

private:
	FreeList _freelist[NLIST];
};

//ʹ�õ������ƣ����߱������˱���Ϊ�߳��ڲ�ʹ�ã�ÿ���̶߳�copyһ�ݣ������̰߳�ȫ��Ч������
static _declspec(thread) ThreadCache* tls_threadcache = nullptr;