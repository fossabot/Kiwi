/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016-2017, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v3
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#include <functional>

#include <KiwiModel/Kiwi_Objects/Kiwi_Objects.h>
#include <KiwiApp_Patcher/KiwiApp_Objects/KiwiApp_Objects.h>
#include <KiwiApp_Patcher/KiwiApp_Factory.h>

namespace kiwi
{
    std::map<flip::ClassBase const* const, Factory::factory_func> Factory::m_creator_map {};
    
    std::unique_ptr<ObjectView> Factory::createObjectView(model::Object& object_model)
    {
        const auto flip_class = &object_model.get_class();
        const auto it = m_creator_map.find(flip_class);
        
        if(it != m_creator_map.end())
        {
            return it->second(object_model);
        }
        
        return std::make_unique<ClassicView>(object_model);
    }
    
    void Factory::initialise()
    {
        add<model::Bang, BangView>();
        add<model::Toggle, ToggleView>();
    }
}
