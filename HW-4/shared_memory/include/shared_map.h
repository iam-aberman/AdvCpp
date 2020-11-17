//
// Created by Osip Chin on 07.11.2020.
//
#ifndef SHARED_MAP_H
#define SHARED_MAP_H

#include "shared_memory_map.h"
#include "shmem_exception.h"
#include "shared_semaphore.h"
#include "shmem_sem_lock.h"
#include "shmem_alloc.h"

#include <type_traits>
#include <tuple>
#include <map>

namespace shmem {

    // Utils
    struct BlockCount {
        explicit BlockCount(size_t val) : val_(val)
        {
        }
        size_t val_;
    };

    struct BlockSize {
        explicit BlockSize(size_t val) : val_(val)
        {
        }
        size_t val_;
    };

    template <typename Key, typename Value>
    class SharedMap {
    public:
        using SharedString = std::basic_string<char, std::char_traits<char>, ShAlloc<char>>;

        template <typename T>
        using T_ = std::conditional_t<std::is_same_v<T, std::string>, SharedString, T>;

        using Key_ = T_<Key>;
        using Value_ = T_<Value>;

        using Node  = std::pair<const Key_, Value_>;
        using ShMap = std::map<Key_, Value_, std::less<Key_>, ShAlloc<Node>>;

        SharedMap(BlockCount block_count, BlockSize block_size);
        ~SharedMap() = default;

        void delete_map();
        void update(const Key& key, const Value& value);
        void insert(const Key& key, const Value& value);
        void remove(const Key& key);
        Value get(const Key& key) const;

    private:
        template <typename T>
        T_<T> get_(const T& t) const;

        MemoryMap* memory_;
        ShMap* map_;
        Semaphore* sem_;
        ShMemState* state_;
    };

    template <typename Key, typename Value>
    SharedMap<Key, Value>::SharedMap(BlockCount block_count, BlockSize block_size) {
        // Memory map
        memory_ = new MemoryMap(sizeof(Semaphore) + sizeof(ShMemState) +
                                sizeof(ShMap) + block_count.val_ + (block_count.val_ * block_size.val_));

        // Semaphore
        sem_ = new (memory_->get()) Semaphore;

        // ShMemState
        state_ = new (memory_->get() + sizeof(Semaphore)) ShMemState;
        state_->blocks_count = block_count.val_;
        state_->block_size   = block_size.val_;

        // Block_table
        char* used_blocks_table = memory_->get() +
                                  sizeof(Semaphore) + sizeof(ShMemState);
        ::memset(used_blocks_table, FREE_BLOCK, block_count.val_);

        state_->used_blocks_table = used_blocks_table;
        ShAlloc<ShMap> allocator(state_);

        map_ = new (used_blocks_table + block_count.val_) ShMap(allocator);
        state_->first_block = used_blocks_table + block_count.val_ + sizeof(ShMap);
    }

    template <typename Key, typename Value>
    void SharedMap<Key, Value>::delete_map() {
        map_->~ShMap();
        sem_->~Semaphore();
        memory_->~MemoryMap();
        delete memory_;
    }

    template <typename Key, typename Value>
    void SharedMap<Key, Value>::update(const Key& key, const Value& value) {
        SemLock lock(*sem_);
        try {
            map_->at(get_(key)) = get_(value);
        } catch (const std::out_of_range&) {
            throw OutOfRange("update_out_of_range");
        }
    }

    template <typename Key, typename Value>
    void SharedMap<Key, Value>::insert(const Key& key, const Value& value) {
        SemLock lock(*sem_);
        map_->insert( {get_(key), get_(value)} );
    }

    template <typename Key, typename Value>
    void SharedMap<Key, Value>::remove(const Key& key) {
        SemLock lock(*sem_);
        if (map_->count(get_(key)) > 0) {
            map_->erase(get_(key));
        }
    }

    template <typename Key, typename Value>
    Value SharedMap<Key, Value>::get(const Key& key) const {
        SemLock lock(*sem_);
        try {
            return static_cast<Value>(map_->at(get_(key)));
        } catch (const std::out_of_range&) {
            throw OutOfRange("get_out_of_range");
        }
    }

    template <typename Key, typename Value>
    template <typename T>
    SharedMap<Key, Value>::T_<T> SharedMap<Key, Value>::get_(const T& t) const {
        if constexpr (std::is_same_v<T, T_<T>>) {
            return t;
        } else {
            return T_<T>(t, map_->get_allocator());
        }
    }

} // namespace shmem

#endif //SHARED_MAP_H
