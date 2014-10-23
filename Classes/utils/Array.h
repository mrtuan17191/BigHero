/*
 * Array.h
 *
 *  Created on: Dec 17, 2012
 *      Author: tony
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <new>
#include <stdio.h>

using namespace std;

template <class T>
class Array {
private:
    int count;
    T* arr;
public:
	Array(void){
		this->count = 0;
		this->arr = NULL;
	}

	virtual ~Array(void){
		delete[] arr;
	}
    
	static Array* create(int capacity){
		Array* instance = new Array();
		instance->arr = new (nothrow) T[capacity];
		return instance;
	}
    
    void add(T e){
		arr[count] = e;
		count++;
	}

    T get(int index){
		return arr[index];
	}

    bool contains(T e){
    	int index = -1;
		for(int i =0; i<count; i++){
			if(arr[i] == e){
				index = i;
				break;
			}
		}
		if(index != -1){
			return true;
		}
		return false;
    }

	void remove(T e){
		int index = -1;
		for(int i =0; i<count; i++){
			if(arr[i] == e){
				index = i;
				break;
			}
		}
		if(index != -1){
			for(int i = index; i<count-1; i++){
				arr[i] = arr[i+1];
			}
			arr[count-1] = NULL;
			count--;
		}
	}

    int size(void){
		return this->count;
	}
};
#endif /* ETT_ARRAY_H */
