#include "error.h"


namespace {
    
    struct KeyBindErrorsCategory : std::error_category
    {
    const char* name() const noexcept override;
    std::string message(int ev) const override;
    };
    
    const char* KeyBindErrorsCategory::name() const noexcept
    {
    return "flights";
    }
    
    std::string KeyBindErrorsCategory::message(int ev) const
    {
        switch (static_cast<KeyBindErrors>(ev))
        {
        case KeyBindErrors::NoMatchesFound:
            return "No such key found";
        
        case KeyBindErrors::KeyAlreadyBound:
            return "The key is already bound to an action";
        
        case KeyBindErrors::InvalidSearchArgument:
            return "Invalid search argument for key to look up";

        case KeyBindErrors::InvalidEvent:
            return "No such event exists to be bound to a key";

        default:
            return "(unrecognized error)";
        }
    }
    
    const KeyBindErrorsCategory keyBindErrors {};
    
}
 
std::error_code make_error_code(KeyBindErrors e)
{
  return {static_cast<int>(e), keyBindErrors};
}



/*
*
*   RENDERER ERRORS
*
*/

namespace {
    
    struct RendererErrorsCategory : std::error_category
    {
    const char* name() const noexcept override;
    std::string message(int ev) const override;
    };
    
    const char* RendererErrorsCategory::name() const noexcept
    {
    return "flights";
    }
    
    std::string RendererErrorsCategory::message(int ev) const
    {
        switch (static_cast<RendererErrors>(ev))
        {
        case RendererErrors::InvalidCameraPos:
            return "New camera position is invalid (correct format: d.d,d.d,d.d)";

        default:
            return "(unrecognized error)";
        }
    }
    
    const RendererErrorsCategory rendererErrors {};
    
}
 
std::error_code make_error_code(RendererErrors e)
{
  return {static_cast<int>(e), rendererErrors};
}