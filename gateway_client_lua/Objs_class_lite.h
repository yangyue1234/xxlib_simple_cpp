﻿#pragma once
#include "xx_object.h"
#include "Objs_class_lite.h.inc"  // user create it for extend include files
namespace Objs {
	struct PkgGenMd5 {
		inline static const std::string value = "#*MD5<074b6bfcb0e142cf0fbf6ff20e3dda2f>*#";
    };
	struct PkgGenTypes {
        static void RegisterTo(xx::ObjectHelper& oh);
    };

    struct FishBase;
    struct CppFish;
    struct LuaFishBase;
}
namespace xx {
    template<> struct TypeId<Objs::CppFish> { static const uint16_t value = 1; };
    template<> struct TypeId<Objs::LuaFishBase> { static const uint16_t value = 2; };
}
namespace Objs {

    struct FishBase : xx::Object {
        XX_GENCODE_OBJECT_H(FishBase, xx::Object)
        int32_t n = 0;
#include "Objs_FishBase.inc"
    };
    struct CppFish : Objs::FishBase {
        XX_GENCODE_OBJECT_H(CppFish, Objs::FishBase)
#include "Objs_CppFish.inc"
    };
    struct LuaFishBase : Objs::FishBase {
        XX_GENCODE_OBJECT_H(LuaFishBase, Objs::FishBase)
        std::string fileName;
    };
}
namespace xx {
}
#include "Objs_class_lite_.h.inc"  // user create it for extend include files at the end
