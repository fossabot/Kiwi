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

#include <KiwiModel/KiwiModel_Objects/KiwiModel_AdcTilde.h>
#include <KiwiModel/KiwiModel_DataModel.h>
#include <KiwiModel/KiwiModel_Factory.h>

namespace kiwi { namespace model {
    
    // ================================================================================ //
    //                                  OBJECT ADC~                                     //
    // ================================================================================ //
    
    void AdcTilde::declare()
    {
        std::unique_ptr<ObjectClass> adctilde_class(new ObjectClass("adc~", &AdcTilde::create));
        
        flip::Class<AdcTilde> & adctilde_model = DataModel::declare<AdcTilde>()
                                                 .name(adctilde_class->getModelName().c_str())
                                                 .inherit<Object>();
        
        Factory::add<AdcTilde>(std::move(adctilde_class), adctilde_model);
    }
    
    std::unique_ptr<Object> AdcTilde::create(std::vector<tool::Atom> const& args)
    {
        return std::make_unique<AdcTilde>(args);
    }
    
    AdcTilde::AdcTilde(std::vector<tool::Atom> const& args)
    {
        size_t channels = parseArgsAsChannelRoutes(args).size();
        
        pushInlet({PinType::IType::Control});
        
        for (int i = 0; i < channels; ++i)
        {
            pushOutlet({PinType::IType::Signal});
        }
    }
    
    std::vector<size_t> AdcTilde::parseArgsAsChannelRoutes(std::vector<tool::Atom> const& args) const
    {
        std::vector<size_t> routes;
        
        for(tool::Atom const& arg : args)
        {
            if (arg.isNumber())
            {
                if (arg.getInt() <= 0)
                {
                    throw Error("audio interface null or negative channel");
                }
                
                routes.push_back(arg.getInt() - 1);
            }
            else if(arg.isString())
            {
                std::string inputs(arg.getString());
                
                size_t sep_pos = inputs.find(":");
                
                if (sep_pos == std::string::npos)
                {
                    throw Error("audio interface wrong arguments");
                }
                
                int left_input = std::stoi(inputs.substr(0, sep_pos)) - 1;
                int right_input = std::stoi(inputs.substr(inputs.find(":") + 1)) - 1;
                
                if (left_input < 0 || right_input < 0)
                {
                    throw Error("audio interface null or negative channel");
                }
                
                const bool rev = left_input > right_input;
                
                for (int channel = left_input;
                     rev ? channel >= right_input : channel <= right_input;
                     rev ? --channel : ++channel)
                {
                    routes.push_back(channel);
                }
            }
        }
        
        if (routes.empty())
        {
            routes = {0, 1};
        }
        
        return routes;
    }
    
    std::string AdcTilde::getIODescription(bool is_inlet, size_t index) const
    {
        if(is_inlet)
        {
            return "Start/Stop dsp";
        }
        else
        {
            if(index < getNumberOfOutlets())
            {
                auto text_atoms = tool::AtomHelper::parse(getText());
                text_atoms.erase(text_atoms.cbegin());
                const auto routes = parseArgsAsChannelRoutes(text_atoms);
                
                return "(signal) Audio In Channel " + std::to_string(routes[index] + 1);
            }
        }
        
        return {};
    }
    
}}
