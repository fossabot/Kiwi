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

#include <KiwiTool/KiwiTool_Listeners.h>

#include <juce_data_structures/juce_data_structures.h>
#include <juce_events/juce_events.h>

#include "../KiwiApp_Network/KiwiApp_Api.h"

#include "../KiwiApp_General/KiwiApp_StoredSettings.h"

#include "KiwiApp_ApiController.h"

namespace kiwi
{
    // ================================================================================ //
    //                                 DOCUMENT BROWSER                                 //
    // ================================================================================ //
    
    //! @brief Request Patcher document informations through a Kiwi API.
    class DocumentBrowser : public juce::Timer
    {
    public: // nested classes
        
        class Drive;
        
    public: // methods
        
        //! @brief Constructor
        DocumentBrowser(std::string const& drive_name, int refresh_time);
        
        //! @brief Destructor
        ~DocumentBrowser();
        
        //! @brief Sets the drive's name.
        void setDriveName(std::string const& name);
        
        //! @brief juce::Timer callback.
        void timerCallback() override;
        
        //! @brief Returns a list of drives.
        Drive* getDrive() const;
        
    private: // methods
        
        //! @brief Handles request that denied by server.
        static void handleDeniedRequest();
        
    private: // variables
        
        std::unique_ptr<Drive>      m_distant_drive;
        int                         m_refresh_time;
    };
    
    // ================================================================================ //
    //                              DOCUMENT BROWSER DRIVE                              //
    // ================================================================================ //
    
    class DocumentBrowser::Drive
    {
    public: // nested classes
        
        struct Listener;
        class DocumentSession;
        
        using DocumentSessions = std::vector<std::unique_ptr<DocumentSession>>;
        
    private:
        
        using Comp = std::function<bool(DocumentSession const& l_hs, DocumentSession const& r_hs)>;
        
    public: // methods
        
        Drive(std::string const& name);
        
        ~Drive() = default;
        
        //! @brief Add a listener.
        void addListener(Listener& listener);
        
        //! @brief remove a listener.
        void removeListener(Listener& listener);
        
        //! @brief Set the name of this drive.
        void setName(std::string const& host);
        
        //! @brief Returns the name of this drive.
        std::string const& getName() const;
        
        //! @brief Uploads a document.
        //! @detail data is represented as a string
        void uploadDocument(std::string const& name, std::string const& data);
        
        //! @brief Creates and opens a new document on this drive.
        void createNewDocument();
        
        //! @brief Changes the way documents are sorted.
        void setSort(Comp comp);
        
        //! @brief Returns the documents.
        DocumentSessions const& getDocuments() const;
        
        //! @brief Returns the documents.
        DocumentSessions& getDocuments();
        
        //! @brief Refresh all the document list.
        void refresh();
        
    private: // methods
        
        //! @brief Refresh the document list without posting network erors.
        void refresh_internal();
        
    private: // members
        
        //! @internal Update the document list (need to be called in the juce Message thread)
        void updateDocumentList(Api::Documents docs);
        
        std::string                 m_name;
        DocumentSessions            m_documents;
        tool::Listeners<Listener>   m_listeners;
        Comp                        m_sort;
        
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
        virtual void documentAdded(DocumentBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when a document session changed.
        virtual void documentChanged(DocumentBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when a document session has been removed.
        virtual void documentRemoved(DocumentBrowser::Drive::DocumentSession& doc) {};
        
        //! @brief Called when one or more documents has been added, removed or changed.
        virtual void driveChanged() {};
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
        
        //! @brief Returns the session id of the document.
        uint64_t getSessionId() const;
        
        //! @brief Returns the open token of the document.
        std::string const& getOpenToken() const;
        
        //! @brief Returns the drive that holds this document.
        DocumentBrowser::Drive const& useDrive() const;
        
        //! @brief Rename the document.
        void rename(std::string const& new_name);
        
        //! @brief Duplicates the document on server side.
        void duplicate();
        
        //! @brief Move the document to trash.
        void trash();
        
        // @brief Moves document out of the trash.
        void untrash();
        
        //! @brief Called to download the document.
        //! @details download is asynchronous and callback is called on the main thread
        //! if request succeed.
        void download(std::function<void(std::string const&)> callback);
        
        //! @brief Returns the date creation as a string.
        std::string const& getCreationDate() const;
        
        //! @brief Returns the author's username.
        std::string const& getAuthor() const;
        
        //! @brief Returns true if document is trashed
        bool isTrashed() const;
        
        //! @brief Returns trashed date as string.
        std::string const& getTrashedDate() const;
        
        //! @brief Returns the last modification date.
        std::string const& getOpenedDate() const;
        
        //! @brief Returns the user that modified document last.
        std::string const& getOpenedUser() const;
        
        //! @brief Returns true if the DocumentSession match another DocumentSession
        //! @details this operator uses the session_id field to compare.
        bool operator==(DocumentSession const& other_doc) const;
        
    private: // members
        
        DocumentBrowser::Drive&             m_drive;
        Api::Document                       m_document;
        std::string                         m_open_token;
        
        friend class DocumentBrowser::Drive;
    };
}
