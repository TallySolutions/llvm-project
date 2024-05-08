set(LLVM_LINK_COMPONENTS
  Support
  )

add_clang_unittest(TWClangFormatTests
  TWClangFormatTest.cpp
  )

clang_target_link_libraries(TWClangFormatTests
  PRIVATE
  clangBasic
  clangFormat
  clangRewrite
  clangToolingCore
  )
