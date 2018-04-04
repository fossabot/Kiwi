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

#include "KiwiDsp_Signal.h"

namespace kiwi
{
    namespace dsp
    {
        class Chain;
        
        // ==================================================================================== //
        //                                    PERFORM CALL BACK                                 //
        // ==================================================================================== //
        
        //! @brief Pure virtual interface that defines a perform function to be implemented by child classes.
        //! @details The call back is held by the processor and enables calling perform.
        //! Using the IPerformCallBack interface enanles dynamical perform method in an efficient way.
        //! The implementation of the interface depends on the child processor type.
        class IPerformCallBack
        {
        public:
            //! @brief The default contrustor.
            IPerformCallBack() = default;
            
            //! @brief Destructor
            virtual ~IPerformCallBack() = default;
            
            //! @brief pure virtual method that perform input and output buffers.
            virtual void perform(Buffer const& intput, Buffer& output) = 0;
        };
        
        //! @brief Templated implementation of IPerformCallBack. Templated on type of processor.
        //! @details The PerformCallBack acts as std::function but encapsulate a pointer of method to avoid
        //! overhead generated by an std::function indirection.
        template<class TProc>
        class PerformCallBack : public IPerformCallBack
        {
        public:
            
            //! @brief Constructor, get a pointer to the processor and its call back to be called later.
            PerformCallBack(TProc & processor, void (TProc::*call_back)(Buffer const& input, Buffer &output)):
            m_processor(processor),
            m_call_back(call_back)
            {
            }
            
            //! @brief Implementation of perform that binds a processor and its method.
            void perform(Buffer const& input, Buffer& output) override final
            {
                (m_processor.*m_call_back)(input, output);
            }
            
            //! @brief Destructor.
            //! @detilas The callback doesn't hold ownership of either the processor or the processor's method
            //! therefor the no destruction occurs in destructor.
            ~PerformCallBack() = default;
            
        private:
            
            TProc& m_processor;
            
            void (TProc::*m_call_back)(Buffer const& input, Buffer &output);
        };
        
        // ==================================================================================== //
        //                                      PROCESSOR                                       //
        // ==================================================================================== //
        
        //! @brief The pure virtual class that processes digital signal in a Chain object.
        //! @details The class is pure virtual and allows to implement digital signal processing.
        //! You should implement the virtual methods prepare, perform and release.
        //! @see Buffer and Infos
        class Processor
        {
        public: // classes
            
            struct PrepareInfo
            {
                const size_t             sample_rate;
                const size_t             vector_size;
                const std::vector<bool> &inputs;
            };
            
        public: // methods
            
            //! @brief The constructor.
            //! @details Allocates and initializes a default Processor object.
            //! @param ninputs The number of inputs.
            //! @param noutputs The number of outputs.
            Processor(const size_t ninputs, const size_t noutputs) noexcept :
            m_ninputs(ninputs), m_noutputs(noutputs) {}
            
            //! @brief The destructor.
            virtual ~Processor() = default;
            
            //! @brief Gets the current number of inputs.
            //! @return The number of inputs of the Processor object.
            //! @see getNumberOfOutputs()
            inline size_t getNumberOfInputs() const noexcept {return m_ninputs;}
            
            //! @brief Gets the current number of outputs.
            //! @return The number of outputs of the Processor object.
            //! @see getNumberOfInputs()
            inline size_t getNumberOfOutputs() const noexcept {return m_noutputs;}
            
            //! @brief Returns true if the processor shall be performed by the chain.
            //! @details Checks it the perform callback is well set by the processor.
            bool shouldPerform() const noexcept
            {
                return m_call_back != nullptr;
            }
            
        protected: // methods
            
            //! @brief Constructs a callback that will bind a processor and its perform method.
            //! @details setPerformCallBack shall be called by the prepare method to set the callback
            //! that will be called when chain processes a processor.
            //! Different methods can be set as callback depending on PrepareInfo status.
            template<class TProc>
            void setPerformCallBack(TProc* processor,
                                    void (TProc::*call_back)(Buffer const& input, Buffer &output))
            {
                m_call_back.reset(new PerformCallBack<TProc>(*processor, call_back));
            }
            
        private: // methods
            
            //! @brief Prepares everything for the perform method.
            //! @details You should use this method to check the vector size, the sample rate,
            //! the connected inputs and outputs and to allocate memory if needed.
            //! Preparing should also set the callback to be called by the chain.
            //! Not setting the callback will result in the processor not being called by the chain.
            //! @param infos The DSP informations.
            //! @see perform() and release()
            virtual void prepare(PrepareInfo const& infos) = 0;
            
            //! @brief Performs the digital signal processing.
            //! @details Triggers the callback set during prepare phase.
            //! @param input    The input Buffer object.
            //! @param output   The output Buffer object.
            //! @see prepare() and release()
            inline void perform(Buffer const& input, Buffer& output) noexcept
            {
                m_call_back->perform(input, output);
            }
            
            //! @brief Releases everything after the digital signal processing.
            //! @details You can use this method to free the memory allocated during the call of
            //! the prepare method for example.
            //! @see prepare() and perform()
            virtual void release() {};
            
        private: // members
            
            const size_t                        m_ninputs;
            const size_t                        m_noutputs;
            
            std::unique_ptr<IPerformCallBack>   m_call_back;
            
            friend class Chain;
        };
    }
}
