include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY    https://github.com/google/googletest.git
  GIT_TAG           c9461a9b55ba954df0489bab6420eb297bed846b    # tag: v1.10.0
  GIT_SHALLOW       true
  GIT_PROGRESS      true
)

set(INSTALL_GTEST OFF CACHE INTERNAL "Don't install gtest")

FetchContent_MakeAvailable(googletest)