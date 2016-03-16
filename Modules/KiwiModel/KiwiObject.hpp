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

#ifndef KIWI_MODEL_OBJECT_HPP_INCLUDED
#define KIWI_MODEL_OBJECT_HPP_INCLUDED

#include "KiwiAttribute.hpp"

namespace kiwi
{
    namespace model
    {
        class Patcher;
        
        // ================================================================================ //
        //                                      OBJECT                                      //
        // ================================================================================ //
        
        //! @brief The Object is an abstract base class for kiwi objects.
        //! @details objects can be instantiated in a Patcher.
        class Object :  public flip::Object,
                        public Attribute::Manager
        {
        public:
            friend class Patcher;
            
            class Classic;
            friend class Classic;
            
            //! @brief Enum of Object types
            //! @remark Can't use Type because of flip::Type conflict
            enum class ObjectType : uint8_t
            {
                Invalid = 0,
                Default,
                Classic,
                Gui,
                Dsp,
                DspGUi
            };
            
            //! @brief Default constructor.
            Object()
            {
                std::cout << "Object Default ctor called\n";
            }
            
            //! @brief Constructor.
            Object(std::string const& name, std::string const& text);
            
            //! @brief Copy constructor.
            //! @remark Needed to be inserted in a flip::Array (Verify)
            Object(const model::Object& rhs) noexcept;
            
            //! @brief Destructor.
            virtual ~Object() noexcept;
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<model::Object>()) return;
                
                TModel::template declare<model::Object>()
                .template name("cicm.kiwi.Object")
                .template member<flip::String, &Object::m_name>("name")
                .template member<flip::String, &Object::m_text>("text")
                .template member<flip::Int, &Object::m_id>("id");
            }
            
            //! @brief Get the type of the Object.
            //! @return The type of the Object.
            virtual inline ObjectType getType() const noexcept { return ObjectType::Invalid; };
            
            //! @brief Returns the patcher that manages the object.
            //! @return The Patcher's pointer.
            inline Patcher* getPatcher() { return parent().ptr<Patcher>(); }
            
            //! @brief Returns the name of the Object.
            //! @return The name of the Object.
            inline std::string getName() const noexcept { return m_name; }
            
            //! @brief Returns the text of the Object.
            //! @return The text of the Object.
            inline std::string getText() const noexcept { return m_text; }
            
            //! @brief Returns the id of the Object.
            //! @return The id of the Object.
            inline int64_t getId() const noexcept       { return m_id; }
            
        private:
            flip::String    m_name;
            flip::String    m_text;
            flip::Int       m_id;
        };
        
        // ================================================================================ //
        //                                  OBJECT CLASSIC                                  //
        // ================================================================================ //
        
        class Object::Classic : public model::Object
        {
        public:
            
            Classic()
            {
                std::cout << "Classic Default ctor called\n";
                addAttr(&m_bgcolor, "bgcolor");
                addAttr(&m_color,   "color");
            }
            
            Classic(std::string name, std::string text)
            : model::Object(name, text)
            {
                std::cout << "Classic ctor called\n";
                addAttr(&m_bgcolor, "bgcolor",  {1., 1., 1., 1.});
                addAttr(&m_color,   "color",    {0., 0., 0., 1.});
            }
            
            //! @brief Get the type of the Object.
            //! @return The type of the Object.
            inline ObjectType getType() const noexcept override { return ObjectType::Classic; };
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Object::Classic>()) return;
                
                TModel::template declare<Object::Classic>()
                .template name("cicm.kiwi.Object.Classic")
                .template inherit<model::Object>()
                .template member<decltype(Classic::m_bgcolor),  &Classic::m_bgcolor>("bgcolor")
                .template member<decltype(Classic::m_color),    &Classic::m_color>("color");
            }
            
        private:
            Attribute::RGBA             m_bgcolor;    ///< background color of the box
            Attribute::RGBA             m_color;      ///< text color of the box
        };
    }
}


#endif // KIWI_MODEL_OBJECT_HPP_INCLUDED
