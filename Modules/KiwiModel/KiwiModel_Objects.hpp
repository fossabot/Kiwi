/*
 ==============================================================================
 
 This file is part of the KIWI library.
 - Copyright (c) 2014-2016, Pierre Guillot & Eliott Paris.
 - Copyright (c) 2016, CICM, ANR MUSICOLL, Eliott Paris, Pierre Guillot, Jean Millot.
 
 Permission is granted to use this software under the terms of the GPL v2
 (or any later version). Details can be found at: www.gnu.org/licenses
 
 KIWI is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 
 ------------------------------------------------------------------------------
 
 Contact : cicm.mshparisnord@gmail.com
 
 ==============================================================================
 */

#ifndef KIWI_MODEL_TYPED_OBJECTS_HPP_INCLUDED
#define KIWI_MODEL_TYPED_OBJECTS_HPP_INCLUDED

#include "KiwiModel_Object.hpp"
#include "KiwiModel_Factory.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                       NEWBOX                                     //
        // ================================================================================ //
        
        class NewBox : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            NewBox(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            NewBox(std::string const& name, std::vector<Atom> const& args);

            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                      ERRORBOX                                    //
        // ================================================================================ //
        
        class ErrorBox : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ErrorBox(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ErrorBox(std::string const& name, std::vector<Atom> const& args);
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setNumberOfInlets(size_t inlets);
            
            //! @brief Set the number of inlets.
            //! @param inlets The number of inlets.
            void setNumberOfOutlets(size_t outlets);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT PLUS                                   //
        // ================================================================================ //
        
        class ObjectPlus : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ObjectPlus(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ObjectPlus(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                    OBJECT PRINT                                  //
        // ================================================================================ //
        
        class ObjectPrint : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ObjectPrint(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ObjectPrint(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
        
        // ================================================================================ //
        //                                  OBJECT RECEIVE                                  //
        // ================================================================================ //
        
        class ObjectReceive : public model::Object
        {
        public:
            
            //! @brief flip Default Constructor
            ObjectReceive(flip::Default& d) : model::Object(d) {}
            
            //! @brief Constructor
            ObjectReceive(std::string const& name, std::vector<Atom> const& args);
            
            //! @internal flip static declare method
            static void declare();
        };
    }
}


#endif // KIWI_MODEL_TYPED_OBJECTS_HPP_INCLUDED