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

#include "KiwiFactory.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      FACTORY                                     //
        // ================================================================================ //
        
        std::unique_ptr<Object> Factory::create(std::string const& name, std::string const& text)
        {
            std::lock_guard<std::mutex> guard(getMutex());
                        
            const auto& creators = getCreators();
            const auto it = creators.find(name);
            if(it != creators.end())
            {
                return std::unique_ptr<Object>(it->second(text));
            }
            
            return nullptr;
        }
                
        bool Factory::has(std::string const& name)
        {
            std::lock_guard<std::mutex> guard(getMutex());
            const auto& creators = getCreators();
            return (creators.find(name) != creators.end());
        }
        
        void Factory::remove(std::string const& name)
        {
            std::lock_guard<std::mutex> guard(getMutex());
            getCreators().erase(name);
        }
        
        std::vector<std::string> Factory::getNames()
        {
            std::lock_guard<std::mutex> guard(getMutex());
            
            std::vector<std::string> names;
            for(const auto& creator : getCreators())
            {
                names.emplace_back(creator.first);
            }
            
            return names;
        }
    }
}
