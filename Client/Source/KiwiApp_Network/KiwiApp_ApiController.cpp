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

#include "KiwiApp_ApiController.h"

#include "../KiwiApp_General/KiwiApp_IDs.h"

#include "../KiwiApp.h"

namespace kiwi
{
    ApiController::ApiController()
    {
        auto& settings = getAppSettings().network();
        
        setHost(settings.getHost());
        setPort(settings.getApiPort());
        
        restoreAuthUserProfile();
    }
    
    ApiController::~ApiController()
    {
        auto& settings = getAppSettings().network();
        
        if(!settings.getRememberUserFlag())
        {
            clearToken();
        }
        
        saveAuthUserProfile();
    }
    
    bool ApiController::saveAuthUserProfile() const
    {
        return saveJsonToFile("UserProfile", {{"user", getAuthUser()}});
    }
    
    bool ApiController::restoreAuthUserProfile()
    {
        auto j = getJsonFromFile("UserProfile");
        if(j.count("user"))
        {
            from_json(j["user"], m_auth_user);
            return true;
        }
        
        return false;
    }
    
    void ApiController::login(std::string const& name_or_email,
                              std::string const& password,
                              std::function<void()> success_callback,
                              Api::ErrorCallback error_callback)
    {
        auto success_cb = [this, success_callback = std::move(success_callback)](Api::AuthUser user){
            
            auto& scheduler = KiwiApp::useInstance().useScheduler();
            scheduler.schedule([this, success_callback, user](){
                
                m_auth_user.resetWith(user);
                
                success_callback();
            });
        };
        
        KiwiApp::useApi().login(name_or_email, password, std::move(success_cb), std::move(error_callback));
    }
    
    void ApiController::signup(std::string const& username,
                               std::string const& email,
                               std::string const& password,
                               std::function<void(std::string)> success_callback,
                               Api::ErrorCallback error_callback)
    {
        auto success_cb = [this, success_callback = std::move(success_callback)](std::string message){
            
            auto& scheduler = KiwiApp::useInstance().useScheduler();
            scheduler.schedule([this, success_callback, message](){
                success_callback(message);
            });
        };
        
        KiwiApp::useApi().signup(username, email, password,
                                 std::move(success_cb), std::move(error_callback));
    }
    
    void ApiController::logout()
    {
        Api::Controller::clearToken();
    }
}
