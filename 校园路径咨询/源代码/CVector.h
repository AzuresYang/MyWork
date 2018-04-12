#ifndef _CVECTOR_H_
#define _CVECTOR_H_
#include<assert.h>
template<class T> class CVector 
{
public:
	unsigned int capacity;//当前容量
	unsigned int length;//当前储存个数
	static const unsigned MAXSIZE = 62350;//比62353小一些，最大容量
	T* data;
	CVector();
	CVector(CVector& copy);
	CVector(int count, T val);
	void add(T t);
//	void add(T* t);
	T* erase(T* t);
	T* erase(const unsigned int index);
	T& operator[](const int i);
	CVector<T>& operator=( CVector& copy);
	T* begin();
	T* end();
	void Clear() { length = 0; }
	void deleteAll();
	~CVector() {
		delete[]data;
	}
	
private:
	static const unsigned int increment = 10;
	void Init();
	void expand();
	void expand(unsigned int count);
};

template<class T> void  CVector<T>::Init()
{
	data = new T[increment];
	capacity = increment;
	length = 0;
}

template<class T> CVector<T>::CVector()
{
	Init();
}

template<class T> CVector<T>::CVector(CVector& copy)
{
	Init();
	UINT rest = capacity - length;
	UINT restNeed = copy.length - length;
	if (rest < restNeed)
		expand(restNeed - rest);
	for (unsigned int i = 0; i < copy.length; i++)
		data[i] = copy[i];
	length = copy.length;

}
template<class T> CVector<T>::CVector(int count, T val)
{
	Init();
	if (count < 0)
	{
		assert(false) ;
	}
	else
		for(int i = 1; i <= count; i++)
		add(val);
}
//Desc:扩容函数，每次扩大为上次容量的一倍。

template<class T> void CVector<T>::expand() {
	if (capacity == MAXSIZE)
	{
		assert(!(capacity == MAXSIZE));//超出容量
		return;
	}
	long expandSize;
	expandSize = capacity * 2;
	if (expandSize >= MAXSIZE)
		expandSize = MAXSIZE - capacity;
	capacity +=expandSize;
	T * temp = new T[capacity];
	for (UINT i = 0; i < length; i++)
	{
		temp[i] = data[i];
	}
	delete[]data;
	data = temp;
}

template<class T> void CVector<T>::expand(unsigned int count)
{
	if (capacity == MAXSIZE)
	{
		assert(!(capacity == MAXSIZE));//超出容量
		return;
	}
	long expandSize;
	expandSize = capacity +count;
	if (expandSize > MAXSIZE)
		expandSize = MAXSIZE - capacity -1;
	capacity += count;
	T * temp = new T[capacity];
	for (UINT i = 0; i < length; i++)
	{
		temp[i] = data[i];
	}
	delete[]data;
	data = temp;
}
template<class T> void CVector<T>::add(T t)
{
	if (capacity == MAXSIZE)
	{
		assert(!(capacity == MAXSIZE));//超出容量
		return;
	}
	if (length >= capacity - 1)//-1是因为end指针的缘故
		expand();
	data[length++] = t;
}

// template<class T> void CVector<T>::add(T* t)
// {
// 	if (length >= capacity - 1)//-1是因为end指针的缘故
// 		expand();
// 	data[length++] = t;
// }

//Desc:擦除指向的数据内容。
//Return:存在则返回擦除内容的下一个内容，不存在则放回NULL
template<class T> T* CVector<T>::erase(T *t)
{
	int index = -1;
	for (int i = 0; i < length; i++)
	{
		if (&(data[i]) == t)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		return NULL;
	else {
		//储存的数据后移
		for (unsigned int  i = index; i < length - 1; i++)
			data[i] = data[i + 1];
		length--;
	}
	return &(data[index]);
}


template<class T> T* CVector<T>::erase(const unsigned int index)
{
	if (index >= length)
	{
		assert(false);
		return NULL;
	}
	else if (length < 0)
	{
		assert(false);
		return NULL;
	}
	else {
		//储存的数据后移
		for (unsigned int i = index; i < length - 1; i++)
			data[i] = data[i + 1];
		length--;
	}
	return &(data[index]);
}

//Desc:返回数组开头内容的指针
template<class T> T* CVector<T>::begin()
{
	return &(data[0]);
}

//Desc:返回数组末尾后一位的指针 
template<class T> T* CVector<T>::end()
{
	return &(data[length]);
}


template<class T> T& CVector<T>::operator[](const int i)
{
	if (i >= (int)length)
	{
		assert(false);
		return data[0];
	}

	return data[i];
}
template<class T> CVector<T>& CVector<T>::operator=( CVector& copy)
{
	UINT rest = capacity - length;
	UINT restNeed = copy.length - length;
	if(rest < restNeed)
		expand(restNeed - rest);
	for (unsigned int i = 0; i < copy.length; i++)
		data[i] = copy[i];
	length = copy.length;
	return *this;
}

template<class T> void CVector<T>::deleteAll()
{
	for (UINT i = 0; i < length; i++)
		SAFE_DELETE(data[i]);
	length = 0;
}
#endif