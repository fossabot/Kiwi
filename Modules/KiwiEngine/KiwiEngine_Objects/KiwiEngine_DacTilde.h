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

#include <KiwiEngine/KiwiEngine_Objects/KiwiEngine_AudioInterface.h>

namespace kiwi { namespace engine {
    
    // ================================================================================ //
    //                                       DAC~                                       //
    // ================================================================================ //
    
    class DacTilde : public AudioInterfaceObject
    {
    public: // methods
        
        static void declare();
        
        static std::unique_ptr<Object> create(model::Object const& model, Patcher & patcher);
        
        DacTilde(model::Object const& model, Patcher& patcher);
        
        void perform(dsp::Buffer const& input, dsp::Buffer& output) noexcept;
        
        void prepare(dsp::Processor::PrepareInfo const& infos) override final;
    };
}}
