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

#ifndef KIWI_MODEL_ATTRIBUTE_HPP_INCLUDED
#define KIWI_MODEL_ATTRIBUTE_HPP_INCLUDED

#include "KiwiFlipTypes.hpp"

namespace kiwi
{
    namespace model
    {
        // ================================================================================ //
        //                                      ATTRIBUTE                                   //
        // ================================================================================ //
        
        //! @brief The Attribute class is an abstract base class for specific attributes.
        class Attribute : public flip::Object
        {
        public:
            
            class Manager;
            class Int;
            class Float;
            class String;
            class RGBA;
            class Enum;
            
            
            //! @brief Enum of Attribute types
            //! @remark Can't use Type because of the flip::Type conflict
            enum class AttrType : uint8_t
            {
                Invalid = 0,
                Int,
                Float,
                String,
                RGBA,
                Enum
            };
            
            //! @internal flip Default constructor
            Attribute(flip::Default&) {};
            
            //! @brief Constructs an Attribute object.
            //! @param name The name of the attribute (usually only letters and undescore characters).
            Attribute(std::string const& name) noexcept : m_name(name) {};
            
            //! @brief Move constructor.
            //! @param name The name of the attribute (usually only letters and undescore characters).
            Attribute(std::string&& name) noexcept : m_name(std::forward<std::string>(name)) {};
            
            //! @brief Destructor.
            virtual ~Attribute() noexcept {};
            
            //! @brief Returns the attribute value as a vector of atoms.
            //! @details This method must be overwritten by subclasses.
            //! @return The attribute value as a vector of atoms.
            virtual std::vector<Atom> getValue() const = 0;
            
            //! @brief Returns the attribute's before value as a vector of atoms.
            //! @details This method must be overwritten by subclasses.
            //! @return The attribute's before value as a vector of atoms.
            virtual std::vector<Atom> getBeforeValue() const = 0;
            
            //! @brief Sets the attribute value with a vector of atoms.
            //! @details This method must be overwritten by subclasses.
            //! @param atoms The vector of atoms.
            virtual void setValue(std::vector<Atom> const&) = 0;
            
            //! @brief Returns the attribute default value as a vector of atoms.
            //! @details This method must be overwritten by subclasses.
            //! @return A vector of atoms.
            virtual std::vector<Atom> getDefaultValue() = 0;
            
            //! @brief Resets the value to its default state.
            //! @details This method must be overwritten by subclasses.
            virtual void resetDefault() = 0;
            
            //! @brief Returns the Type of the attribute.
            //! @return The Type of the attribute.
            virtual AttrType getType() const noexcept = 0;
            
            //! @brief Returns the name of the attribute.
            //! @return The name of the attribute.
            inline std::string getName() const { return m_name; }
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Attribute>()) return;
                
                TModel::template declare<Attribute>()
                .name("cicm.kiwi.Attribute")
                .template member<flip::String, &Attribute::m_name>("name");
            }
            
        private:
            template<class ValueType> class Typed;
            flip::String    m_name;
        };
        
        // ================================================================================ //
        //                                  ATTRIBUTE TYPED                                 //
        // ================================================================================ //
        
        //! @brief Base class for typed attributes.
        template<class ValueType>
        class Attribute::Typed : public Attribute
        {
        public:
            using value_t = ValueType;
            
        protected:
            value_t m_default;
            value_t m_value;
            
        public:
            
            //! @internal flip Default constructor
            Typed(flip::Default& d) : Attribute(d) {};
            
            //! @brief Constructs an Attribute.
            Typed(std::string const& name, value_t const& value) :
            Attribute(name), m_default(value), m_value(value) {}
            
            //! @brief Destructor.
            virtual ~Typed() noexcept {}
            
            //! @internal flip static declare method
            template<class TModel, class TSubclass>
            static void declare(std::string const& subclass_name)
            {
                if(! TModel::template has<Typed<value_t>>())
                {
                    const std::string typed_name_str = "cicm.kiwi.AttrTyped." + subclass_name;
                    TModel::template declare<Typed<value_t>>()
                    .name(typed_name_str.c_str())
                    .template inherit<Attribute>()
                    .template member<value_t, &Typed::m_default>("default_value")
                    .template member<value_t, &Typed::m_value>("value");
                }
                
                if(! TModel::template has<TSubclass>())
                {
                    const std::string sublass_name_str = "cicm.kiwi." + subclass_name;
                    TModel::template declare<TSubclass>()
                    .template name(sublass_name_str.c_str())
                    .template inherit<Attribute::Typed<value_t>>();
                }
            }
            
            //! @brief Returns the attribute's before value as a vector of atoms.
            //! @details This method must be overwritten by subclasses.
            //! @return The attribute's before value as a vector of atoms.
            std::vector<Atom> getBeforeValue() const override {return {m_value.before()}; };
            
            //! @brief Get the Attribute value.
            //! @return The current value as a value_t.
            virtual inline value_t get() const              { return m_value; }
            
            //! @brief Set the Attribute value.
            //! @param value The new value as a value_t.
            virtual void set(value_t const& value)          { m_value = value; }
            
            //! @brief Get the Attribute default value.
            //! @return The default value as a value_t.
            virtual inline value_t getDefault() const       { return m_default; }
            
            //! @brief Resets the value to its default state.
            void resetDefault() override                    { set(m_default); }
        };
        
        // ================================================================================ //
        //                                   ATTRIBUTE INT                                  //
        // ================================================================================ //
        
        //! @brief An Attribute that hold an integer value.
        class Attribute::Int final : public Attribute::Typed<flip::Int>
        {
        public:
            
            //! @internal flip Default constructor
            Int(flip::Default& d) : Attribute::Typed<value_t>(d) {};
            
            Int(std::string const& name, value_t const& value) :
            Attribute::Typed<value_t>(name, value) {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                Attribute::Typed<value_t>::declare<TModel, Int>("Attribute.Int");
            }
            
            //! @brief Retrieve the Type of the Attribute.
            //! @return The Type of the Attribute (Type::Int).
            inline AttrType getType() const noexcept final       { return AttrType::Int; }
            
            //! @brief Get the Attribute value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getValue() const noexcept final    { return {m_value.value()}; }
            
            //! @brief Get the Attribute default value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getDefaultValue() final            { return {m_default.value()}; };
            
            //! @brief Set the Attribute value with a vector of atoms.
            //! @param atoms A vector of atoms.
            void setValue(std::vector<Atom> const& atoms) final
            {
                if(atoms.size() >= 1 && atoms[0].isNumber())
                {
                    m_value = atoms[0].getInt();
                }
            }
        };
        
        // ================================================================================ //
        //                                  ATTRIBUTE FLOAT                                 //
        // ================================================================================ //
        
        //! @brief An Attribute that hold a floating-point value.
        class Attribute::Float final : public Attribute::Typed<flip::Float>
        {
        public:
            
            //! @internal flip Default constructor
            Float(flip::Default& d) : Attribute::Typed<value_t>(d) {};
            
            Float(std::string const& name, value_t const& value) :
            Attribute::Typed<value_t>(name, value) {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                Attribute::Typed<value_t>::declare<TModel, Attribute::Float>("Attribute.Float");
            }
            
            //! @brief Retrieve the Type of the Attribute.
            //! @return The Type of the Attribute (Type::Float).
            inline AttrType getType() const noexcept final      { return AttrType::Float; };
            
            //! @brief Get the Attribute value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getValue() const final            { return {m_value.value()};}
            
            //! @brief Get the Attribute default value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getDefaultValue() final           { return {m_default.value()}; };
            
            //! @brief Set the Attribute value with a vector of atoms.
            //! @param atoms A vector of atoms.
            void setValue(std::vector<Atom> const& atom) final
            {
                if(atom.size() >= 1 && atom[0].isNumber())
                {
                    m_value = atom[0].getFloat();
                }
            }
        };
        
        // ================================================================================ //
        //                                   ATTRIBUTE STRING                               //
        // ================================================================================ //
        
        //! @brief An Attribute that hold a string value.
        class Attribute::String final : public Attribute::Typed<flip::String>
        {
        public:
            
            //! @internal flip Default constructor
            String(flip::Default& d) : Attribute::Typed<value_t>(d) {};
            
            String(std::string const& name, value_t const& value) :
            Attribute::Typed<value_t>(name, value) {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                Attribute::Typed<value_t>::declare<TModel, Attribute::String>("Attribute.String");
            }
            
            //! @brief Retrieve the Type of the Attribute.
            //! @return The Type of the Attribute (Type::Int).
            inline AttrType getType() const noexcept final       { return AttrType::String; }
            
            //! @brief Get the Attribute value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getValue() const noexcept final    { return {m_value.value()}; }
            
            //! @brief Get the Attribute default value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getDefaultValue() final            { return {m_default.value()}; };
            
            //! @brief Set the Attribute value with a vector of atoms.
            //! @param atoms A vector of atoms.
            void setValue(std::vector<Atom> const& atoms) final
            {
                if(atoms.size() >= 1 && atoms[0].isString())
                {
                    m_value = atoms[0].getString();
                }
            }
        };
        
        // ================================================================================ //
        //                                  ATTRIBUTE RGBA                                  //
        // ================================================================================ //
        
        //! @brief An Attribute that hold a RGBA (red, green, blue, alpha) color value.
        class Attribute::RGBA final : public Attribute::Typed<FlipRGBA>
        {
        public:
            
            //! @internal flip Default constructor
            RGBA(flip::Default& d) : Attribute::Typed<value_t>(d) {};
            
            RGBA(std::string const& name, value_t const& value) :
            Attribute::Typed<value_t>(name, value) {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                Attribute::Typed<value_t>::declare<TModel, Attribute::RGBA>("Attribute.RGBA");
            }
            
            //! @brief Retrieve the Type of the Attribute.
            //! @return The Type of the Attribute (Type::RGBA).
            inline AttrType getType() const noexcept final               { return AttrType::RGBA; }
            
            //! @brief Get the Attribute value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getValue() const noexcept final            { return m_value; }
            
            //! @brief Get the Attribute default value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getDefaultValue() final                    { return m_default; }
            
            //! @brief Set the Attribute value with a vector of atoms.
            //! @param atoms A vector of atoms.
            void setValue(std::vector<Atom> const& atom) final           { m_value = atom; }
        };
        
        // ================================================================================ //
        //                                  ATTRIBUTE ENUM                                  //
        // ================================================================================ //
        
        //! @brief An Attribute that hold an enum value.
        class Attribute::Enum final : public Attribute::Typed<flip::Int>
        {
        public:
            
            //! @internal flip Default constructor
            Enum(flip::Default& d) : Attribute::Typed<value_t>(d) {};
            
            Enum(std::string const& name, value_t const& value) :
            Attribute::Typed<value_t>(name, value) {}
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                Attribute::Typed<value_t>::declare<TModel, Attribute::Enum>("Attribute.Enum");
            }
            
            //! @brief Retrieve the Type of the Attribute.
            //! @return The Type of the Attribute (Type::RGBA).
            inline AttrType getType() const noexcept final       { return AttrType::Enum; }
            
            //! @brief Get the Attribute value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getValue() const noexcept final    { return {m_value.value()}; }
            
            //! @brief Get the Attribute default value as a vector of atoms.
            //! @return A vector of atoms.
            std::vector<Atom> getDefaultValue() final            { return {m_default.value()}; };
            
            //! @brief Set the Attribute value with a vector of atoms.
            //! @param atoms A vector of atoms.
            void setValue(std::vector<Atom> const& atoms) final
            {
                if(atoms.size() >= 1 && atoms[0].isNumber())
                {
                    m_value = atoms[0].getInt();
                }
            }
        };
        
        // ================================================================================ //
        //                                  ATTRIBUTE MANAGER                               //
        // ================================================================================ //
        
        //! @brief The attribute manager manages a set of attributes.
        //! @details it allows to retrieve or set the attribute value by name.
        class Attribute::Manager : public flip::Object
        {
        public:
            
            //! @brief Default constructor.
            Manager() = default;
            
            //! @brief Destructor.
            virtual inline ~Manager() noexcept
            {
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                m_attributes.clear();
            }
            
            //! @internal flip static declare method
            template<class TModel>
            static void declare()
            {
                if(TModel::template has<Attribute::Manager>()) return;
                
                TModel::template declare<Attribute::Manager>()
                .name("cicm.kiwi.Attribute.Manager")
                .template member<decltype(Attribute::Manager::m_attributes),
                                 &Attribute::Manager::m_attributes>("attributes");
            }
            
            //! @brief Returns true if an attribute value changed.
            inline bool attributeChanged() const noexcept { return m_attributes.changed(); }
            
            //! @brief Get the value of a given attribute by name.
            //! @param name the name of the attribute.
            //! @return The value of the attribute as a vector of atoms,
            //! or an empty vector if the attribute name doesn't exist.
            inline std::vector<Atom> getAttributeValue(std::string const& name) const noexcept
            {
                const Attribute* attr = getAttribute(name);
                if(attr)
                {
                    return attr->getValue();
                }
                
                return std::vector<Atom>();
            }
            
            //! @brief Set the value of a given attribute by name.
            //! @param name the name of the attribute.
            //! @param atoms A vector of atoms.
            void setAttributeValue(std::string const& name, std::vector<Atom> const& atoms) noexcept
            {
                Attribute* attr = getAttribute(name);
                if(attr != nullptr)
                {
                    attr->setValue(atoms);
                }
            }
            
            //! @brief Returns an attribute by name.
            //! @param name the name of the attribute.
            //! @return The attribute pointer if found, otherwise nullptr.
            Attribute* getAttribute(std::string const& name) const noexcept
            {
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                
                if(!name.empty())
                {
                    const auto predicate = [&name](Attribute const& attr)
                    {
                        return (attr.getName() == name);
                    };
                    
                    const auto it = find_if(m_attributes.begin(), m_attributes.end(), predicate);
                    if(it != m_attributes.cend())
                    {
                        return it.operator->();
                    }
                }
                
                return nullptr;
            }
            
            //! @brief Returns all the attributes.
            std::vector<Attribute*> getAttributes() const noexcept
            {
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                
                std::vector<Attribute*> attrs;
                
                for(auto& attr : m_attributes)
                {
                    attrs.push_back( &attr );
                }
                
                return attrs;
            }
            
        protected:
            
            //! @brief Add an attribute to the be managed.
            //! @param name The name of the attribute.
            //! @param value The value of the attribute.
            template<class AttrType, class... Args>
            void addAttr(std::string const& name, Args&&... args)
            {
                static_assert(std::is_base_of<Attribute, AttrType>::value, "The class must inherit from Attribute.");
                static_assert(std::is_constructible<AttrType, std::string, Args...>::value, "Bad Attribute arguments");
                
                std::lock_guard<std::mutex> guard(m_attrs_mutex);
                
                if(!name.empty())
                {
                    auto it = find_if(m_attributes.begin(), m_attributes.end(), [&name](Attribute const& attr)
                    {
                        return (attr.getName() == name);
                    });
                    
                    if(it == m_attributes.end())
                    {
                        m_attributes.emplace<AttrType>(name, std::forward<Args>(args)...);
                    }
                }
            }
            
        private:
            flip::Collection<Attribute>         m_attributes;
            mutable std::mutex                  m_attrs_mutex;
        };
    }
}

#endif // KIWI_MODEL_ATTRIBUTE_HPP_INCLUDED