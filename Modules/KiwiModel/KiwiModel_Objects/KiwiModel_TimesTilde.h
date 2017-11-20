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

#pragma once

#include <KiwiModel/KiwiModel_Objects/KiwiModel_OperatorTilde.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                    OBJECT *~                                     //
    // ================================================================================ //
    
    class TimesTilde : public OperatorTilde
    {
    public:
        
        static void declare();
        
        static std::unique_ptr<Object> create(std::vector<tool::Atom> const& args);
        
        TimesTilde(flip::Default& d): OperatorTilde(d){};
        
        TimesTilde(std::vector<tool::Atom> const& args);
    };
    
}}
