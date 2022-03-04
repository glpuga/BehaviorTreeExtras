include(FetchContent)
FetchContent_Declare(spdlog
  GIT_REPOSITORY    https://github.com/gabime/spdlog.git
  GIT_TAG           100f30043f33277122e0991c83845a2617172ffd # tag: v1.8.5
  GIT_SHALLOW       true
  GIT_PROGRESS      true
  )
FetchContent_MakeAvailable(spdlog)
