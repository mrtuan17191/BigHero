/*
 * UGMap.h
 *
 *  Created on: Dec 17, 2012
 *      Author: tony
 */

#ifndef UGMAP_H
#define UGMAP_H

#include <iostream>
#include <new>
#include <stdio.h>

using namespace std;

template <class K, class V>
class UGMap {
private:
    int _count;
    K* keyArr;
    V* valueArr;
public:
	UGMap(void){
		this->_count = 0;
		this->keyArr = NULL;
        this->valueArr = NULL;
	}

	virtual ~UGMap(void){
		delete[] keyArr;
        delete[] valueArr;
	}
    
	static UGMap* create(int capacity){
		UGMap* instance = new UGMap();
		instance->keyArr = new (nothrow) K[capacity];
        instance->valueArr = new (nothrow) V[capacity];
		return instance;
	}
    
    void add(K key, V value){
		if(containsKey(key)){
			int index = getKeyIndex(key);
			if(index > -1){
				valueArr[index] = value;
			}
		} else {
			keyArr[_count] = key;
			valueArr[_count] = value;
			_count++;
		}
	}

    K getKey(int index){
		return keyArr[index];
	}
    
    V getValue(int index){
        return valueArr[index];
    }
    
    bool containsKey(K key){
        for(int i =0; i<_count; i++){
			if(keyArr[i] == key){
				return true;
			}
		}
        return false;
    }
    
	int getKeyIndex(K key){
        for(int i =0; i<_count; i++){
			if(keyArr[i] == key){
				return i;
			}
		}
        return -1;
    }

    V get(K key){
        for(int i =0; i<_count; i++){
			if(keyArr[i] == key){
				return valueArr[i];
			}
		}
        return NULL;
    }

	void remove(K key){
		int index = -1;
		for(int i =0; i<_count; i++){
			if(keyArr[i] == key){
				index = i;
				break;
			}
		}
		if(index != -1){
			for(int i = index; i<_count-1; i++){
				keyArr[i] = keyArr[i+1];
                valueArr[i] = valueArr[i+1];
			}
			keyArr[_count-1] = NULL;
            valueArr[_count-1] = NULL;
			_count--;
		}
	}

    int count(void){
		return this->_count;
	}
};
#endif /* ETT_ARRAY_H */
