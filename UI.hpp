#ifndef UI_hpp
#define UI_hpp

#include <AntTweakBar.h>
#include "Globals.hpp"

namespace UI {
    
    template <typename T> struct Type2Tweak {
        static const TwType value{ TW_TYPE_UNDEF };
    };
    
    template<>	struct Type2Tweak<int>    { static const TwType value{ TW_TYPE_INT32 }; };
    template<>	struct Type2Tweak<bool>   { static const TwType value{ TW_TYPE_BOOLCPP }; };
    template<>	struct Type2Tweak<float>  { static const TwType value{ TW_TYPE_FLOAT }; };
    
    template <typename T> void AddVariableRW(const char *name, T *var, const char *def) {
        TwAddVarRW(Globals::mainBar, name, Type2Tweak<T>::value, var, def);
    }
    
    inline void AddColour(const char *name, float *col, const char *def) {
        TwAddVarRW(Globals::mainBar, name, TW_TYPE_COLOR4F, col, def);
    }
    
    inline void AddDirection(const char *name, float *dir, const char *def) {
        TwAddVarRW(Globals::mainBar, name, TW_TYPE_DIR3F, dir, def);
    }
    
    template <typename T> void AddVariableRO(const char *name, T *var, const char *def) {
        TwAddVarRO(Globals::mainBar, name, Type2Tweak<T>::value, var, def);
    }
    
    inline void AddSeparator() {
        TwAddSeparator(Globals::mainBar, NULL, NULL);
    }
}

#endif /* UI_hpp */
