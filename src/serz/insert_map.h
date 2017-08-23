/**
 * Contains implementation of special map that maintains
 * the order of insertion, which is useful for XML.
 * @author Chen Weiguang
 * @version 0.1.0
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <unordered_map>
#include <utility>
#include <vector>

namespace serz {
    // declaration section

    // forward declaration
    template <class Imap>
    class insert_map_const_iter;

    /**
     * Iterator for insert_map.
     */
    template <class Imap>
    class insert_map_iter {
        template <class Imapx>
        friend class insert_map_const_iter;

    public:
        /**
         * Defines the difference_type type.   
         */
        using difference_type = typename Imap::difference_type;

        /**
         * Defines the value_type type.   
         */
        using value_type = typename Imap::value_type;

        /**
         * Defines the pointer type.   
         */
        using pointer = typename Imap::pointer;

        /**
         * Defines the const_pointer type.   
         */
        using const_pointer = typename Imap::const_pointer;

        /**
         * Defines the reference type.   
         */
        using reference = typename Imap::reference;

        /**
         * Defines the const_reference type.   
         */
        using const_reference = typename Imap::const_reference;

        /**
         * Defines the iterator_category type.   
         */
        using iterator_category = std::input_iterator_tag;

        /**
         * Initializes the iterator with reference to the
         * insert_map value and the ordering value.
         */
        insert_map_iter(
            typename Imap::inner_type &imap,
            typename Imap::orderings_type &orderings,
            const size_t index = 0);

        /**
         * Defaulted copy constructor.
         */
        insert_map_iter(const insert_map_iter &rhs) = default;

        /**
         * Defaulted move constructor.
         */
        insert_map_iter(insert_map_iter &&rhs) = default;

        /**
         * Defaulted copy assignment.
         */
        auto operator=(const insert_map_iter &rhs) -> insert_map_iter & = default;

        /**
         * Defaulted move assignment.
         */
        auto operator=(insert_map_iter &&rhs) -> insert_map_iter & = default;

        /**
         * Checks if the position of current iterator
         * is different from the given iterator.
         */
        auto operator!=(const insert_map_iter &rhs) const -> bool;

        /**
         * Checks if the position of current iterator
         * is different from the given iterator.
         */
        auto operator!=(const insert_map_const_iter<Imap> &rhs) const -> bool;

        /**
         * Dereferences and get the reference to the value
         * at the current position.
         */
        auto operator*() -> reference;

        /**
         * Dereferences and get the const reference to the value
         * at the current position.
         */
        auto operator*() const -> const_reference;

        /**
         * Access the methods directly on the value reference
         * at the current position.
         */
        auto operator->() -> pointer;

        /**
         * Access the non-mutating methods directly on the
         * value reference at the current position.
         */
        auto operator->() const -> const_pointer;

        /**
         * Shifts this iterator to the next position.
         * Pre-increment version.
         */
        auto operator++() -> insert_map_iter &;

        /**
         * Shifts this iterator to the next position.
         * Pre-increment version.
         */
        auto operator++(int) -> insert_map_iter;

    private:
        /**
         * Reference wrapper to the implementing map value.
         */
        std::reference_wrapper<typename Imap::inner_type> imap;

        /**
         * Reference wrapper to the ordering value.
         */
        std::reference_wrapper<typename Imap::orderings_type> orderings;

        /**
         * Position of the iterator on the map.
         */
        size_t index;
    };

    /**
     * Const iterator for insert_map.
     */
    template <class Imap>
    class insert_map_const_iter {
        template <class Imapx>
        friend class insert_map_iter;

    public:
        /**
         * Defines the difference_type type. 
         */
        using difference_type = typename Imap::difference_type;

        /**
         * Defines the value_type type. 
         */
        using value_type = typename Imap::value_type;

        /**
         * Defines the const_pointer type. 
         */
        using const_pointer = typename Imap::const_pointer;

        /**
         * Defines the pointer type. 
         */
        using pointer = const_pointer;

        /**
         * Defines the const_reference type. 
         */
        using const_reference = typename Imap::const_reference;

        /**
         * Defines the reference type. 
         */
        using reference = const_reference;

        /**
         * Defines the iterator_category type. 
         */
        using iterator_category = std::input_iterator_tag;

        /**
         * Initializes the const iterator with reference to the
         * insert_map value and the ordering value.
         */
        insert_map_const_iter(
            const typename Imap::inner_type &imap,
            const typename Imap::orderings_type &orderings,
            const size_t index = 0);

        /**
         * Implicit converting constructor from non-const iterator.
         */
        insert_map_const_iter(const insert_map_iter<Imap> &rhs);

        /**
         * Defaulted copy constructor.
         */
        insert_map_const_iter(const insert_map_const_iter &rhs) = default;

        /**
         * Defaulted move constructor.
         */
        insert_map_const_iter(insert_map_const_iter &&rhs) = default;

        /**
         * Defaulted copy assignment.
         */
        auto operator=(const insert_map_const_iter &rhs) -> insert_map_const_iter & = default;

        /**
         * Defaulted move assignment.
         */
        auto operator=(insert_map_const_iter &&rhs) -> insert_map_const_iter & = default;

        /**
         * Checks if the position of current iterator
         * is different from the given iterator.
         */
        auto operator!=(const insert_map_const_iter &rhs) const -> bool;

        /**
         * Checks if the position of current iterator
         * is different from the given iterator.
         */
        auto operator!=(const insert_map_iter<Imap> &rhs) const -> bool;

        /**
         * Dereferences and get the const reference to the value
         * at the current position.
         */
        auto operator*() const -> const_reference;

        /**
         * Access the non-mutating methods directly on the
         * value reference at the current position.
         */
        auto operator->() const -> const_pointer;

        /**
         * Shifts this iterator to the next position.
         * Pre-increment version.
         */
        auto operator++() -> insert_map_const_iter &;

        /**
         * Shifts this iterator to the next position.
         * Pre-increment version.
         */
        auto operator++(int) -> insert_map_const_iter;

    private:
        /**
         * Const reference wrapper to the implementing map value.
         */
        std::reference_wrapper<const typename Imap::inner_type> imap;

        /**
         * Const reference wrapper to the ordering value.
         */
        std::reference_wrapper<const typename Imap::orderings_type> orderings;

        /**
         * Position of the iterator on the map.
         */
        size_t index;
    };

    /**
     * Implements map while preserving insertion order.
     * Key must be copy constructible. 
     */
    template <class Key, class T>
    class insert_map {
    public:
        /**
         * Alias to the map implementation type.
         */
        using inner_type = std::unordered_map<Key, T>;

        /**
         * Alias to the ordering value type.
         */
        using orderings_type = std::vector<Key>;

        /**
         * Defines key_type type.
         */
        using key_type = typename inner_type::key_type;

        /**
         * Defines mapped_type type.
         */
        using mapped_type = typename inner_type::mapped_type;

        /**
         * Defines value_type type.
         */
        using value_type = typename inner_type::value_type;

        /**
         * Defines size_type type.
         */
        using size_type = typename inner_type::size_type;

        /**
         * Defines difference_type type.
         */
        using difference_type = typename inner_type::difference_type;

        /**
         * Defines hasher type.
         */
        using hasher = typename inner_type::hasher;

        /**
         * Defines key_equal type.
         */
        using key_equal = typename inner_type::key_equal;

        /**
         * Defines allocator_type type.
         */
        using allocator_type = typename inner_type::allocator_type;

        /**
         * Defines reference type.
         */
        using reference = typename inner_type::reference;

        /**
         * Defines const_reference type.
         */
        using const_reference = typename inner_type::const_reference;

        /**
         * Defines pointer type.
         */
        using pointer = typename inner_type::pointer;

        /**
         * Defines const_pointer type.
         */
        using const_pointer = typename inner_type::const_pointer;

        /**
         * Defines iterator type.
         */
        using iterator = insert_map_iter<insert_map<Key, T>>;

        /**
         * Defines const_iterator type.
         */
        using const_iterator = insert_map_const_iter<insert_map<Key, T>>;

        /**
         * Defaulted default constructor.
         */
        insert_map() = default;

        /**
         * Defaulted copy constructor.
         */
        insert_map(const insert_map &rhs) = default;

        /**
         * Defaulted move constructor.
         */
        insert_map(insert_map &&rhs) = default;

        /**
         * Defaulted copy assignment.
         */
        auto operator=(const insert_map &rhs) -> insert_map & = default;

        /**
         * Defaulted move assignment.
         */
        auto operator=(insert_map &&rhs) -> insert_map & = default;

        /**
         * Creates an iterator that points to the first element.
         */
        auto begin() -> iterator;

        /**
         * Creates an const iterator that points to the first element.
         */
        auto begin() const -> const_iterator;

        /**
         * Same as begin() non-mutating version.
         */
        auto cbegin() const -> const_iterator;

        /**
         * Creates an iterator that points to the position
         * past the last element.
         */
        auto end() -> iterator;

        /**
         * Creates a const iterator that points to the position
         * past the last element.
         */
        auto end() const -> const_iterator;

        /**
         * Same as end() non-mutating version.
         */
        auto cend() const -> const_iterator;

        /**
         * Performs map-find process to find the iterator with position
         * of the given key. Returns iterator that points to end if no such key
         * was found. 
         */
        auto find(const Key &key) -> iterator;

        /**
         * Same as above find, except returns const iterator for
         * non-mutating operations. 
         */
        auto find(const Key &key) const -> const_iterator;

        /**
         * Perfect forwards all the given arguments
         * into the emplace of the map implementation type.
         * Typically meant for directly constructing the key and value.
         */
        template <class... Args>
        auto emplace(Args &&... args) -> std::pair<iterator, bool>;

        /**
         * Erases the element at the given const iterator position.
         */
        void erase(const_iterator pos);

        /**
         * Checks if the object is empty.
         */
        auto empty() const -> bool;

        /**
         * Gets the number of paired elements in the object.
         */
        auto size() const -> size_t;

    private:
        /**
         * Holds the actual map implementation.
         */
        inner_type impl;

        /**
         * Holds the struct that stores insertion ordering. 
         */
        orderings_type orderings;
    };

    // implementation section

    template <class Imap>
    insert_map_iter<Imap>::insert_map_iter(typename Imap::inner_type &imap, typename Imap::orderings_type &orderings, const size_t index) :
        imap(imap),
        orderings(orderings),
        index(index) {

    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator!=(const insert_map_iter &rhs) const -> bool {
        return index != rhs.index || &orderings.get() != &rhs.orderings.get() || &imap.get() != &rhs.imap.get();
    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator!=(const insert_map_const_iter<Imap> &rhs) const -> bool {
        return index != rhs.index || &orderings.get() != &rhs.orderings.get() || &imap.get() != &rhs.imap.get();
    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator*() -> reference {
        return *imap.get().find(orderings.get()[index]);
    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator*() const -> const_reference {
        return *imap.get().find(orderings.get()[index]);
    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator->() -> pointer {
        return &**this;
    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator->() const -> const_pointer {
        return &**this;
    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator++() -> insert_map_iter & {
        ++index;
        return *this;
    }

    template <class Imap>
    auto insert_map_iter<Imap>::operator++(int) -> insert_map_iter {
        auto it = *this;
        ++(*this);
        return it;
    }

    template <class Imap>
    insert_map_const_iter<Imap>::insert_map_const_iter(const typename Imap::inner_type &imap, const typename Imap::orderings_type &orderings, const size_t index) :
        imap(imap),
        orderings(orderings),
        index(index) {

    }

    template <class Imap>
    insert_map_const_iter<Imap>::insert_map_const_iter(const insert_map_iter<Imap> &rhs) :
        imap(rhs.imap),
        orderings(rhs.orderings),
        index(rhs.index) {

    }

    template <class Imap>
    auto insert_map_const_iter<Imap>::operator!=(const insert_map_const_iter &rhs) const -> bool {
        return index != rhs.index || &orderings.get() != &rhs.orderings.get() || &imap.get() != &rhs.imap.get();
    }

    template<class Imap>
    auto insert_map_const_iter<Imap>::operator!=(const insert_map_iter<Imap> &rhs) const -> bool {
        return index != rhs.index || &orderings.get() != &rhs.orderings.get() || &imap.get() != &rhs.imap.get();
    }

    template <class Imap>
    auto insert_map_const_iter<Imap>::operator*() const -> const_reference {
        return *imap.get().find(orderings.get()[index]);
    }

    template <class Imap>
    auto insert_map_const_iter<Imap>::operator->() const -> const_pointer {
        return &**this;
    }

    template <class Imap>
    auto insert_map_const_iter<Imap>::operator++() -> insert_map_const_iter & {
        ++index;
        return *this;
    }

    template <class Imap>
    auto insert_map_const_iter<Imap>::operator++(int) -> insert_map_const_iter {
        auto it = *this;
        ++(*this);
        return it;
    }

    template <class Key, class T>
    auto insert_map<Key, T>::begin() -> iterator {
        return iterator(impl, orderings, 0);
    }

    template <class Key, class T>
    auto insert_map<Key, T>::begin() const -> const_iterator {
        return const_iterator(impl, orderings, 0);
    }

    template <class Key, class T>
    auto insert_map<Key, T>::cbegin() const -> const_iterator {
        return begin();
    }

    template <class Key, class T>
    auto insert_map<Key, T>::end() -> iterator {
        return iterator(impl, orderings, orderings.size());
    }

    template <class Key, class T>
    auto insert_map<Key, T>::end() const -> const_iterator {
        return const_iterator(impl, orderings, orderings.size());
    }

    template <class Key, class T>
    auto insert_map<Key, T>::cend() const -> const_iterator {
        return end();
    }

    template <class Key, class T>
    auto insert_map<Key, T>::find(const Key &key) -> iterator {
        const auto end_it = end();

        for (auto it = begin(); it != end_it; ++it) {
            if (it->first == key) {
                return it;
            }
        }

        return end();
    }

    template <class Key, class T>
    auto insert_map<Key, T>::find(const Key &key) const -> const_iterator {
        const auto end_it = end();

        for (auto it = begin(); it != end_it; ++it) {
            if (it->first == key) {
                return it;
            }
        }

        return end();
    }

    template <class Key, class T>
    void insert_map<Key, T>::erase(const_iterator pos) {
        const auto &name = pos->first;
        const auto impl_it = impl.find(name);

        if (impl_it != impl.cend()) {
            const auto orderings_it = std::find_if(orderings.cbegin(), orderings.cend(),
                [&name](const Key &orderingName) {
                    return orderingName == name;
                });

            if (orderings_it != orderings.cend()) {
                orderings.erase(orderings_it);
                impl.erase(impl_it);
            }
        }
    }

    template <class Key, class T>
    template <class... Args>
    auto insert_map<Key, T>::emplace(Args &&... args) -> std::pair<iterator, bool> {
        const auto res = impl.emplace(std::forward<Args>(args)...);

        if (res.second) {
            // insert new entry into ordering
            orderings.push_back(res.first->first);
            return std::make_pair(iterator(impl, orderings, orderings.size() - 1), true);
        } else {
            // use find on the key to get the iterator
            return std::make_pair(find(res.first->first), false);
        }
    }

    template <class Key, class T>
    auto insert_map<Key, T>::empty() const -> bool {
        return impl.empty();
    }

    template <class Key, class T>
    auto insert_map<Key, T>::size() const -> size_t {
        return impl.size();
    }
}
