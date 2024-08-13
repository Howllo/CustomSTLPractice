//
// Created by tony on 8/12/24.
//

#ifndef VECARRAY_H
#define VECARRAY_H

#include <cmath>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <memory>

template<typename T>
class VecArray {
    size_t _capacity;
    size_t _size;
    T* object;
    bool LockArray;

public:
    VecArray() {
        object = new T[1];
        _capacity = 1;
        _size = 0;
        LockArray = false;
    }

    explicit VecArray(const int32_t in_size) {
        _capacity = in_size - 1;
        object = new T[in_size];
        _size = 0;
        LockArray = false;
    }

    VecArray(std::initializer_list<T> list) {
        _capacity = list.size();
        object = new T[list.size()];
        _size = 0;
        LockArray = false;

        for(auto Element : list) {
            push_back(Element);
        }
    }

    ~VecArray() {
        delete[] object;
    }

    /**
     * Used to get the current used element in the array.
     * @return The currently size_t element in the array.
     */
    [[nodiscard]] size_t size() const;

    /**
     * Used to get the total allocated memeory in the array.
     * @return Returns the total number of elements within the array.
     */
    [[nodiscard]] size_t capacity() const;

    /**
     * Used to get the total amount of element in the array.
     * @return Return the total amount of elements within the array.
     */
    [[nodiscard]] size_t max_size() const;

    /**
     * Used to add new element to the array.
     * @param in_object Take in reference to T type to be
     * stored in the array.
     */
    void push_back(T& in_object);

    /**
     * Used to remove the last element of the array and reduce the array by one.
     */
    void pop_back();

    /**
     * Used to tell the array to size to a specific size.
     * @param Size The new size of the array.
     * @param override Override the protection that prevent
     * smaller size than the current using size.
     * @return
     */
    bool Resize(size_t Size, bool override = false);

    /**
     * Locks the array size to current size.
     */
    void LockArraySize();

    /**
     * Unlocks the array size to dynamically allocate again.
     */
    void UnlockArraySize();

    /**
     * Return the current status of the lock.
     */
    [[nodiscard]] bool GetLockType() const;

    /**
     * Used to access the array within the Vector and return
     * the elment at index location.
     * @param idx Take in the location that the array will
     * be access at.
     * @return Return the T type of data at location index.
     */
    T& operator[](size_t idx);

    /**
     * Get the start of the array.
     * @return Returns *T of the array, also know as start.
     */
    T* begin();

    /**
     * Gets the end of the array.
     * @return Returns a T* + used_size of the array.
     */
    T* end();

    /**
     * Used for needs of read-only iterator.
     * @return A read-only (constant) iterator that points
     * one of the first element in the VecArray.
     */
    T* cbegin() const noexcept;
    
    /**
     * Used for needs of read-only iterator.
     * @return A read-only (constant) iterator that points
     * one of the end element in the VecArray.
     */
    T* cend() const noexcept;

    /**
     * Used for taking in rvalue instead of lvalue.
     * @param item Take the element that will be inserted in a
     * location idx.
     * @param idx Take the index location where the item will be
     * inserted.
     * @throw std::out_of_range If idx is invalid index.
     */
    void insert(T&& item, size_t idx);

    /**
     * Used for taking in lvalue instead of rvalue.
     * @param item Take in a reference to T item.
     * @param idx Take the index location where the item will be
     * inserted.
     * @throw std::out_of_range If idx is invalid index.
     */
    void insert(T& item, size_t idx);
    
    /**
     * 
     * @param loc The index of the element for which data should be
     * accessed.
     * @return Read/write reference to data.
     * @throw std::out_of_range If idx is invalid index.
     */
    T* at(size_t loc);

    /**
     * Used to check if element is zero.
     * @return true if the VecArray is empty (Begin() == End().)
     */
    bool empty();

    /**
     * Erases all the element. This does not delete the pointers.
     * This is user's responsibility.
     */
    void clear();

    /**
     * Reduces the capacity of the VecArray to the usage size.
     */
    void strink_to_fit();

protected:
    /**
     * Resizes the dynamic array by std::ceil(1.5 * total size).
     */
    void Reallocate();

    /**
     * Used to check whether or not something needs to be
     * reallocated.
     * @param new_size Take in the value that will be checked
     * against total_size.
     */
    void CheckAllocation(size_t new_size);

    /**
     * Used to populate the new array.
     * @param OldArray Takes in old array to be used to populate
     * the new array.
     * @param NewArray Take in the new array to be populated.
     * @param old_size Take in the old size of the array for the
     * for loop O(n).
     */
    static void NewArray(T* OldArray, T* NewArray, size_t old_size);

    /**
     * Used to check if a value is out of range.
     * @param value Take in a value that is check if it is
     * out range based on used_size.
     * @return Return whether or not the range is out of
     * range (true) or not (false.
     */
    void Out_Of_Range(size_t value) const;

    /**
     * Used to insert an item at any location within the array.
     * @param item Take the element that will be inserted in a location idx.
     * @param idx Take the index location where the item will be inserted.
     */
    void InsertImpl(const T& item, size_t idx);
};

template<typename T>
size_t VecArray<T>::size() const {
    return _size;
}

template<typename T>
size_t VecArray<T>::capacity() const {
    return _capacity;
}

template<typename T>
size_t VecArray<T>::max_size() const {
    return ULLONG_MAX/sizeof(T) - 1;
}

template<typename T>
void VecArray<T>::push_back(T& in_object) {
    if(LockArray) {
        return;
    }
    CheckAllocation(_size + 1);

    object[_size++] = in_object;
}

template<typename T>
void VecArray<T>::pop_back() {
    if(object[_size - 1] == NULL || _size == 0) {
        return;
    }

    if(std::is_array_v<T>) {
        delete[] object[_size - 1];
    } else {
        delete object[_size - 1];
    }

    T* Holder = object;

    --_capacity;
    --_size;

    object = new T[_size];
    for(int i = 0; i < _size - 1; i++) {
        object[i] == Holder[i];
    }
}

template<typename T>
bool VecArray<T>::Resize(const size_t Size, const bool override) {
    if(_size > Size && !override || LockArray) {
        return false;
    }

    T* Holder = object;
    object = new T[Size];
    VecArray::NewArray(Holder, object, Size);
    _capacity = Size;

    return true;
}

template<typename T>
void VecArray<T>::LockArraySize() {
    LockArray = true;
}

template<typename T>
void VecArray<T>::UnlockArraySize() {
    LockArray = false;
}

template<typename T>
bool VecArray<T>::GetLockType() const {
    return LockArray;
}

template<typename T>
T& VecArray<T>::operator[](size_t idx) {
    return object[idx];
}

template<typename T>
T* VecArray<T>::begin() {
    return object;
}

template<typename T>
T* VecArray<T>::end() {
    return object + _size;
}

template<typename T>
T * VecArray<T>::cbegin() const noexcept {
    return object;
}

template<typename T>
T * VecArray<T>::cend() const noexcept {
    return object + _size;
}

template<typename T>
void VecArray<T>::insert(T&& item, const size_t idx) {
    InsertImpl(std::forward<T>(item), idx);
}

template<typename T>
void VecArray<T>::insert(T &item, const size_t idx) {
    InsertImpl(item, idx);
}

template<typename T>
T * VecArray<T>::at(size_t loc) {
    Out_Of_Range(loc);
    
    return object[loc];
}

template<typename T>
bool VecArray<T>::empty() {
    return begin() == end();
}

template<typename T>
void VecArray<T>::clear() {
    std::destroy(begin(), end());
}

template<typename T>
void VecArray<T>::strink_to_fit() {
    T* Holder = object;
    object = new T[_size];

    for(int i = 0; i < _size; i++) {
        object[i] = Holder[i];
    }
}

template<typename T>
void VecArray<T>::Reallocate() {
    T* Holder = object;
    const size_t temp_size = _capacity;

    _capacity = std::ceil(_capacity * 1.5f);
    object = new T[_capacity];

    // Copy Old to New.
    VecArray::NewArray(Holder, object, temp_size);
    delete[] Holder;
}

template<typename T>
void VecArray<T>::CheckAllocation(const size_t new_size) {
    if(new_size > _capacity && !LockArray) {
        Reallocate();
    }
}

template<typename T>
void VecArray<T>::NewArray(T* OldArray, T* NewArray, const size_t old_size) {
    for(size_t i  = 0; i < old_size; ++i) {
        NewArray[i] = OldArray[i];
    }
}

template<typename T>
void VecArray<T>::Out_Of_Range(const size_t value) const {
    if(value > _size) {
        throw std::out_of_range("VecArray<T>::operator[] index is out of range of the used size.");
    }
}

template<typename T>
void VecArray<T>::InsertImpl(const T& item, const size_t idx) {
    Out_Of_Range(idx);
    CheckAllocation(_size + 1);

    T* Holder = object;
    object = new T[_capacity];

    for(size_t i = 0, j = 0; i < _capacity; ++i, ++j) {
        if(i == idx) {
            object[i] = item;
            ++j;
        }
        object[j] = Holder[i];
    }
    ++_size;
    delete[] Holder;
}


#endif //VECARRAY_H
