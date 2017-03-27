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

#ifndef KIWI_APP_DOCUMENT_BROWSER_HPP_INCLUDED
#define KIWI_APP_DOCUMENT_BROWSER_HPP_INCLUDED

#include <KiwiEngine/KiwiEngine_Listeners.hpp>

#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>

#include "KiwiApp_Api.hpp"

#include <list>

namespace kiwi
{
    // ================================================================================ //
    //                                 DOCUMENT BROWSER                                 //
    // ================================================================================ //
    
    //! @brief Request Patcher document informations through a Kiwi API.
    class DocumentBrowser : public juce::Timer, public juce::ValueTree::Listener
    {
    public: // nested classes
        
        struct Listener;
        class Drive;
        
    public: // methods
        
        //! @brief Constructor
        DocumentBrowser();
        
        //! @brief Destructor
        ~DocumentBrowser();
        
        //! @brief start processing
        void start(const int interval = 5000);
        
        //! @brief stop processing
        void stop();
        
        //! @brief Scan the LAN to find a service provider.
        void process();
        
        //! @brief juce::Timer callback.
        void timerCallback() override;
        
        //! @brief Returns a list of drives.
        std::vector<Drive*> getDrives() const;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
    private: // methods
        
        void valueTreePropertyChanged(juce::ValueTree&, const juce::Identifier&) override;
        void valueTreeChildAdded(juce::ValueTree&, juce::ValueTree&) override               { }
        void valueTreeChildRemoved(juce::ValueTree&, juce::ValueTree&, int) override        { }
        void valueTreeChildOrderChanged(juce::ValueTree&, int, int) override                { }
        void valueTreeParentChanged(juce::ValueTree&) override                              { }
        
    private: // variables
        
        std::unique_ptr<Drive>                          m_distant_drive;
        engine::Listeners<Listener>                     m_listeners = {};
    };
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER LISTENER                           //
    // ================================================================================ //
    
    //! @brief Listen to document explorer changes.
    struct DocumentBrowser::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when the document list changed.
        virtual void driveAdded(DocumentBrowser::Drive& drive) = 0;
        
        //! @brief Called when the document list changed.
        virtual void driveChanged(DocumentBrowser::Drive const& drive) = 0;
        
        //! @brief Called when the document list changed.
        virtual void driveRemoved(DocumentBrowser::Drive const& drive) = 0;
    };
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER DRIVE                              //
    // ================================================================================ //
    
    class DocumentBrowser::Drive
    {
    public: // nested classes
        
        struct Listener;
        class DocumentSession;
        
    public: // methods
        
        Drive(std::string const& name,
              std::string const& host,
              uint16_t api_port,
              uint16_t session_port);
        
        ~Drive() = default;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Returns the API object reference.
        Api& useApi();
        
        //! @brief Set the kiwi api port.
        void setApiPort(uint16_t port);
        
        //! @brief Returns the kiwi api port.
        uint16_t getApiPort() const;
        
        //! @brief Set the kiwi document session port.
        void setSessionPort(uint16_t port);
        
        //! @brief Returns the kiwi document session port.
        uint16_t getSessionPort() const;
        
        //! @brief Set both the api's and session's host.
        void setHost(std::string const& host);
        
        //! @brief Returns the session host.
        std::string const& getHost() const;
        
        //! @brief Set the name of this drive.
        void setName(std::string const& host);
        
        //! @brief Returns the name of this drive.
        std::string const& getName() const;
        
        //! @brief Creates and opens a new document on this drive.
        void createNewDocument();
        
        //! @brief Returns the documents.
        std::list<DocumentSession> const& getDocuments() const;
        
        //! @brief Returns the documents.
        std::list<DocumentSession>& getDocuments();
        
        //! @brief Returns true if the drive match the other drive
        //! @details this operator only compares ip and port.
        bool operator==(Drive const& drive) const;
        
        //! @brief Refresh all the document list.
        void refresh();
        
    private: // members
        
        Api                         m_api;
        uint16_t                    m_session_port = 9090;
        std::string                 m_name = "Drive";
        std::list<DocumentSession>  m_documents;
        engine::Listeners<Listener> m_listeners;
        
        friend class DocumentBrowser;
    };
    
    // ================================================================================ //
    //                           DOCUMENT BROWSER LISTENER                           //
    // ================================================================================ //
    
    //! @brief Listen to document browser changes.
    struct DocumentBrowser::Drive::Listener
    {
        //! @brief Destructor.
        virtual ~Listener() = default;
        
        //! @brief Called when a document session has been added.
        virtual void documentAdded(DocumentBrowser::Drive::DocumentSession& doc) = 0;
        
        //! @brief Called when a document session changed.
        virtual void documentChanged(DocumentBrowser::Drive::DocumentSession& doc) = 0;
        
        //! @brief Called when a document session has been removed.
        virtual void documentRemoved(DocumentBrowser::Drive::DocumentSession& doc) = 0;
    };
    
    // ================================================================================ //
    //                                  DRIVE DOCUMENT                                  //
    // ================================================================================ //
    
    class DocumentBrowser::Drive::DocumentSession
    {
    public: // methods
        
        //! @brief Constructor.
        DocumentSession(DocumentBrowser::Drive& parent, Api::Document document);
        
        //! @brief Destructor.
        ~DocumentSession();
        
        //! @brief Returns the document drive
        Drive& useDrive();
        
        //! @brief Tells the Kiwi instance to open up this document.
        void open();
        
        //! @brief Returns the document name
        std::string getName() const;
        
        //! @brief Returns the document session host
        std::string getHost() const;
        
        //! @brief Returns the session id of the document.
        uint64_t getSessionId() const;
        
        //! @brief Returns the document session port.
        uint16_t getSessionPort() const;
        
        //! @brief Returns the drive that holds this document.
        DocumentBrowser::Drive const& useDrive() const;
        
        //! @brief Rename the document.
        void rename(std::string const& new_name);
        
        //! @brief Returns true if the DocumentSession match another DocumentSession
        //! @details this operator uses the session_id field to compare.
        bool operator==(DocumentSession const& other_doc) const;
        
    private: // members
        
        DocumentBrowser::Drive&             m_drive;
        Api::Document                       m_document;
        
        friend class DocumentBrowser::Drive;
    };
}


#endif // KIWI_APP_DOCUMENT_BROWSER_HPP_INCLUDED
