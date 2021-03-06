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

#include "KiwiHttp.h"

namespace kiwi { namespace network { namespace http {
    
    // ================================================================================ //
    //                                     PAYLOAD                                      //
    // ================================================================================ //
    
    class Payload
    {
    public:
        
        struct Pair;
        
        Payload() = default;
        
        template <class It>
        Payload(const It begin, const It end);
        
        Payload(std::initializer_list<Pair> const& pairs);
        
        void AddPair(Pair const& pair);
        
        std::string content;
    };
    
    // ================================================================================ //
    //                                     PARAMETERS                                   //
    // ================================================================================ //
    
    class Parameters
    {
    public:
        
        struct Parameter;
        
        Parameters() = default;
        Parameters(const std::initializer_list<Parameter>& parameters);
        
        void AddParameter(Parameter const& parameter);
        
        std::string content;
    };
    
    struct Parameters::Parameter
    {
        template <typename KeyType, typename ValueType>
        Parameter(KeyType&& key, ValueType&& value);
        
        std::string key;
        std::string value;
    };
    
    // ================================================================================ //
    //                                       BODY                                       //
    // ================================================================================ //
    
    class Body
    {
    public:
        
        Body() = default;
        Body(std::string const& body);
        
        std::string content;
    };
    
    // ================================================================================ //
    //                                     SESSION                                      //
    // ================================================================================ //
    
    class Session
    {
    public: // methods
        
        using Response = http::Response<beast::http::string_body>;
        using AsyncResponse = std::future<void>;
        using Callback = std::function<void(Response)>;
        
        Session();
        ~Session() = default;
        
        void setHost(std::string const& host);
        void setPort(std::string const& port);
        void setTarget(std::string const& endpoint);
        void setTimeout(Timeout timeout);
        void setAuthorization(std::string const& auth);
        
        void setParameters(Parameters && parameters);
        void setPayload(Payload && payload);
        void setBody(std::string const& content);
        
        bool isPending();
        void cancel();
        
        Response Get();
        void GetAsync(Callback callback);
        
        Response Post();
        void PostAsync(Callback callback);
        
        Response Put();
        void PutAsync(Callback callback);
        
        Response Delete();
        void DeleteAsync(Callback callback);
        
    private: // variables
        
        std::unique_ptr<Query<beast::http::string_body, Response::body_type>> makeQuery();
        
        Response makeResponse(beast::http::verb verb);
        void makeResponse(beast::http::verb verb, Callback && callback);
        
        std::string         m_port;
        std::string         m_target;
        Parameters          m_parameters;
        Payload             m_payload;
        Body                m_body;
        Timeout             m_timeout;
        std::future<void>   m_future;
        std::atomic<bool>   m_keep_processing;
        
        beast::http::request_header<> m_req_header;
    };
    
}}} // namespace kiwi::network::http

#include "KiwiHttp_Session.hpp"
