#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "reproc" for configuration "Debug"
set_property(TARGET reproc APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(reproc PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libreproc.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS reproc )
list(APPEND _IMPORT_CHECK_FILES_FOR_reproc "${_IMPORT_PREFIX}/lib/libreproc.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
