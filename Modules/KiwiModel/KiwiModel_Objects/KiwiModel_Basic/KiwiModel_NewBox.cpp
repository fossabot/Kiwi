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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_Basic/KiwiModel_NewBox.h>

#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        void NewBox::declare()
        {
            Factory::add<NewBox>("newbox").setInternal(true);
        }
        
        NewBox::NewBox() : model::Object("", std::vector<Atom>())
        {
            setFlag(Flag::DefinedSize);
            setWidth(80);
            setHeight(20);
            pushInlet({PinType::IType::Control});
        }
        
        NewBox::NewBox(std::string const& name, std::vector<Atom> const& args)
        : NewBox()
        {
            ;
        }
        
        std::string NewBox::getIODescription(bool is_inlet, size_t index) const
        {
            return "(nothing here)";
        }
    }
}