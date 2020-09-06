#pragma once

// ------------------------------------------------------------------------------------------------
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <type_traits>

/* ------------------------------------------------------------------------------------------------
 * Hybrid associative container combining the performance of a vector and usefulness of a map container.
*/
template < class Key, class T, class Pred = std::equal_to< Key > > struct VecMap
{
    // --------------------------------------------------------------------------------------------
    using key_type              = Key;
    using mapped_type           = T;
    using value_type            = std::pair< Key, T >;
    using storage_type          = std::vector< value_type >;
    using key_equal             = Pred;
    using pointer               = typename storage_type::pointer;
    using const_pointer         = typename storage_type::const_pointer;
    using reference             = typename storage_type::reference;
    using const_reference       = typename storage_type::const_reference;
    using size_type             = typename storage_type::size_type;
    using difference_type       = typename storage_type::difference_type;
    using iterator              = typename storage_type::iterator;
    using const_iterator        = typename storage_type::const_iterator;
    using insert_return_type    = std::pair< iterator, pointer >;
    /* --------------------------------------------------------------------------------------------
     * Default constructor.
    */
    VecMap() noexcept(noexcept(storage_type())) = default;
    /* --------------------------------------------------------------------------------------------
     * Copy constructor. Does exactly what you expect it to do.
    */
    VecMap(const VecMap & o)
        : m_Storage(o.m_Storage)
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Move constructor. Does exactly what you expect it to do.
    */
    VecMap(VecMap && o) noexcept
        : m_Storage(std::forward< storage_type >(o.m_Storage))
    {
    }
    /* --------------------------------------------------------------------------------------------
     * Sub-script operator.
    */
    mapped_type & operator [] (const key_type & key)
    {
        for (auto & e : m_Storage)
        {
            if (e.first == key) return e.second;
        }
        return m_Storage.emplace_back(key, mapped_type{}).second;
    }
    /* --------------------------------------------------------------------------------------------
     * Sub-script operator (const).
    */
    const mapped_type & operator [] (const key_type & key) const
    {
        for (const auto & e : m_Storage)
        {
            if (e.first == key) return e.second;
        }
        return m_Storage.emplace_back(key, mapped_type{}).second;
    }
    /* --------------------------------------------------------------------------------------------
     * Retrieve an iterator to the beginning. See: std::vector::begin()
    */
    iterator begin() noexcept { return m_Storage.begin(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve an iterator to the beginning (const). See: std::vector::[c]begin()
    */
    const_iterator begin() const noexcept { return m_Storage.begin(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve an iterator to the beginning (const). See: std::vector::cbegin()
    */
    const_iterator cbegin() const noexcept { return m_Storage.cbegin(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve an iterator to the beginning. See: std::vector::end()
    */
    iterator end() noexcept { return m_Storage.end(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve an iterator to the beginning (const). See: std::vector::[c]end()
    */
    const_iterator end() const noexcept { return m_Storage.end(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve an iterator to the beginning (const). See: std::vector::cend()
    */
    const_iterator cend() const noexcept { return m_Storage.cend(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a reverse iterator to the beginning. See: std::vector::rbegin()
    */
    iterator rbegin() noexcept { return m_Storage.rbegin(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a reverse iterator to the beginning (const). See: std::vector::[c]rbegin()
    */
    const_iterator rbegin() const noexcept { return m_Storage.rbegin(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a reverse iterator to the beginning (const). See: std::vector::crbegin()
    */
    const_iterator crbegin() const noexcept { return m_Storage.crbegin(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a reverse iterator to the beginning. See: std::vector::rend()
    */
    iterator rend() noexcept { return m_Storage.rend(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a reverse iterator to the beginning (const). See: std::vector::[c]rend()
    */
    const_iterator rend() const noexcept { return m_Storage.rend(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve a reverse iterator to the beginning (const). See: std::vector::crend()
    */
    const_iterator crend() const noexcept { return m_Storage.crend(); }
    /* --------------------------------------------------------------------------------------------
     * Check if elements are stored in the container.
    */
    bool empty() const noexcept { return m_Storage.empty(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements stored in the container.
    */
    size_type size() const noexcept { return m_Storage.size(); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements that can be stored in the container.
    */
    size_type max_size() const noexcept { return m_Storage.max_size(); }
    /* --------------------------------------------------------------------------------------------
     * Reserve space for a specific amount of elements.
    */
    void reserve(size_type n) { m_Storage.reserve(n); }
    /* --------------------------------------------------------------------------------------------
     * Retrieve the number of elements that can be held in currently allocated storage.
    */
    size_type capacity() const noexcept { return m_Storage.capacity(); }
    /* --------------------------------------------------------------------------------------------
     * Reduce memory usage by freeing unused memory.
    */
    void conform() { m_Storage.shrink_to_fit(); }
    /* --------------------------------------------------------------------------------------------
     * Discard all stored elements.
    */
    void clear() noexcept { m_Storage.clear(); }
    /* --------------------------------------------------------------------------------------------
     * Locate a an element with a specific key and obtain an iterator to it's location.
    */
    iterator find(const key_type & key) noexcept
    {
        return std::find_if(m_Storage.begin(), m_Storage.end(),
                            [&](reference e) -> bool { return e.first == key; });
    }
    /* --------------------------------------------------------------------------------------------
     * Locate a an element with a specific key and obtain an iterator to it's location.
    */
    const_iterator find(const key_type & key) const noexcept
    {
        return std::find_if(m_Storage.cbegin(), m_Storage.cend(),
                            [&](const_reference e) -> bool { return e.first == key; });
    }
    /* --------------------------------------------------------------------------------------------
     *
    */
    // Check if an element with a specific key exists in the container.
    bool exists(const key_type & key) const noexcept { return find(key) != m_Storage.cend(); }
    /* --------------------------------------------------------------------------------------------
     * Append a new element to the end of the container.
    */
    template< class... Args > mapped_type & emplace_back( Args&&... args )
    {
        return m_Storage.emplace_back(std::forward< Args >(args)...).second;
    }
    /* --------------------------------------------------------------------------------------------
     * Remove the last element of the container.
    */
    void pop_back() { m_Storage.pop_back(); }
    /* --------------------------------------------------------------------------------------------
     * Removes specified element from the container. Returns true if found and removed, false otherwise.
    */
    bool erase(const key_type & key)
    {
        auto itr = find(key);
        if (itr != m_Storage.end())
        {
            m_Storage.erase(itr);
            return true;
        }
        return false;
    }
    /* --------------------------------------------------------------------------------------------
     * Removes specified element from the container. Returns iterator to the next element.
    */
    iterator erase(iterator pos) { return m_Storage.erase(pos); }
    /* --------------------------------------------------------------------------------------------
     * Removes specified element from the container. Returns iterator to the next element.
    */
    iterator erase(const_iterator pos) { return m_Storage.erase(pos); }
private:
    /* --------------------------------------------------------------------------------------------
     * Internal container used to store elements.
    */
    storage_type m_Storage;
};
