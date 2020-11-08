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

#include <tuple>
#include <type_traits>
#include <map>

namespace shmem {

    // Utils
    struct BlockCount {
        BlockCount(size_t val) : val_(val)
        {
        }
        size_t val_;
    };

    struct BlockSize {
        BlockSize(size_t val) : val_(val)
        {
        }
        size_t val_;
    };

    template <typename Key, typename Value>
    class SharedMap {
    public:
        using SharedString = std::basic_string<char, std::char_traits<char>, ShAlloc<char>>;

        using Key_ = std::conditional_t<std::is_pod_v<Key>, Key, SharedString>;
        using Value_ = std::conditional_t<std::is_pod_v<Value>, Value, SharedString>;

        using Node  = std::pair<Key_, Value_>;
        using ShMap = std::map<Key_, Value_, std::less<Key_>, ShAlloc<Node>>;

        SharedMap(BlockCount block_count, BlockSize block_size);
        ~SharedMap();

        void update(const Key& key, const Value& value);
        void insert(const Key& key, const Value& value);
        void remove(const Key& key);
        Value get(const Key& key) const;

    private:
        Key_ get_key_(const Key& key) const;
        Value_ get_value_(const Value& value) const;

        MemoryMap memory_;
        ShMap* map_;
        Semaphore* sem_;
    };

    template <typename Key, typename Value>
    SharedMap<Key, Value>::SharedMap(BlockCount block_count, BlockSize block_size) :
    memory_(sizeof(Semaphore) + sizeof(ShMemState) +
            sizeof(ShMap) + block_count.val_ + (block_count.val_ * block_size.val_)) {
        // Semaphore
        sem_ = new (memory_.get()) Semaphore;

        // ShMemState
        auto* state = new (static_cast<char*>(memory_.get()) + sizeof(Semaphore)) ShMemState;
        state->blocks_count = block_count.val_;
        state->block_size   = block_size.val_;

        // Block_table
        auto* used_blocks_table = static_cast<char*>(memory_.get()) +
                                  sizeof(Semaphore) + sizeof(ShMemState);
        ::memset(used_blocks_table, FREE_BLOCK, block_count.val_);

        state->used_blocks_table = used_blocks_table;
        ShAlloc<ShMap> allocator(state);

        map_ = new (used_blocks_table + block_count.val_) ShMap(allocator);
        state->first_block = used_blocks_table + block_count.val_ + sizeof(ShMap);
    }

    template <typename Key, typename Value>
    SharedMap<Key, Value>::~SharedMap() {
        map_->~ShMap();
        sem_->~Semaphore();
    }

    template <typename Key, typename Value>
    void SharedMap<Key, Value>::update(const Key& key, const Value& value) {
        SemLock lock(*sem_);
        try {
            map_->at(get_key_(key)) = get_value_(value);
        } catch (const std::out_of_range&) {
            throw out_of_range("update_out_of_range");
        }
    }

    template <typename Key, typename Value>
    void SharedMap<Key, Value>::insert(const Key& key, const Value& value) {
        SemLock lock(*sem_);
        map_->insert( {get_key_(key), get_value_(value)} );
    }

    template <typename Key, typename Value>
    void SharedMap<Key, Value>::remove(const Key& key) {
        if (map_->count(get_key_(key)) > 0) {
            map_->erase(get_key_(key));
        }
    }

    template <typename Key, typename Value>
    Value SharedMap<Key, Value>::get(const Key& key) const {
        try {
            auto ret_value = Value(map_->at(get_key_(key)));
            return ret_value;
        } catch (const std::out_of_range&) {
            throw out_of_range("get_out_of_range");
        }
    }

    template <typename Key, typename Value>
    typename SharedMap<Key, Value>::Key_ SharedMap<Key, Value>::get_key_(const Key& key) const {
        if constexpr (std::is_pod_v<Key>) {
            return key;
        } else {
            return Key_(key, map_->get_allocator());
        }
    }

    template <typename Key, typename Value>
    typename SharedMap<Key, Value>::Value_ SharedMap<Key, Value>::get_value_(const Value &value) const {
        if constexpr (std::is_pod_v<Value>) {
            return value;
        } else {
            return Value_(value, map_->get_allocator());
        }
    }

}

#endif //SHARED_MAP_H
