#pragma once

#include <vector>

#include "../core/type_defs.hpp"

namespace ecs {

    /**
     * @brief The EntityManager will create entity ids and keep them contiguous
     * 
     * @tparam TSettings 
     */
    template<typename TSettings>
    class EntityManager {
        using Settings = TSettings;

        using Bitset = typename Settings::Bitset;
    public:
        EntityManager(unsigned int size) : m_last_entity{0}, m_signatures(size) { }
        ~EntityManager() = default;

        EntityID create_entity()
        {
            return m_last_entity++;
        } 

        void destroy_entity(EntityID id)
        {
            m_signatures[id] = m_signatures[m_last_entity-1];
            m_signatures[m_last_entity-1].reset();
            m_last_entity--;
        }

        template<typename T>
        bool has_component(EntityID id)
        {
            return m_signatures[id][Settings::template component_bit<T>()];
        }

        template<typename T>
        void add_component(EntityID id)
        {
            m_signatures[id][Settings::template component_bit<T>()] = 1;
        }

        template<typename T>
        void remove_component(EntityID id)
        {
            m_signatures[id][Settings::template component_bit<T>()] = 0;
        }

        template<typename T>
        void add_tag(EntityID id)
        {
            m_signatures[id][Settings::template tag_bit<T>()] = 1;
        }

        template<typename T>
        void remove_tag(EntityID id)
        {
            m_signatures[id][Settings::template tag_bit<T>()] = 0;
        }

        template<typename T>

        bool has_tag(EntityID id)
        {
            return m_signatures[id][Settings::template tag_bit<T>()];
        }

        const Bitset& get_signature(EntityID id) 
        {
            return m_signatures[id];
        }

        void resize(unsigned size)
        {
            m_signatures.reserve(size);
        }

        EntityID get_last_entity()
        {
            return m_last_entity -1;
        }

        inline void for_each(std::function<void(EntityID)> function)
        {
            for(EntityID entity = 0; entity < m_last_entity; entity++)
            {
                function(entity);
            }
        }

    private:
        EntityID m_last_entity;
        std::vector<Bitset> m_signatures;
    };
}