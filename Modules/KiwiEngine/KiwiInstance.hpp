/*
 ==============================================================================
 
 This file is part of the KIWI library.
 Copyright (c) 2014 Pierre Guillot & Eliott Paris.
 
 Permission is granted to use this software under the terms of either:
 a) the GPL v2 (or any later version)
 b) the Affero GPL v3
 
 Details of these licenses can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 To release a closed-source product which uses KIWI, contact : guillotpierre6@gmail.com
 
 ==============================================================================
*/

#ifndef KIWI_ENGINE_INSTANCE_HPP_INCLUDED
#define KIWI_ENGINE_INSTANCE_HPP_INCLUDED

#include "KiwiPatcher.hpp"

namespace kiwi
{
    namespace engine
    {
        // ================================================================================ //
        //                                      INSTANCE                                    //
        // ================================================================================ //
        
        class Instance : public flip::DocumentObserver<model::Patcher>
        {
        public:
            
            //! @brief Constructor.
            //! @param user_id The user ID.
            //! @param name The instance name.
            Instance(uint64_t user_id, std::string const& name) noexcept;
            
            //! @brief Destructor.
            ~Instance();
            
            //! @brief Get the name of the Instance.
            inline std::string getName() const                  { return m_name; }
            
            //! @brief Get the user ID of the Instance.
            inline uint64_t getUserId() const noexcept          { return m_user_id; }
            
            //! @internal flip::DocumentObserver<model::Patcher>::document_changed
            void document_changed(model::Patcher& patcher) final;
            
        private:
            
            const uint64_t          m_user_id;
            const std::string       m_name;
        };
    }
}


#endif // KIWI_ENGINE_INSTANCE_HPP_INCLUDED