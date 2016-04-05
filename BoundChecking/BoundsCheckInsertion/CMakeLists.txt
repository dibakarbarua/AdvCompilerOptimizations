add_library(LLVMBoundsCheckInsert MODULE BoundsCheckInsert.cpp)

#target_compile_features(LLVMNewpass PRIVATE cxx_range_for cxx_auto_type)
set(CMAKE_CXX_FLAGS 	"-std=c++11")

set_target_properties(LLVMBoundsCheckInsert PROPERTIES COMPILE_FLAGS "-fno-rtti"
)

#add_llvm_loadable_module( LLVMHello
#  Hello.cpp
#
#  DEPENDS
#  intrinsics_gen
#  )

